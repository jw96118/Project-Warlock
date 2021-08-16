#include "stdafx.h"
#include "EventDoor.h"
#include "Export_Function.h"
#include "CollisionMgr.h"
CEventDoor::CEventDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev) , m_bUp(false), m_fUpSpeed(3.f)
{
	ZeroMemory(&m_tBoxCollider, sizeof(m_tBoxCollider));
}

CEventDoor::~CEventDoor(void)
{

}

BOXCOLLIDER * CEventDoor::Get_BoxCollider(void)
{
	return &m_tBoxCollider;
}



HRESULT CEventDoor::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

HRESULT CEventDoor::LateReady_GameObject(void)
{


	return S_OK;
}

_int CEventDoor::Update_GameObject(const _float& fTimeDelta)
{

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	Update_Collider(fTimeDelta);



	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	return NO_EVENT;
}

void CEventDoor::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//Set Alpha Test
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Render UI
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CEventDoor::Update_Collider(const _float& fTimeDelta)
{
	_vec3 vPos = { 0.f, 0.f, 0.f };
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	if (m_pTransformCom->m_vInfo[Engine::INFO_POS].y <= 3)
	{
		if (m_bUp)
		{
			m_pTransformCom->m_vInfo[Engine::INFO_POS].y += fTimeDelta;
		}
	}
	m_tBoxCollider.vMax = { vPos.x + m_ColSize.x, vPos.y + m_ColSize.y, vPos.z + m_ColSize.z };
	m_tBoxCollider.vMin = { vPos.x - m_ColSize.x, vPos.y - m_ColSize.y, vPos.z - m_ColSize.z };


}

void CEventDoor::Set_Transform(_vec3 scale, _vec3 pos,  _vec3 angle)
{
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = pos;
	m_pTransformCom->m_vScale = scale;
	m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(angle.x));
	m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(angle.y));
	m_pTransformCom->Rotation(Engine::ROT_Z, D3DXToRadian(angle.z));

}

void CEventDoor::Set_ColSize(_vec3 colSize)
{
	m_ColSize = colSize;
}

void CEventDoor::Set_Up(bool flag)
{
	m_bUp = flag;
}



HRESULT CEventDoor::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	/* For Debug */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CLightTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_LightTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_EventDoor"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}



CEventDoor* CEventDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEventDoor*	pInstance = new CEventDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CEventDoor::Free(void)
{
	Engine::CGameObject::Free();
}