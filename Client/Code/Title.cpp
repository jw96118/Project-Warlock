#include "stdafx.h"
#include "Export_Function.h"
#include "DynamicCamera.h"
#include "Camera.h"
#include "Title.h"
CTitle::CTitle(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, SCROLLDIR eScrollDIr, _vec3 vPos, _float fScale, _float fSpd)
	:Engine::CGameObject(pGraphicDev),
	m_vPos(vPos),
	m_fScale(fScale),
	m_wstrTexture(wstrTex),
	m_eSCroll(eScrollDIr),
	m_fSpd(fSpd)
{
	
	m_eSCroll ? m_fScroll = 0.49f : m_fScroll = -1.49f;

	D3DXMatrixIdentity(&m_MatView);
	D3DXMatrixIdentity(&m_MatView);
}

CTitle::~CTitle(void)
{
}

HRESULT CTitle::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	CalculateTransform();

	return S_OK;
}

_int CTitle::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	
	if (m_fScroll >= 0.5f)
		m_fScroll = -0.499f;
	if (m_fScroll <= -0.5f)
		m_fScroll = 0.499f;

	
	m_eSCroll ? m_fScroll += fTimeDelta*+m_fSpd :m_fScroll += fTimeDelta*-m_fSpd;
		
	//swprintf_s(m_szScrollVal, L"ScrollVal=: %f  ", m_fScroll);


	return 0;
}

void CTitle::Render_GameObject(void)
{

	//Engine::Render_Font(L"Font_Default", m_szScrollVal, &_vec2(10.f, 30.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	_matrix mat_OldWolrd, mat_OldView, mat_Perpective;
	//Back up Preivous Value
	//m_pGraphicDev->GetTransform(D3DTS_WORLD, &mat_OldWolrd);
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &mat_OldView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &mat_Perpective);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &mat_Perpective);

	//Apply Orthogonal Projection
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_MatView);

	//Set Alpha Test
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Render UI
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Render_Texture();

	m_pBufferCom->SendToClient(m_fScroll);
	m_pBufferCom->Render_Buffer();
	//Revert Previous Value
	//m_pBufferCom->SendToClient(0.0f, 1.0f, 0.0f, 1.0f);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat_OldWolrd);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &mat_OldView);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &mat_Perpective);

}

HRESULT CTitle::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CScrollTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_ScrollTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_LOGO, m_wstrTexture.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	return S_OK;

}

void CTitle::CalculateTransform()
{
	SetScale();
	SetPos();
}


void CTitle::SetScale()
{
	m_MatView._11 *= m_pTextureCom->Get_ImageInfo().Width*m_fScale;
	m_MatView._22 *= m_pTextureCom->Get_ImageInfo().Height*m_fScale;
}

void CTitle::SetPos()
{	
	memcpy(&(m_MatView._41), m_vPos, sizeof(_float) * 3);
}

CTitle * CTitle::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, SCROLLDIR eScrollDIr, _vec3 vPos, _float fScale, _float fSpd)
{
	CTitle*	pInstance = new CTitle(pGraphicDev,wstrTex, eScrollDIr,vPos, fScale,fSpd);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}



void CTitle::Free(void)
{
	Engine::CGameObject::Free();

}
