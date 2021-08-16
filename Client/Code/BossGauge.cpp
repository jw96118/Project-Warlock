#include "stdafx.h"
#include "BossGauge.h"
#include "Export_Function.h"
#include "DynamicCamera.h"
#include "Camera.h"

CBossGauge::CBossGauge(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, Engine::CTransform* pTransform, _float fScale,_uint uiBossType)
	:Engine::CGameObject(pGraphicDev),
	m_pTargetTransform(pTransform),
	m_fScale(fScale),
	m_wstrTexture(wstrTex),
	m_uiBossType(uiBossType)
{
	m_fScale = 0.005f;
}

CBossGauge::~CBossGauge()
{
}

HRESULT CBossGauge::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CBossGauge::Update_GameObject(const _float & fTimeDelta)
{
	/*	m_pTransformCom->m_matWorld = m_pTargetTransform->m_matWorld;
	SetScale(m_fScale);
	m_pTransformCom->m_matWorld._42 = 4.0f;// 스케일에따른 높이보정
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);*/

	_vec3 vTargetPos= *m_pTargetTransform->Get_Info(Engine::INFO_POS);

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	_matrix			matBill, matWorld, matView;
	D3DXMatrixIdentity(&matBill);

	SetScale(m_fScale);
	matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_pTransformCom->m_matWorld = matBill * matWorld;
	SetPos(vTargetPos);
	if(m_uiBossType)
		m_pTransformCom->m_matWorld._42 = 3.0f;// 스케일에따른 높이보정
	else
		m_pTransformCom->m_matWorld._42 = 5.0f;// 스케일에따른 높이보정

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return NO_EVENT;

}
void CBossGauge::SetScale(_float fScale)
{
	
	m_pTransformCom->m_matWorld._11 *= m_pTextureCom->Get_ImageInfo().Width* fScale;
	m_pTransformCom->m_matWorld._22 *= m_pTextureCom->Get_ImageInfo().Height*fScale;
	m_pTransformCom->m_matWorld._33 *= m_pTextureCom->Get_ImageInfo().Width*fScale;

}
void CBossGauge::SetPos(_vec3 vPos)
{
	m_pTransformCom->m_matWorld._41 = vPos.x;
	m_pTransformCom->m_matWorld._42 = vPos.y;
	m_pTransformCom->m_matWorld._43 = vPos.z;
}
void CBossGauge::Set_HPRatio(_float fHPRatio)
{
	m_fHPRatio = fHPRatio;
	if (m_fHPRatio <= 0.f)
		m_fHPRatio = 0.f;
}
void CBossGauge::Render_GameObject(void)
{
	//m_pTransformCom->m_matWorld._42 = 0.6f;//Y 높이 보정
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//Set Alpha Test
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Render UI
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Render_Texture();
	m_pBufferCom->SendToClient(0.0f, m_fHPRatio, 0.0f, 1.0f);

	m_pBufferCom->Render_Buffer();
	m_pBufferCom->SendToClient(0.0f, 1.0f, 0.0f, 1.0f);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBossGauge::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

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


CBossGauge * CBossGauge::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, Engine::CTransform* pTransform, _float fScale,  _uint uiBossType)
{
	CBossGauge*	pInstance = new CBossGauge(pGraphicDev, wstrTex, pTransform, fScale, uiBossType);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBossGauge::Free(void)
{
	Engine::CGameObject::Free();

}
