#include "stdafx.h"
#include "Weapon.h"
#include "WeaponMgr.h"
#include "Player.h"
#include "EffectMgr.h"

#include "Export_Function.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pWeaponMgr(nullptr)
	, m_pEffectMgr(nullptr)
	, m_vScale(0.f, 0.f, 0.f)
	, m_vPos(0.f, 0.f, 0.f)
	, m_vPivotPos(0.f, 0.f, 0.f)
	, m_vTextureOffset(0.f, 0.f, 0.f)
	, m_fDamage(0.f)
	, m_bIsMovePlayer(false)
	, m_iMoveOrder(MOVE_END)
	, m_fDegree(0.f)
	, m_fMoveSpeedX(0.f)
	, m_fMoveSpeedY(0.f)
	, m_eWeaponID(WEAPONID_END)
	, m_eWeaponType(WEAPONTYPE_END)
	, m_bIsChanging(false)
	, m_vChangingPos(0.f, 0.f, 0.f)
	, m_bIsIdle(true)
	, m_pShop(nullptr)
{
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
}

CWeapon::~CWeapon(void)
{
}

const WEAPONID & CWeapon::Get_WeaponID() const
{
	return m_eWeaponID;
}

const WEAPONTYPE & CWeapon::Get_WeaponType() const
{
	return m_eWeaponType;
}

const _bool & CWeapon::Get_IsIdle() const
{
	return m_bIsIdle;
}

const _float & CWeapon::Get_Damage() const
{
	return m_fDamage;
}

HRESULT CWeapon::Ready_GameObject(void)
{
	m_bEnable = false;

	m_iMoveOrder = MOVE_RIGHT;

	m_fMoveSpeedX = 180.f;
	m_fMoveSpeedY = 100.f;

	FAILED_CHECK_RETURN(CWeapon::Add_Component(), E_FAIL);

	m_pWeaponMgr = CWeaponMgr::GetInstance();
	NULL_CHECK_RETURN(m_pWeaponMgr, E_FAIL);

	m_pEffectMgr = CEffectMgr::GetInstance();
	NULL_CHECK_RETURN(m_pEffectMgr, E_FAIL);

	m_pWeaponMgr->AddRef();

	return S_OK;
}

HRESULT CWeapon::LateReady_GameObject(void)
{
	m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"02.GameLogic", L"Obj_Player"));
	m_pShop = dynamic_cast<CShop*>(Engine::Get_GameObject(L"02.GameLogic", L"Shop"));
	NULL_CHECK_RETURN(m_pShop, E_FAIL);

	return S_OK;
}

_int CWeapon::Update_GameObject(const _float& fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return NO_EVENT;
}

void CWeapon::Render_GameObject(void)
{
}

void CWeapon::OnEnable_GameObject(void)
{
	m_bIsChanging = true;
	m_vPos = m_vChangingPos;
}

HRESULT CWeapon::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

void CWeapon::Key_Input(const _float & fTimeDelta)
{
	m_bIsMovePlayer = false;

	// m_tFrame.uiAnimationID == Weapon Idle Animation
	if (/*m_tFrame.uiAnimationID == 0 &&*/
		(Engine::Get_DIKeyState(DIK_W) & 0x80 ||
			Engine::Get_DIKeyState(DIK_A) & 0x80 ||
			Engine::Get_DIKeyState(DIK_S) & 0x80 ||
			Engine::Get_DIKeyState(DIK_D) & 0x80))
	{
		m_bIsMovePlayer = true;
		m_fDegree += m_fMoveSpeedX * fTimeDelta;

		switch (m_iMoveOrder)
		{
			// ¿À¸¥ÂÊ
		case MOVE_RIGHT:
			if (m_fDegree >= 90.f)
			{
				m_fDegree = 90.f;
				m_fMoveSpeedX = -180.f;
				m_fMoveSpeedY = -100.f;
			}
			else if (m_fDegree <= 0.f)
			{
				m_vPos.y = m_vPivotPos.y;
				m_fDegree = 0.f;
				m_fMoveSpeedX = 180.f;
				m_fMoveSpeedY = 100.f;

				m_iMoveOrder = (m_iMoveOrder == MOVE_RIGHT) ? MOVE_LEFT : MOVE_RIGHT;
			}
			m_vPos.x += m_fMoveSpeedX * fTimeDelta;
			m_vPos.y += D3DXToRadian(m_fDegree) * m_fMoveSpeedY * fTimeDelta;
			break;
			// ¿ÞÂÊ
		case MOVE_LEFT:
			if (m_fDegree >= 90.f)
			{
				m_fDegree = 90.f;
				m_fMoveSpeedX = -180.f;
				m_fMoveSpeedY = -100.f;
			}
			else if (m_fDegree <= 0.f)
			{
				m_vPos.y = m_vPivotPos.y;
				m_fDegree = 0.f;
				m_fMoveSpeedX = 180.f;
				m_fMoveSpeedY = 100.f;

				m_iMoveOrder = (m_iMoveOrder == MOVE_LEFT) ? MOVE_RIGHT : MOVE_LEFT;
			}
			m_vPos.x -= m_fMoveSpeedX * fTimeDelta;
			m_vPos.y += D3DXToRadian(m_fDegree) * m_fMoveSpeedY * fTimeDelta;
			break;
		}
	}

	if (!m_bIsMovePlayer)
	{
		m_bIsMovePlayer = false;
		m_fDegree = 0.f;
		m_iMoveOrder = MOVE_RIGHT;
		m_fMoveSpeedX = 180.f;
		m_fMoveSpeedY = 100.f;
	}
}

void CWeapon::Mouse_Move(const _float & fTimeDelta)
{
	// 0 = Default Animation Idle
	if (m_bIsMovePlayer ||
		m_tFrame.uiAnimationID != 0)
		return;

	_long	dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))
		m_vPos.x -= dwMouseMove / 10.f;

	_vec3 vMoveDir = m_vPivotPos - m_vPos;
	m_vPos += vMoveDir * fTimeDelta * 3.f;
}

void CWeapon::Change_WeaponAction(const _float & fTimeDelta)
{
	// 0 = Default Animation Idle
	if (m_tFrame.uiAnimationID == 0)
	{
		_vec3 vMoveDir = m_vPivotPos - m_vPos;
		m_vPos += vMoveDir * fTimeDelta * 8.f;

		if (D3DXVec3Length(&(m_vPivotPos - m_vPos)) < 5.f)
		{
			m_vPos = m_vPivotPos;
			m_bIsChanging = false;
		}
	}
}

void CWeapon::Free(void)
{
	Engine::Safe_Release(m_pWeaponMgr);
	Engine::CGameObject::Free();
}