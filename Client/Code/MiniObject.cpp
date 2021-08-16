#include "stdafx.h"
#include "MiniObject.h"
#include "Export_Function.h"
#include "DynamicCamera.h"
#include "Camera.h"
CMiniObject::CMiniObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex)
	:Engine::CGameObject(pGraphicDev), m_wstrTexture(wstrTex)
{
	D3DXMatrixIdentity(&m_MatView);
	D3DXMatrixIdentity(&m_MatView);

}

CMiniObject::~CMiniObject(void)
{
}

HRESULT CMiniObject::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	return S_OK;
}

_int CMiniObject::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	CalculateTransform();
	if (m_wstrTexture == L"Texture_MiniPlayer")
	{
		Engine::CTransform* m_pPlayerTransformCom = dynamic_cast<Engine::CTransform*>(
			Engine::Get_Component(L"02.GameLogic", L"Obj_Player", L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPlayerTransformCom, E_FAIL);
		m_vPos = { m_pPlayerTransformCom->m_vInfo[Engine::INFO_POS].x * 7,m_pPlayerTransformCom->m_vInfo[Engine::INFO_POS].z * 7 ,0.f };
		m_vPos += m_vParentPos;
		
	}
	if (m_pObjectEnable != nullptr)
	{
		Set_Enable(*m_pObjectEnable);
	}
	return 0;
}

void CMiniObject::Render_GameObject(void)
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

HRESULT CMiniObject::Add_Component(void)
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

void CMiniObject::CalculateTransform()
{
	SetScale();
	SetPos();
	m_MatView = m_MatScale * m_MatRot * m_MatPos;
	//SetPivot();
}

void CMiniObject::SetPivot()
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


void CMiniObject::SetScale()
{
	//m_MatView._11 *= m_pTextureCom->Get_ImageInfo().Width * m_vScale.x * 0.1f;
	//m_MatView._22 *= m_pTextureCom->Get_ImageInfo().Height* m_vScale.y * 0.1f; 
	if (m_wstrTexture == L"Texture_MiniWall")
	{
		D3DXMatrixScaling(&m_MatScale,
			m_pTextureCom->Get_ImageInfo().Width * m_vScale.x * 0.025f,
			m_pTextureCom->Get_ImageInfo().Height* m_vScale.y * 0.025f,
			0.f); // 다이렉트 크기행렬 함수
	}
	else if (m_wstrTexture == L"Texture_MiniItem")
	{
		D3DXMatrixScaling(&m_MatScale,
			m_pTextureCom->Get_ImageInfo().Width * m_vScale.x * 0.07f,
			m_pTextureCom->Get_ImageInfo().Height* m_vScale.y * 0.07f, 0.f);
	}
	else if(m_wstrTexture == L"Texture_MiniPlayer")
	{
		D3DXMatrixScaling(&m_MatScale,
			m_pTextureCom->Get_ImageInfo().Width * m_vScale.x * 0.1f,
			m_pTextureCom->Get_ImageInfo().Height* m_vScale.y * 0.1f,
			0.f); // 다이렉트 크기행렬 함수
	}

}

void CMiniObject::SetPos()
{
	//memcpy(&(m_MatView._41), m_vPos, sizeof(_float) * 3);

	D3DXMatrixTranslation(&m_MatPos,
		m_vPos.x ,
		m_vPos.y ,
		m_vPos.z); // 다이렉트 이동행렬 함수
}




void CMiniObject::Set_Transform2D(Engine::CTransform * pTransform)
{
    m_vPos = { pTransform->m_vInfo[Engine::INFO_POS].x * 7,pTransform->m_vInfo[Engine::INFO_POS].z * 7,0.0f };
	//m_vPos = {0,0,0.1f };
	m_vPos += m_vParentPos;
	m_vScale = { pTransform->m_vScale.x,pTransform->m_vScale.z,0.f };

	D3DXMatrixRotationZ(&m_MatRot, pTransform->m_vAngle.y);
}




CMiniObject * CMiniObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, Engine::CTransform* pTransform, _vec3 vParentPos, bool* pObjectEnabel)
{
	CMiniObject*	pInstance = new CMiniObject(pGraphicDev, wstrTex);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);
	pInstance->m_vParentPos = vParentPos;
	pInstance->Set_Transform2D(pTransform);
	pInstance->CalculateTransform();
	pInstance->m_pObjectEnable = pObjectEnabel;
	return pInstance;
}



void CMiniObject::Free(void)
{
	Engine::CGameObject::Free();

}
