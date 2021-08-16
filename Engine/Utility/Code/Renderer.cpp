#include "Renderer.h"
#include "GraphicDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

Engine::CRenderer::CRenderer(void)
{

}

Engine::CRenderer::~CRenderer(void)
{
	Free();
}

void CRenderer::Set_Perspective(const _matrix & matPerspective)
{
	m_matPerspective = matPerspective;
}

void Engine::CRenderer::Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject)
{
	if (RENDER_END <= eGroup)
		return;

	m_RenderGroup[eGroup].push_back(pGameObject);

	pGameObject->AddRef();
}

void Engine::CRenderer::Render_GameObject(void)
{
	// 원근 투영 적용
	RenderState_Projection(PROJ_PERSPECTIVE);

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		// 직교 투영 적용
		if (RENDER_UI == i)
			RenderState_Projection(PROJ_ORTHOGRAPH);
		
		for (auto& iter : m_RenderGroup[i])
		{
			iter->Render_GameObject();
			Safe_Release(iter);
		}

		m_RenderGroup[i].clear();
	}

	RenderState_Projection(PROJ_PERSPECTIVE);
}

void Engine::CRenderer::Clear_RenderGroup(void)
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::RenderState_Projection(PROJECTIONTYPE eProjType)
{
	switch (eProjType)
	{
	case PROJ_PERSPECTIVE:
		CGraphicDev::GetInstance()->Get_Device()->SetTransform(D3DTS_PROJECTION, &m_matPerspective);
		break;
	case PROJ_ORTHOGRAPH:
		RESOLUTION tResolution;
		CGraphicDev::GetInstance()->Get_Resolution(tResolution);
		LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_Device();

		_matrix matWorld, matView, matProj;

		D3DXMatrixIdentity(&matWorld);
		matProj = matView = matWorld;

		D3DXMatrixOrthoLH(&matProj, tResolution.fWidth, tResolution.fHeigth, 0.f, 1.f);

		pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
		pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
		break;
	}
}

void Engine::CRenderer::Free(void)
{
	Clear_RenderGroup();
}

