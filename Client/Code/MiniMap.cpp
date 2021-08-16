#include "stdafx.h"
#include "MiniMap.h"
#include "MiniObject.h"
#include "Export_Function.h"
#include "DynamicCamera.h"
#include "Camera.h"
CMiniMap::CMiniMap(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex,_vec3 vPos , _float fScale,_int iPivot)
	:Engine::CGameObject(pGraphicDev), 
	m_vPos(vPos),
	m_fScale(fScale),
	m_iPivot(iPivot),
	m_wstrTexture(wstrTex)	
{
	D3DXMatrixIdentity(&m_MatView);
	D3DXMatrixIdentity(&m_MatView);

}

CMiniMap::~CMiniMap(void)
{
}

HRESULT CMiniMap::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	CalculateTransform();

	return S_OK;
}

_int CMiniMap::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return 0;
}

void CMiniMap::Render_GameObject(void)
{
	_matrix mat_OldWolrd, mat_OldView, mat_Perpective;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &mat_Perpective);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_MatView);

	//Set Alpha Test
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Render UI
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	//Revert Previous Value
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat_OldWolrd);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &mat_OldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &mat_Perpective);
}

HRESULT CMiniMap::Add_Component(void)
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

void CMiniMap::CalculateTransform()
{
	SetScale();
	SetPos();
	SetPivot();
}

void CMiniMap::SetPivot()
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

void CMiniMap::SetScale()
{
	m_MatView._11 *= m_pTextureCom->Get_ImageInfo().Width * m_fScale;
	m_MatView._22 *= m_pTextureCom->Get_ImageInfo().Height* m_fScale;

}

void CMiniMap::SetPos()
{	
	memcpy(&(m_MatView._41), m_vPos, sizeof(_float) * 3);
}

HRESULT CMiniMap::Add_WallObject(Engine::CGameObject* pGameObject,wstring TexName )
{
	/*CMiniObject* tempWall = CMiniObject::Create(m_pGraphicDev, TexName, pGameObject->GetTransformCom());
	m_vecWall.push_back(tempWall);*/
	return S_OK;
}

CMiniMap * CMiniMap::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, _vec3 vPos, _float fScale,_int iPivot)
{
	CMiniMap*	pInstance = new CMiniMap(pGraphicDev,wstrTex, vPos, fScale,iPivot);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}



void CMiniMap::Free(void)
{
	for_each(m_vecWall.begin(), m_vecWall.end(), Safe_Release<CMiniObject*>);
	Engine::CGameObject::Free();

}
