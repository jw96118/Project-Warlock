#include "stdafx.h"
#include "Monster.h"

#include "Export_Function.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName)
	: Engine::CGameObject(pGraphicDev),
	m_pLayerTag(pLayerTag),
	m_wstrObjName(wstrObjName),
	m_bAttack(false),
	m_bShoot(false),
	m_bHit(false),
	m_bShootEnd(false),
	m_bDeath(false),
	m_uiSoundFlag(0)
{
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
}

CMonster::~CMonster(void)
{

}

const BOXCOLLIDER & CMonster::Get_BoxCollider() const
{
	return m_tCollider;
}

const SPHERECOLLIDER & CMonster::Get_HitCollider() const
{
	return m_tHitColl;
}

void CMonster::Get_Damage(_float fDamage)
{
	m_tData.fCurHP -= fDamage;
	if (m_tData.fCurHP <= 0.f)
		m_tData.fCurHP = 0.f;
}

HRESULT CMonster::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CMonster::Update_GameObject(const _float& fTimeDelta)
{
	if (m_tData.fCurHP < 0.0f)
		m_tHitColl.fRadius = 0.f;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	_matrix			matBill, matWorld, matView;
	D3DXMatrixIdentity(&matBill);

	SetScale(m_fScale);
	matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_pTransformCom->m_matWorld = matBill * matWorld;

	m_pTransformCom->m_matWorld._42 = m_fScale*0.5f;// 스케일에따른 높이보정
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return NO_EVENT;
}

void CMonster::Render_GameObject(void)
{
}

HRESULT CMonster::LateReady_GameObject(void)
{
	return S_OK;
}

HRESULT CMonster::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CLightTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_LightTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

void CMonster::On_SoundFlag(_uint uiFlag, wstring AudioName)
{
	if (!(m_uiSoundFlag & uiFlag))
	{
		_bool bSound = false;
		Engine::Play_Sound(AudioName,
			Engine::CHANNELID::EFFECT,
			bSound);
		m_uiSoundFlag |= uiFlag;
	}
}

void CMonster::Free(void)
{
	Engine::CGameObject::Free();
}

void CMonster::Off_SoundFlag(_uint uiFlag)
{
	if (m_uiSoundFlag &uiFlag)
		m_uiSoundFlag ^= uiFlag;

}

void CMonster::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>
		(Engine::Get_Component(L"02.GameLogic", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos,
		pTerrainBufferCom->Get_VtxPos(),
		pTerrainBufferCom->Get_VtxCntX(),
		pTerrainBufferCom->Get_VtxCntZ());

	m_pTransformCom->Set_Pos(vPos.x, fHeight + 4.f, vPos.z);
}

void CMonster::Set_Pos(_float fX, _float fY, _float fZ)
{
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = _vec3(fX, fY, fZ);
}

void CMonster::SetScale(_float fScale)
{
	m_pTransformCom->m_matWorld._11 *= fScale;
	m_pTransformCom->m_matWorld._22 *= fScale;
	m_pTransformCom->m_matWorld._33 *= fScale;

}


void CMonster::Render_HitCollider(void)
{
	_matrix matWorld, matScale, matTrans;

	D3DXMatrixTranslation(&matTrans, m_tHitColl.vCenter.x, m_tHitColl.vCenter.y, m_tHitColl.vCenter.z);
	D3DXMatrixScaling(&matScale, m_tHitColl.fRadius, m_tHitColl.fRadius, m_tHitColl.fRadius);
	matWorld = matScale * matTrans;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	Engine::Render_SphereMesh();
}

void CMonster::Render_BoxCollider(void)
{
	_matrix matWorld, matScale, matTrans;

	D3DXMatrixTranslation(&matTrans,
		(m_tCollider.vMax.x + m_tCollider.vMin.x)*0.5f,
		m_fScale*0.5f,
		(m_tCollider.vMax.z + m_tCollider.vMin.z)*0.5f);
	D3DXMatrixScaling(&matScale,
		m_tCollider.vMax.x - m_tCollider.vMin.x,
		m_tCollider.vMax.y - m_tCollider.vMin.y,
		m_tCollider.vMax.z - m_tCollider.vMin.z);
	matWorld = matScale * matTrans;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	Engine::Render_BoxMesh();
}
