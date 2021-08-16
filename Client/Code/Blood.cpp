#include "stdafx.h"
#include "Blood.h"
#include "FirstPersonCamera.h"
#include "CollisionMgr.h"
#include "Monster.h"
#include "ColObject.h"

#include "Export_Function.h"

CBlood::CBlood(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
}

CBlood::~CBlood(void)
{
}

HRESULT CBlood::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(CEffect::Ready_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pCamera = dynamic_cast<CFirstPersonCamera*>(Engine::Get_GameObject(L"01.Environment", L"MainCamera"));
	NULL_CHECK_RETURN(m_pCamera, E_FAIL);

	D3DXMatrixScaling(&m_matScale, 0.8f, 0.8f, 0.8f);

	m_tFrame.fMaxFrameCnt = m_pTextureCom->Get_TextureFrameSize();
	m_tFrame.fTimeScale = 3.f;

	return S_OK;
}

_int CBlood::Update_GameObject(const _float & fTimeDelta)
{
	Animator_GameObject(fTimeDelta);

	Update_WorldMatrix();

	if(m_bEnable)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	return NO_EVENT;
}

void CBlood::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);

	m_pTextureCom->Render_Texture((_uint)m_tFrame.fFrameCnt);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBlood::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Effect_Blood"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Blood", pComponent);

	return S_OK;
}

void CBlood::Update_Collider(void)
{
	m_tCollider.vCenter = m_vPos;
}

void CBlood::OnEffect(const _vec3& vPos)
{
	m_vPos = vPos;

	D3DXMatrixTranslation(&m_matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	Set_Enable(true);
}

void CBlood::Animator_GameObject(const _float & fTimeDelta)
{
	m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

	if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
	{
		m_tFrame.fFrameCnt = 0.f;
		Set_Enable(false);
	}
}

void CBlood::Update_WorldMatrix(void)
{
	_matrix matView = *m_pCamera->Get_ViewSpace();
	_matrix matBill;

	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_matWorld = m_matScale * m_matTrans;

	m_matWorld = matBill * m_matWorld;
}

CBlood * CBlood::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pLayerTag, const _tchar * pObjectTag)
{
	CBlood*	pInstance = new CBlood(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	Engine::CLayer* pLayer = Engine::Get_Layer(L"09.Effect");
	pLayer->Add_GameObject(pObjectTag, pInstance);

	return pInstance;
}

void CBlood::Free(void)
{
	CEffect::Free();
}