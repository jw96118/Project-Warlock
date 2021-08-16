#include "stdafx.h"
#include "Spell.h"
#include "Export_Function.h"
CSpell::CSpell(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, _vec3 vPos, _float fScale)
	:CGameObject(pGraphicDev),
	m_wstrTex(wstrTex),
	m_vPos(vPos),
	m_fTimer(0.f),
	m_fScale(fScale)
{
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
	m_tFrame.fTimeScale = 1.0f;

}

CSpell::~CSpell()
{

}

HRESULT CSpell::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = { m_vPos.x,m_fScale*0.5f,m_vPos.z };


	return S_OK;

}

_int CSpell::Update_GameObject(const _float & fTimeDelta)
{
	m_fTimer += fTimeDelta;
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

	if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
	{
		m_tFrame.fFrameCnt = 0.f;
		Set_Enable(false);
	}

	SetScale(m_fScale);
	m_pTransformCom->Get_WorldMatrix();

	_matrix			matBill, matWorld, matView;
	D3DXMatrixIdentity(&matBill);
	D3DXMatrixIdentity(&matWorld);

	matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_pTransformCom->m_matWorld = matBill * matWorld;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	return 0;
}

void CSpell::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//Set Alpha Test
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Render UI
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Render_Texture((_uint)m_tFrame.fFrameCnt);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


}

HRESULT CSpell::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CLightTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_LightTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, m_wstrTex.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);
	m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom->Get_TextureFrameSize();

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);


	return S_OK;
}

void CSpell::SetScale(_float fScale)
{
	m_pTransformCom->m_matWorld._11 *= fScale;
	m_pTransformCom->m_matWorld._22 *= fScale;
	m_pTransformCom->m_matWorld._33 *= fScale;

}

CSpell * CSpell::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, _vec3 vPos, _float fScale)
{
	CSpell*	pInstance = new CSpell(pGraphicDev, wstrTex, vPos, fScale);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;

}

void CSpell::Free(void)
{
	CGameObject::Free();
}
