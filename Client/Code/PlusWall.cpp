#include "stdafx.h" 
#include "PlusWall.h"
#include "Export_Function.h"

CPlusWall::CPlusWall(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CPlusWall::~CPlusWall(void)
{
}

HRESULT CPlusWall::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CPlusWall::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	return 0;
}

void CPlusWall::Render_GameObject(void)
{
	_matrix matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	if (m_pTextureCom != nullptr)
	{
		m_pTextureCom->Render_Texture(m_iTexCount);
	}
	else
	{
		m_pGraphicDev->SetTexture(0,nullptr);
	}
	if (FAILED(SetUp_Material()))
		return;
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CPlusWall::Set_Transform(Engine::CGameObject * pGameObjcet, float high)
{
	for(int i =0; i<Engine::INFO_END;i++)
		m_pTransformCom->m_vInfo[i] = pGameObjcet->GetTransformCom()->m_vInfo[i];
	m_pTransformCom->m_vAngle = pGameObjcet->GetTransformCom()->m_vAngle;
	m_pTransformCom->m_vScale = pGameObjcet->GetTransformCom()->m_vScale;


	m_pTransformCom->m_vInfo[Engine::INFO_POS].y += high;

	m_iTexCount = pGameObjcet->Get_TexCount();
}



HRESULT CPlusWall::Add_Component(void)
{

	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CLightTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_LightTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);


	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Wall"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

HRESULT CPlusWall::SetUp_Material(void)
{
	D3DMATERIAL9		tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);


	m_pGraphicDev->SetMaterial(&tMtrlInfo);

	return S_OK;
}




CPlusWall* CPlusWall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlusWall* pInstance = new CPlusWall(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	
	return pInstance;
}



void CPlusWall::Free(void)
{

	Engine::CGameObject::Free();

}

