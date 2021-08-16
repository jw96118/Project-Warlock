#include "stdafx.h"
#include "Player.h"
#include "Weapon.h"
#include "WeaponMgr.h"
#include "Export_Function.h"
#include "CollisionMgr.h"
#include "Gauge.h"
#include "Shop.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pWeaponMgr(nullptr)
	, m_uiUpgradePoint(0)
	, m_pShop(nullptr)
{
	ZeroMemory(&m_tBoxCollider, sizeof(m_tBoxCollider));
	ZeroMemory(&m_tData, sizeof(m_tData));
}

CPlayer::~CPlayer(void)
{

}

BOXCOLLIDER * CPlayer::Get_BoxCollider(void)
{
	return &m_tBoxCollider;
}

SPHERECOLLIDER CPlayer::Get_HitCollider(void)
{
	return m_tHitCollider;
}

const OBJ_DATA & CPlayer::Get_Data(void)
{
	return m_tData;
}

const bool & CPlayer::Get_MiniDead(void) const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_bMiniDead;
}

const bool & CPlayer::Get_MiniEvnet(void) const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_bMiniEvent;
}

const bool & CPlayer::Get_MiniGame(void) const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_bMiniGame;
}

void CPlayer::Set_Ammo(_float fCurAmmo, _float fMaxAmmo)
{
	m_fCurAmmo = fCurAmmo;
	m_fMaxAmmo = fMaxAmmo;
}

void CPlayer::Set_MP(_float fCurMP, _float fMaxMP)
{
	m_fCurMP = fCurMP;
	m_fMaxMP = fMaxMP;
}

void CPlayer::Set_UpgradePoint(_uint uiUpgradePoint)
{
	m_uiUpgradePoint = uiUpgradePoint;
}

void CPlayer::Set_MiniDead(bool flag)
{
	m_bMiniDead = flag;
}

void CPlayer::Set_MiniEvent(bool flag)
{
	m_bMiniEvent = flag;
}

void CPlayer::Add_UpgradePoint()
{
	m_uiUpgradePoint += 1;

	CShop* pShop = dynamic_cast<CShop*>(Engine::Get_GameObject(L"02.GameLogic", L"Shop"));
	pShop->ToString_UpgradePoint(m_uiUpgradePoint);

}

void CPlayer::Add_CurHP(_float fHP)
{
	m_tData.fCurHP += fHP;
	if (m_tData.fCurHP > m_tData.fMaxHP)
		m_tData.fCurHP = m_tData.fMaxHP;
}

void CPlayer::Add_CurMP(_float fMP)
{
	m_fCurMP += fMP;
	if (m_fCurMP > m_fMaxMP)
		m_fCurMP = m_fMaxMP;
}

void CPlayer::Add_CurAmmo(_float fMP)
{
	m_fCurAmmo += fMP;
	if (m_fCurAmmo> m_fMaxMP)
		m_fMaxMP = m_fCurAmmo;
}

void CPlayer::BeAttack(_float fDamage)
{
	m_tData.fCurHP -= fDamage;
	if (m_tData.fCurHP <= 0.f)
		m_tData.fCurHP = 0.f;
}

OBJ_DATA * CPlayer::Get_DataInfo(void)
{
	return &m_tData;
}

_uint * CPlayer::Get_UpgradePoint(void)
{
	return &m_uiUpgradePoint;
}

HRESULT CPlayer::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pWeaponMgr = CWeaponMgr::GetInstance();
	m_pWeaponMgr->AddRef();

	m_tHitCollider.fRadius = 0.5f;

	//m_tData.uiLev = 1;
	//m_tData.fMaxHP = 150.f;
	//m_tData.fCurHP = m_tData.fMaxHP;
	//m_tData.fDamage = 15.f;
	

	m_fMaxMP = m_pWeaponMgr->Get_AmmoMaxCnt(AMMO_SPELL);
	m_fCurMP = m_pWeaponMgr->Get_AmmoCurCnt(AMMO_SPELL);

	//m_tData.fMaxEXE = 100.f;
	//m_tData.fCurEXE = 0.f;
	m_fMaxAmmo = 0.f;
	m_fCurAmmo = m_fMaxAmmo;

	return S_OK;
}

HRESULT CPlayer::LateReady_GameObject(void)
{
	//// 메인 카메라 
	//CFirstPersonCamera* pCamera = dynamic_cast<CFirstPersonCamera*>(Engine::Get_GameObject(L"01.Environment", L"MainCamera"));
	//NULL_CHECK_RETURN(pCamera, E_FAIL);

	//// 플레이어 위치
	//_vec3 vPos = pCamera->Get_Eye();
	//m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	
	m_pGauge[HP] = dynamic_cast<CGauge*>(Engine::Get_GameObject(L"03.UI", L"HP_Bar"));
	m_pGauge[MP] = dynamic_cast<CGauge*>(Engine::Get_GameObject(L"03.UI", L"MP_Bar"));
	m_pGauge[EXE] = dynamic_cast<CGauge*>(Engine::Get_GameObject(L"03.UI", L"EXE_Bar"));
	m_pGauge[AMMO] = dynamic_cast<CGauge*>(Engine::Get_GameObject(L"03.UI", L"Ammo_Bar"));
	if (!m_bMiniGame)
	{
		m_pShop = dynamic_cast<CShop*>(Engine::Get_GameObject(L"02.GameLogic", L"Shop"));
		NULL_CHECK_RETURN(m_pShop, E_FAIL);

		m_pShop->ToString_UpgradePoint(m_uiUpgradePoint);
	}
	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
//	m_pWeaponMgr->Key_Input();
	
	//TODO: 이벤트시 값수정하게 하면 연동가능!!
	//if (GetAsyncKeyState('1') & 0x8000)
	//{
	//	m_tData.fCurHP -= 1.f;
	//}
	//if (GetAsyncKeyState('2') & 0x8000)
	//{
	//	m_fCurMP -= 1.f;
	//}
	//if (GetAsyncKeyState('3') & 0x8000)
	//{
	//	m_fCurAmmo -= 0.01f;
	//}
	//if (GetAsyncKeyState('4') & 0x8000)
	//{
	//	m_tData.fCurEXE += 1.f;
	//}

	if (!m_bMiniGame)
	{
		if(!m_pShop->Get_Enable())
			m_pWeaponMgr->Key_Input();

		Update_Gauge();

		if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_I))
		{
			CGameObject* pObject = Engine::Get_GameObject(L"02.GameLogic", L"Shop");
			_bool bEnable = pObject->Get_Enable();

			bEnable = (bEnable == true) ? false : true;

			pObject->Set_Enable(bEnable);
		}

	}
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	Update_Collider();
		
	/* For Debug */
	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);


	return NO_EVENT;
}

void CPlayer::Render_GameObject(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//SPHERECOLLIDER tCollider;
	//tCollider.fRadius = 0.5f;
	//tCollider.vCenter = { 2.f, 0.5f, 0.f };

	//_float f;
	//CollisionMgr::IsCollisionRayToSphere(_vec3(), f, m_pGraphicDev, tCollider);

	//_matrix matWorld, matScale, matTrans;

	//D3DXMatrixScaling(&matScale, tCollider.fRadius, tCollider.fRadius, tCollider.fRadius);
	//D3DXMatrixTranslation(&matTrans, tCollider.vCenter.x, tCollider.vCenter.y, tCollider.vCenter.z);

	//matWorld = matScale * matTrans;
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	//Engine::Render_SphereMesh();
	//m_pTextureCom->Render_Texture();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPlayer::Update_Collider(void)
{

	_vec3 vPos = { 0.f, 0.f, 0.f };
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	// Box Collider
	_float fIntervalX = 0.5f;
	_float fIntervalY = 0.5f;
	_float fIntervalZ = 0.5f;

	m_tBoxCollider.vMax = { vPos.x + fIntervalX, vPos.y + fIntervalY, vPos.z + fIntervalZ };
	m_tBoxCollider.vMin = { vPos.x - fIntervalX, vPos.y - fIntervalY, vPos.z - fIntervalZ };

	// Hit Collider
	m_tHitCollider.vCenter = vPos;
	m_tHitCollider.vCenter.y -= 0.25f;
}

void CPlayer::Update_Gauge(void)
{
	m_pGauge[HP]->Set_Item(HP, m_tData.fCurHP / m_tData.fMaxHP);
	m_pGauge[MP]->Set_Item(MP, m_fCurMP / m_fMaxMP);
	m_pGauge[AMMO]->Set_Item(AMMO, m_fCurAmmo / m_fMaxAmmo);
    m_pGauge[EXE]->Set_Item(EXE, m_tData.fCurEXE / m_tData.fMaxEXE);
}

HRESULT CPlayer::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	/* For Debug */
	pComponent = m_pBufferCom = dynamic_cast<Engine::CLightTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_LightTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Player"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

void CPlayer::Level_UP(void)
{
	m_tData.fMaxEXE += 25.f;
	m_tData.fCurEXE = 0.f;
	m_tData.fMaxHP += 30.f;
	m_tData.fCurHP = m_tData.fMaxHP;
	m_tData.fDamage += 5.f;
	++m_tData.uiLev;;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, bool bMiniGame)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);
	pInstance->m_bMiniGame = bMiniGame;
	return pInstance;
}

void CPlayer::Free(void)
{
	Engine::Safe_Release(m_pWeaponMgr);

	Engine::CGameObject::Free();
}