#include "pch.h"
#include "ColObject.h"
#include "MainFrm.h"
#include "Export_Function.h"

CColObject::CColObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CColObject::~CColObject(void)
{

}

HRESULT CColObject::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXCreateLine(m_pGraphicDev, &m_pLine);
	m_pLine->SetWidth(5.f);
	m_pLine->SetAntialias(true);

	return S_OK;
}

_int CColObject::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	return 0;
}

void CColObject::Render_GameObject(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pTextureCom->Render_Texture();
	Engine::Render_BoxMesh();

	DrawLine();	
}

HRESULT CColObject::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = { 0,1,0 };
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);


	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Test"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);


	return S_OK;
}

void CColObject::DrawLine()
{
	if (m_bSelect)
	{
	
		_matrix m_matView, m_matProj;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &m_matView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &m_matProj);

		D3DXVECTOR3 pX[2] = { {0,0,0 },
						  {0.7f,0,0} };
		D3DXVECTOR3 pY[2] = { {0,0,0 },
						  {0,0.7f,0} };
		D3DXVECTOR3 pZ[2] = { {0,0,0 },
						  {0,0,-0.7f} };

		m_pLine->Begin();
		m_pLine->DrawTransform(pX, 2, &(m_pTransformCom->m_matWorld *m_matView* m_matProj), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));    //이때의 행렬은 전체월드행렬*뷰*프로젝션 행렬을 넘겨줘야함
		m_pLine->DrawTransform(pY, 2, &(m_pTransformCom->m_matWorld *m_matView* m_matProj), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));    //이때의 행렬은 전체월드행렬*뷰*프로젝션 행렬을 넘겨줘야함
		m_pLine->DrawTransform(pZ, 2, &(m_pTransformCom->m_matWorld *m_matView* m_matProj), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));    //이때의 행렬은 전체월드행렬*뷰*프로젝션 행렬을 넘겨줘야함
		m_pLine->End();
	}
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
	if(m_pLine != nullptr)
		m_pLine->Release();
	Engine::CGameObject::Free();

}

