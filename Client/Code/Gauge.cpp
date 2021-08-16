#include "stdafx.h"
#include "Gauge.h"
#include "Export_Function.h"
#include "DynamicCamera.h"
#include "Camera.h"
CGauge::CGauge(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, Item eType, _vec3 vPos, _float fScale, _int iPivot)
	:Engine::CGameObject(pGraphicDev),
	m_vPos(vPos),
	m_fScale(fScale),
	m_iPivot(iPivot),
	m_wstrTexture(wstrTex),
	m_eItemType(eType)
{
	fill_n(m_fItemVal,(_int)ITEM_END,1.f);
	D3DXMatrixIdentity(&m_MatWorld);
	D3DXMatrixIdentity(&m_MatView);

}

CGauge::~CGauge(void)
{
}

void CGauge::Set_Item(_uint uiItem, _float fVal)
{
	m_fItemVal[uiItem] = fVal;
}

HRESULT CGauge::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	CalculateTransform();

	return S_OK;
}

_int CGauge::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	

	for (int i = 0; i < ITEM_END; i++)
	{
		if (m_fItemVal[i] < 0.f)
			m_fItemVal[i] = 0.f;
	}

	return 0;
}

void CGauge::Render_GameObject(void)
{
	
	_matrix mat_OldWolrd, mat_OldView, mat_Perpective;
	//const Engine::_matrix* mat_Oroth = dynamic_cast<Engine::CCamera*>(m_pCamera)->Get_Ortho();

	////Back up Preivous Value
	m_pGraphicDev->GetTransform(D3DTS_WORLD, &mat_OldWolrd);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &mat_OldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &mat_Perpective);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_MatView);

	//Set Alpha Test
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Render UI
	float f = m_fItemVal[m_eItemType];
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Render_Texture();
	m_pBufferCom->SendToClient(0.0f, m_fItemVal[m_eItemType], 0.0f, 1.0f);
	//m_pBufferCom->SendToClient();
	m_pBufferCom->Render_Buffer();
	
	//Revert Previous Value
	m_pBufferCom->SendToClient(0.0f, 1.0f, 0.0f, 1.0f);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat_OldWolrd);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &mat_OldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &mat_Perpective);

}

HRESULT CGauge::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, m_wstrTexture.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	return S_OK;

}

void CGauge::CalculateTransform()
{
	SetScale();
	SetPos();
	SetPivot();
}

void CGauge::SetPivot()
{
	switch (m_iPivot)
	{
	case 0:
		m_MatView._41 += (m_MatView._11*0.5f);
		m_MatView._42 -= (m_MatView._22*0.5f);
		break;
	case 1:
		m_MatView._41 -= (m_MatView._11*0.5f);
		m_MatView._42 -= (m_MatView._22*0.5f);
		break;
	case 2:
		m_MatView._41 -= (m_MatView._11*0.5f);
		m_MatView._42 += (m_MatView._22*0.5f);
		break;
	case 3:
		m_MatView._41 += (m_MatView._11*0.5f);
		m_MatView._42 += (m_MatView._22*0.5f);
		break;
	default:
		break;
	}
}

void CGauge::SetScale()
{
	m_MatView._11 *= m_pTextureCom->Get_ImageInfo().Width*m_fScale;
	m_MatView._22 *= m_pTextureCom->Get_ImageInfo().Height*-m_fScale;
}

void CGauge::SetPos()
{	
	memcpy(&(m_MatView._41), m_vPos, sizeof(_float) * 3);
}

CGauge * CGauge::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, Item eType, _vec3 vPos, _float fScale,_int iPivot)
{
	CGauge*	pInstance = new CGauge(pGraphicDev,wstrTex, eType,vPos, fScale,iPivot);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}



void CGauge::Free(void)
{
	Engine::CGameObject::Free();

}
