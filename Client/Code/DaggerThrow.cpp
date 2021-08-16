#include "stdafx.h"
#include "DaggerThrow.h"
#include "FirstPersonCamera.h"
#include "CollisionMgr.h"
#include "Monster.h"
#include "ColObject.h"
#include "EffectMgr.h"
#include "Shock.h"

#include "Export_Function.h"

CDaggerThrow::CDaggerThrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
	, m_fSpeed(0.f)
{
}

CDaggerThrow::~CDaggerThrow(void)
{
}

HRESULT CDaggerThrow::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(CEffect::Ready_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pCamera = dynamic_cast<CFirstPersonCamera*>(Engine::Get_GameObject(L"01.Environment", L"MainCamera"));
	NULL_CHECK_RETURN(m_pCamera, E_FAIL);

	D3DXMatrixScaling(&m_matScale, 0.7f, 0.7f, 0.7f);
	D3DXMatrixRotationX(&m_matRotX, D3DXToRadian(90.f));
	D3DXMatrixRotationY(&m_matRotY, D3DXToRadian(-90.f));

	m_tCollider.fRadius = 0.1f;

	m_fSpeed = 20.f;

	return S_OK;
}

_int CDaggerThrow::Update_GameObject(const _float & fTimeDelta)
{
	Move_DaggerThrow(fTimeDelta);
	Update_Collider();

	if (!IsCollision_OtherGameObject())
	{
		Update_WorldMatrix();
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);
	}
	else
		Set_Enable(false);

	return NO_EVENT;
}

void CDaggerThrow::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALS\E);

	//_matrix matWorld, matScale;
	//D3DXMatrixScaling(&matScale, m_tCollider.fRadius, m_tCollider.fRadius, m_tCollider.fRadius);
	//matWorld = matScale * m_matTrans;
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	//Engine::Render_SphereMesh();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


}

HRESULT CDaggerThrow::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Effect_DaggerThrow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_DaggerThrow", pComponent);

	return S_OK;
}

void CDaggerThrow::Update_Collider(void)
{
	m_tCollider.vCenter = m_vPos;
}

void CDaggerThrow::OnEffect(const _vec3& vPos, const _vec3& vMoveDir, const _float& fDamage)
{
	m_vPos = vPos + vMoveDir;
	m_vPos.y -= 0.2f;
	m_vMoveDir = vMoveDir;

	D3DXMatrixRotationY(&m_matCamRotY, D3DXToRadian(m_pCamera->Get_RotationY()));
	D3DXMatrixRotationX(&m_matCamRotX, D3DXToRadian(m_pCamera->Get_RotationX()-10.f));

	Set_Enable(true);
}

void CDaggerThrow::Update_WorldMatrix()
{
	D3DXMatrixTranslation(&m_matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = m_matScale * m_matRotX * m_matRotY * m_matCamRotY * m_matCamRotX * m_matTrans;
}

void CDaggerThrow::Move_DaggerThrow(const _float& fTimeDelta)
{
	m_vPos += m_vMoveDir * fTimeDelta * m_fSpeed;
}

_bool CDaggerThrow::IsCollision_OtherGameObject()
{
	Engine::CLayer*	pLayer = Engine::Get_Layer(L"06.Monster");

	for (auto iter : pLayer->GetMapObj())
	{
		if (!iter.second->Get_Enable())
			continue;

		CMonster* pMonster = dynamic_cast<CMonster*>(iter.second);
		if (pMonster == nullptr ||
			pMonster->Dead_Mon())
			continue;

		const SPHERECOLLIDER& tHitCollider = pMonster->Get_HitCollider();

		_bool bIsCollision = CollisionMgr::IsCollisionSphereToSphere(m_tCollider, tHitCollider);

		if (bIsCollision)
		{
			CShock* pShock = dynamic_cast<CShock*>(m_pEffectMgr->PopEffect(EFFECT_SHOCK));
			pShock->OnEffect(m_vPos);

			pMonster->Damage();
			return true;
		}
	}

	pLayer = Engine::Get_Layer(L"05.Col");
	BOXCOLLIDER tBoxCollider;
	tBoxCollider.vMin = { m_tCollider.vCenter.x - m_tCollider.fRadius, 
						  m_tCollider.vCenter.y - m_tCollider.fRadius, 
						  m_tCollider.vCenter.z - m_tCollider.fRadius };
	tBoxCollider.vMax = { m_tCollider.vCenter.x + m_tCollider.fRadius,
						  m_tCollider.vCenter.y + m_tCollider.fRadius,
						  m_tCollider.vCenter.z + m_tCollider.fRadius };

	for (auto iter : pLayer->GetMapObj())
	{
		CColObject* pCol = dynamic_cast<CColObject*>(iter.second);
		if (pCol == nullptr)
			continue;

		BOXCOLLIDER tCollider = *pCol->Get_Collider();

		if (CollisionMgr::IsCollisionBoxToBox(tBoxCollider.vMin, tBoxCollider.vMax, tCollider.vMin, tCollider.vMax))
		{
			CShock* pShock = dynamic_cast<CShock*>(m_pEffectMgr->PopEffect(EFFECT_SHOCK));
			pShock->OnEffect(m_vPos - m_vMoveDir);

			return true;
		}
	}


	return false;
}

CDaggerThrow * CDaggerThrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pLayerTag, const _tchar * pObjectTag)
{
	CDaggerThrow*	pInstance = new CDaggerThrow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	Engine::CLayer* pLayer = Engine::Get_Layer(L"09.Effect");
	pLayer->Add_GameObject(pObjectTag, pInstance);

	return pInstance;
}

void CDaggerThrow::Free(void)
{
	CEffect::Free();
}