#include "stdafx.h"
#include "ColObject.h"
#include "Export_Function.h"

CColObject::CColObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tCollider, sizeof(m_tCollider));
}

CColObject::~CColObject(void)
{
}

const BOXCOLLIDER * CColObject::Get_Collider(void) const
{
	return &m_tCollider;
}



HRESULT CColObject::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

HRESULT CColObject::LateReady_GameObject(void)
{
	_vec3 vPivot = { 0.f, 0.f, 0.f };
	_vec3 vScale = { 0.f, 0.f, 0.f };

	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPivot);
	vScale = m_pTransformCom->m_vScale;

	_float fInvervalX = vScale.x * 0.5f;
	_float fInvervalY = vScale.y * 0.5f;
	_float fInvervalZ = vScale.z * 0.5f;

	m_tCollider.vMax = { vPivot.x + fInvervalX , vPivot.y + fInvervalY, vPivot.z + fInvervalZ };
	m_tCollider.vMin = { vPivot.x - fInvervalX , vPivot.y - fInvervalY, vPivot.z - fInvervalZ };

	return S_OK;
}

_int CColObject::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	return 0;
}

void CColObject::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	Engine::Render_BoxMesh();
}

HRESULT CColObject::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

CColObject* CColObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CColObject* pInstance = new CColObject(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CColObject::Free(void)
{
	Engine::CGameObject::Free();
}

