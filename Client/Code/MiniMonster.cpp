#include "stdafx.h"
#include "MiniMonster.h"
#include "Export_Function.h"
#include "CollisionMgr.h"
#include "Player.h"
#include "MiniMonsterEvent.h"
#include <iostream>
using namespace std;
CMiniMonster::CMiniMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_fSpeed(4.f)
{
	ZeroMemory(&m_tBoxCollider, sizeof(m_tBoxCollider));
}

CMiniMonster::~CMiniMonster(void)
{

}

BOXCOLLIDER * CMiniMonster::Get_BoxCollider(void)
{
	return &m_tBoxCollider;
}



HRESULT CMiniMonster::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_ColSize = { 0.5f,0.5f,0.5f };
	m_tFrame.fFrameCnt = 0;
	m_tFrame.fMaxFrameCnt = 8;
	m_tFrame.fTimeScale = 1.f;

	return S_OK;
}

HRESULT CMiniMonster::LateReady_GameObject(void)
{


	return S_OK;
}

_int CMiniMonster::Update_GameObject(const _float& fTimeDelta)
{
	m_pPlayerObj = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"02.GameLogic", L"Obj_Player"));
	m_pEventUi = dynamic_cast<CMiniMonsterEvent*>(Engine::Get_GameObject(L"03.UI", L"Monster_Event"));
	/*m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>
		(Engine::Get_Component(L"02.GameLogic", L"Obj_Player", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pTargetTransformCom, -1);*/


	Engine::CGameObject::Update_GameObject(fTimeDelta);



	_vec3 tempPos = m_pPlayerObj->GetTransformCom()->m_vInfo[Engine::INFO_POS];
	m_pTransformCom->Chase_Target(&tempPos, (m_fSpeed * fTimeDelta));

	//cout << m_pTransformCom->m_vScale.x << " , " << m_pTransformCom->m_vScale.y << " , " << m_pTransformCom->m_vInfo[Engine::INFO_POS].z << endl;
	_matrix			matBill, matWorld, matView;
	D3DXMatrixIdentity(&matBill);

	matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_pTransformCom->m_matWorld = matBill * matWorld;


	Update_Collider();

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	if (CollisionMgr::IsCollisionSphereToSphere(m_tSphereCollider, m_pPlayerObj->Get_HitCollider()))
	{
		//m_pPlayerObj->GetTransformCom()->Set_Pos(_vec3(20.f, 1.f, 0.f));
		//m_pPlayerObj->Set_MiniDead(true);
		//m_pTransformCom->Set_Pos(_vec3(20.f, 1.f, 15.f));
		if (m_pEventUi->Get_Pos().y <= -50)
		{
			float  Speed = 4000.f;
			_vec3 vDir = { 0.f,Speed * fTimeDelta, 0.f };
			m_pEventUi->Move_Pos(vDir);
			m_pEventUi->SetPos();
			m_pPlayerObj->Set_MiniEvent(true);
			if (!m_bSoundAllStop)
			{
				Engine::Stop_All();

				m_bSoundAllStop = true;
			}
			Engine::Play_Sound(L"yeti_attack_2.wav", Engine::CHANNELID::MONSTER, temp2);

		}
		if (m_bSoundAllStop)
			m_fEventCount += fTimeDelta;
		if (m_fEventCount >= 1.5f)
		{
			m_pPlayerObj->GetTransformCom()->Set_Pos(_vec3(20.f, 1.f, 0.f));
			m_pPlayerObj->Set_MiniDead(true);
			m_pPlayerObj->Set_MiniEvent(false);
			m_pTransformCom->Set_Pos(_vec3(20.f, 1.f, 15.f));
			m_pEventUi->SetPos(_vec3(0.f, -WINCY * 1.4f, 0.07f));
			m_pEventUi->SetPos();
			m_bSoundAllStop = false;
			m_fEventCount = 0.f;
			temp = false;
			temp2 = false;
			Engine::Play_BGM(L"027 Antarctic - That Thing.wav");

		}
	}

	Animaition(fTimeDelta);


	Engine::Play_Sound(L"horor-b.mp3", Engine::CHANNELID::MONSTER, temp, true);
	_vec3 tempSoundPos = tempPos - m_pTransformCom->m_vInfo[Engine::INFO_POS];
	if (D3DXVec3Length(&tempSoundPos) <= 10.f)
	{
		float fVolume = 1.f - D3DXVec3Length(&tempSoundPos) * 0.1f;
		Engine::Set_Volume(Engine::CHANNELID::MONSTER, fVolume);
		Engine::Set_Volume(Engine::CHANNELID::BGM, D3DXVec3Length(&tempSoundPos) * 0.1f);
	}
	else
	{
		Engine::Set_Volume(Engine::CHANNELID::MONSTER, 0.f);
		Engine::Set_Volume(Engine::CHANNELID::BGM, 1.f);
	}
	return NO_EVENT;
}

void CMiniMonster::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//Set Alpha Test 
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Render UI
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Render_Texture(m_tFrame.fFrameCnt);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CMiniMonster::Update_Collider()
{
	_vec3 vPos = { 0.f, 0.f, 0.f };
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	m_tBoxCollider.vMax = { vPos.x + m_ColSize.x, vPos.y + m_ColSize.y, vPos.z + m_ColSize.z };
	m_tBoxCollider.vMin = { vPos.x - m_ColSize.x, vPos.y - m_ColSize.y, vPos.z - m_ColSize.z };

	m_tSphereCollider.fRadius = m_ColSize.x;
	m_tSphereCollider.vCenter = vPos;
}

void CMiniMonster::Set_Transform(_vec3 scale, _vec3 pos, _vec3 angle)
{
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = pos;
	m_pTransformCom->m_vScale = scale;
	m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(angle.x));
	m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(angle.y));
	m_pTransformCom->Rotation(Engine::ROT_Z, D3DXToRadian(angle.z));

}

void CMiniMonster::Set_ColSize(_vec3 colSize)
{
	m_ColSize = colSize;
}

void CMiniMonster::Set_Layer(Engine::CLayer * pColObject)
{
	m_ColObject = pColObject;
}

const float & CMiniMonster::Get_Speed() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_fSpeed;
}



HRESULT CMiniMonster::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	/* For Debug */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CLightTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_LightTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_MiniMonster"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

void CMiniMonster::Animaition(const _float & fTimeDelta)
{
	m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

	if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
	{
		m_tFrame.fFrameCnt = 0.f;
	}
}



CMiniMonster* CMiniMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniMonster*	pInstance = new CMiniMonster(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMiniMonster::Free(void)
{
	Engine::CGameObject::Free();
}