#include "stdafx.h"
#include "Projectile.h"
#include "Export_Function.h"
#include "Player.h"
#include "CollisionMgr.h"
#include "ColObject.h"

CProjectile::CProjectile(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, _vec3 vPos, _vec3 vDir, _float fSpeed, _float fScale, _float fDamage, PROJECTILE_PATTERN ePattern, D3DXCOLOR lightColor, _float fIntaval)
	:CGameObject(pGraphicDev),
	m_wstrTex(wstrTex),
	m_vPos(vPos),
	m_vDir(vDir),
	m_fTimer(0.f),
	m_fSpeed(fSpeed),
	m_fScale(fScale),
	m_ePattern(ePattern),
	m_fDamage(fDamage),
	m_fPatternVal(0.f),
	m_fIntaval(fIntaval)
{
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
	m_tFrame.fTimeScale = 1.0f;
	m_tLightInfo.Diffuse = lightColor;

}

CProjectile::~CProjectile()
{

}

HRESULT CProjectile::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = m_vPos;
	m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"02.GameLogic", L"Obj_Player", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pTargetTransformCom, -1);


	m_tLightInfo.Type = D3DLIGHT_POINT;
	m_tLightInfo.Range = 45.f;
	m_tLightInfo.Attenuation0 = 0.02f;
	m_tLightInfo.Attenuation1 = 0.06f;
	m_tLightInfo.Attenuation2 = 0.02f;
	m_tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tLightInfo.Position = m_vPos;


	return S_OK;

}

_int CProjectile::Update_GameObject(const _float & fTimeDelta)
{
	m_tLightInfo.Position = m_pTransformCom->m_vInfo[Engine::INFO_POS];
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &m_tLightInfo, 1), E_FAIL);
	_vec3 vToTarget = m_pTargetTransformCom->m_vInfo[Engine::INFO_POS] - m_pTransformCom->m_vInfo[Engine::INFO_POS];
	_float	fLength = D3DXVec3Length(&vToTarget);
	m_fTimer += fTimeDelta;

	Update_Pattern(vToTarget, fLength, fTimeDelta);
	if (m_fTimer >= 2.0f)
	{
		Set_Enable(false);
		m_pGraphicDev->LightEnable(1, false);
	}
	if (fLength <= 1.0f)
	{
		dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"02.GameLogic", L"Obj_Player"))->BeAttack(m_fDamage);
		Set_Enable(false);
		m_pGraphicDev->LightEnable(1, false);
	}

	if (m_bEnable)
	{
		Engine::CLayer* pLayer = Engine::Get_Layer(L"05.Col");

		_float fInterval = m_fScale * 0.5f;

		_vec3 vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);

		BOXCOLLIDER tBoxCollider;
		tBoxCollider.vMin = { vPos.x - fInterval, vPos.y - fInterval, vPos.z - fInterval };
		tBoxCollider.vMax = { vPos.x + fInterval, vPos.y + fInterval, vPos.z + fInterval };

		for (auto iter : pLayer->GetMapObj())
		{
			CColObject* pCol = dynamic_cast<CColObject*>(iter.second);
			if (pCol == nullptr)
				continue;

			BOXCOLLIDER tCollider = *pCol->Get_Collider();

			if (CollisionMgr::IsCollisionBoxToBox(tBoxCollider.vMin, tBoxCollider.vMax, tCollider.vMin, tCollider.vMax))
			{
				Set_Enable(false);
			}
		}
	}

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

	if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
	{
		m_tFrame.fFrameCnt = 0.f;
	}

	SetScale(m_fScale);
	m_pTransformCom->Get_WorldMatrix();

	_matrix			matBill, matWorld, matView;
	D3DXMatrixIdentity(&matBill);
	D3DXMatrixIdentity(&matWorld);

	matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_pTransformCom->m_matWorld = matBill * matWorld;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	return 0;
}

void CProjectile::Render_GameObject(void)
{
	//m_pTransformCom->m_matWorld._42 = 0.6f;//Y 높이 보정
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//Set Alpha Test
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Render UI
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Render_Texture((_uint)m_tFrame.fFrameCnt);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


HRESULT CProjectile::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CLightTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_LightTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, m_wstrTex.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);
	m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom->Get_TextureFrameSize();

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);


	return S_OK;
}

void CProjectile::SetScale(_float fScale)
{
	m_pTransformCom->m_matWorld._11 *= fScale;
	m_pTransformCom->m_matWorld._22 *= fScale;
	m_pTransformCom->m_matWorld._33 *= fScale;

}

void CProjectile::Update_Pattern(_vec3 vToTarget, _float fLength, _float fTimeDelta)
{
	_vec3 vTilePos;
	_vec3 temp;
	m_fPatternVal += fTimeDelta*150.f;
	switch (m_ePattern)
	{
	case CProjectile::PROJECTILE_STRAIGHT:
		m_pTransformCom->m_vInfo[Engine::INFO_POS] += m_vDir*fTimeDelta*m_fSpeed;
		break;
	case CProjectile::PROJECTILE_TRACKING:
		D3DXVec3Normalize(&vToTarget, &vToTarget);
		m_pTransformCom->m_vInfo[Engine::INFO_POS] += ((m_vDir*0.4f) + (vToTarget*0.6f))*fTimeDelta*m_fSpeed;
		break;
	case CProjectile::PROJECTILE_SINE:
	{
		D3DXVec3TransformNormal(&temp, &_vec3{ 1.f, 0, 0 }, &m_pTransformCom->m_matWorld);
		D3DXVec3Normalize(&temp, &temp);

		vTilePos = m_vDir*fTimeDelta*m_fSpeed;
		vTilePos = { vTilePos.x + (temp.x*m_fIntaval*sinf(D3DXToRadian(m_fPatternVal))),vTilePos.y,vTilePos.z + (temp.z*m_fIntaval*sinf(D3DXToRadian(m_fPatternVal))) };
		m_pTransformCom->m_vInfo[Engine::INFO_POS] += ((m_vDir*0.7f) + (vTilePos*0.5f))*fTimeDelta*m_fSpeed;
	}
	break;
	case CProjectile::PRJECTILE_TWISTER:
		break;
	default:
		break;
	}

}

void CProjectile::Set_Intaval(_float fIntaval)
{
	m_fIntaval = fIntaval;
}

CProjectile * CProjectile::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, _vec3 vPos, _vec3 vDir, _float fSpeed, _float fScale,
	_float fDamage, PROJECTILE_PATTERN ePattern, D3DXCOLOR lightColor, _float fIntaval)
{
	CProjectile*	pInstance = new CProjectile(pGraphicDev, wstrTex, vPos, vDir, fSpeed, fScale, fDamage, ePattern, lightColor, fIntaval);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CProjectile::Free(void)
{
	CGameObject::Free();
}
