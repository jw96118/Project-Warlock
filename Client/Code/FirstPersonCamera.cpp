#include "stdafx.h"
#include "FirstPersonCamera.h"
#include "WeaponMgr.h"
#include "Weapon.h"
#include "Player.h"
#include "Export_Function.h"
#include <iostream>
CFirstPersonCamera::CFirstPersonCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
	, m_vMoveDir(0.f, 0.f, 0.f)
	, m_fRotationX(0.f)
	, m_fRotationY(0.f)
	, m_pShop(nullptr)
{
	m_fLightVal = 0.f;
	m_fToggleVal = 0.f;
}

CFirstPersonCamera::~CFirstPersonCamera(void)
{
}

const _vec3 & CFirstPersonCamera::Get_Eye() const
{
	return m_vEye;
}

const _vec3 & CFirstPersonCamera::Get_At() const
{
	return m_vAt;
}

const _vec3 & CFirstPersonCamera::Get_MoveDir() const
{
	return m_vMoveDir;
}

const _vec3 & CFirstPersonCamera::Get_LookDir() const
{

	return m_vLookDir;
}

const _float & CFirstPersonCamera::Get_RotationX() const
{
	return m_fRotationX;
}

const _float & CFirstPersonCamera::Get_RotationY() const
{
	return m_fRotationY;
}

HRESULT CFirstPersonCamera::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar, const _bool& bMainCamera)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	m_bMainCamera = bMainCamera;

	m_vMoveDir = m_vAt - m_vEye;
	D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);

	m_fSpeed = 7.f;

	FAILED_CHECK_RETURN(Engine::CCamera::Ready_GameObject(), E_FAIL);

	ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));

	return S_OK;
}

HRESULT CFirstPersonCamera::LateReady_GameObject()
{
	m_pPlayerObj = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"02.GameLogic", L"Obj_Player"));
	if (!m_pPlayerObj->Get_MiniGame())
	{
		m_pShop = dynamic_cast<CShop*>(Engine::Get_GameObject(L"02.GameLogic", L"Shop"));
		NULL_CHECK_RETURN(m_pShop, E_FAIL);
	}
	//m_pPlayerTransformCom->Set_Pos(m_vEye);

	m_vEye = m_pPlayerObj->GetTransformCom()->m_vInfo[Engine::INFO_POS];
	m_vAt.y = m_vEye.y;
	m_vMoveDir = m_vAt - m_vEye;
	m_vLookDir = m_vMoveDir;
	return S_OK;
}

_int CFirstPersonCamera::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_pPlayerObj->Get_MiniEvnet())
	{
		if (!m_pPlayerObj->Get_MiniGame())
		{
			if (!m_pShop->Get_Enable())
			{
				Key_Input(fTimeDelta);
				Mouse_Move(fTimeDelta);
			}
		}
		else
		{
			Key_Input(fTimeDelta);
			Mouse_Move(fTimeDelta);
		}
	}

	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);

	if (m_pPlayerObj->Get_MiniDead())
	{
		m_vEye = m_pPlayerObj->GetTransformCom()->m_vInfo[Engine::INFO_POS];
		m_vAt.y = m_vEye.y;
		m_vMoveDir = m_vAt - m_vEye;
		m_vLookDir = m_vMoveDir;
		m_pPlayerObj->Set_MiniDead(false);
	}
	if (!m_pPlayerObj->Get_MiniGame())
	{
		if (m_pShop->Get_Enable())
			return NO_EVENT;
	}
	m_tLightInfo.Type = D3DLIGHT_POINT;
	if (Engine::Get_KeyMgr()->KeyPressing(Engine::KEY_LBUTTON))
	{
		if (CWeaponMgr::GetInstance()->Get_CurWeapon() != nullptr)
		{
			switch (CWeaponMgr::GetInstance()->Get_CurWeapon()->Get_WeaponType())
			{
			case WEAPONTYPE_DEFAULT:
				if (m_fLightVal <= 50.f)
					m_fLightVal += fTimeDelta * 85.f;

				m_tLightInfo.Diffuse = D3DXCOLOR(1.f - sinf(D3DXToRadian(m_fLightVal)), 1.f - sinf(D3DXToRadian(m_fLightVal)), 0.4f + (sinf(D3DXToRadian(m_fLightVal)*0.2f)), 1.f);
				m_tLightInfo.Range = 1075.f;
				m_tLightInfo.Attenuation0 = 0.001f;
				m_tLightInfo.Attenuation1 = 0.02f;
				m_tLightInfo.Attenuation2 = 0.01f;

				break;
			case WEAPONTYPE_STAFF:
				if (m_fLightVal <= 50.f)
					m_fLightVal += fTimeDelta * 50;
				else
					m_fToggleVal += fTimeDelta * 10.f;

				m_tLightInfo.Diffuse = D3DXCOLOR(1.f - sinf(D3DXToRadian(m_fLightVal)), 1.f - sinf(D3DXToRadian(m_fLightVal)), 0.4f + (sinf(D3DXToRadian(m_fLightVal)*0.2f)), 1.f);
				m_tLightInfo.Attenuation0 = 0.001f*100.f*cosf(D3DXToRadian(m_fToggleVal * 50));
				m_tLightInfo.Range = 1075.f;
				m_tLightInfo.Attenuation1 = 0.02f;
				m_tLightInfo.Attenuation2 = 0.01f;
				break;
			case WEAPONTYPE_CROSSBOW:
				m_tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				m_tLightInfo.Range = 75.f;
				m_tLightInfo.Attenuation0 = 0.02f;
				m_tLightInfo.Attenuation1 = 0.06f;
				m_tLightInfo.Attenuation2 = 0.02f;
				break;
			case WEAPONTYPE_GUN:
				m_fLightVal += fTimeDelta;
				m_tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.f, 1.f);
				m_tLightInfo.Range = 1075.f;
				m_tLightInfo.Attenuation0 = 0.001f*100.f*cosf(m_fLightVal*50.f);
				m_tLightInfo.Attenuation1 = 0.02f;
				m_tLightInfo.Attenuation2 = 0.01f;
				break;
			default:
				m_tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				m_tLightInfo.Range = 1075.f;
				m_tLightInfo.Attenuation0 = 0.001f;
				m_tLightInfo.Attenuation1 = 0.02f;
				m_tLightInfo.Attenuation2 = 0.01f;
				break;
			}
		}
	}
	else
	{
		m_fToggleVal = 0.f;
		m_fLightVal = 0.f;
		m_tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_tLightInfo.Range = 75.f;
		m_tLightInfo.Attenuation0 = 0.02f;
		m_tLightInfo.Attenuation1 = 0.06f;
		m_tLightInfo.Attenuation2 = 0.02f;
	}
	m_tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tLightInfo.Position = m_vEye - *D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir) * 2;


	m_tLightInfo.Direction = m_vMoveDir;

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &m_tLightInfo, 0), E_FAIL);


	return NO_EVENT;
}

void CFirstPersonCamera::Move_View(const _vec3 & vMove)
{
	m_vEye += vMove;
	m_vAt += vMove;

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void CFirstPersonCamera::Key_Input(const _float & fTimeDelta)
{
	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3 vMoveDist = m_vMoveDir * m_fSpeed * fTimeDelta;

		m_vEye += vMoveDist;
		m_vAt += vMoveDist;

		m_pPlayerObj->GetTransformCom()->Move_Pos(&vMoveDist);
	}
	else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3 vMoveDist = m_vMoveDir * m_fSpeed * fTimeDelta;

		m_vEye -= vMoveDist;
		m_vAt -= vMoveDist;

		m_pPlayerObj->GetTransformCom()->Move_Pos(&(-vMoveDist));
	}
	else if (Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3 vUp, vRight;

		memcpy(&vUp, &matCamWorld.m[1][0], sizeof(_vec3));

		D3DXVec3Cross(&vRight, &vUp, &m_vMoveDir);
		D3DXVec3Normalize(&vRight, &vRight);

		_vec3 vMoveDist = vRight * m_fSpeed * fTimeDelta;

		m_vEye -= vMoveDist;
		m_vAt -= vMoveDist;

		m_pPlayerObj->GetTransformCom()->Move_Pos(&(-vMoveDist));
	}
	else if (Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3 vUp, vRight;

		memcpy(&vUp, &matCamWorld.m[1][0], sizeof(_vec3));

		D3DXVec3Cross(&vRight, &vUp, &m_vMoveDir);
		D3DXVec3Normalize(&vRight, &vRight);

		_vec3 vMoveDist = vRight * m_fSpeed * fTimeDelta;

		m_vEye += vMoveDist;
		m_vAt += vMoveDist;

		m_pPlayerObj->GetTransformCom()->Move_Pos(&vMoveDist);
	}
}

void CFirstPersonCamera::Mouse_Move(const _float & fTimeDelta)
{
	_long	dwMouseMove = 0;

	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y))
	{
		_float fDegree = dwMouseMove / 15.f;
		_float fRotationX = m_fRotationX + fDegree;

		if (fRotationX > 45.f || fRotationX < -45.f)
			return;

		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLook = m_vAt - m_vEye;

		_matrix		matRotAxis;
		D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(fDegree));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRotAxis);
		D3DXVec3TransformNormal(&m_vLookDir, &m_vLookDir, &matRotAxis);
		D3DXVec3Normalize(&m_vLookDir, &m_vLookDir);

		m_vAt = m_vEye + vLook;

		m_fRotationX = fRotationX;
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))
	{
		_float	fDegree = dwMouseMove / 15.f;
		m_fRotationY += fDegree;

		_vec3	vUp = _vec3(0.f, 1.f, 0.f);

		_vec3	vLook = m_vAt - m_vEye;

		_matrix		matRotAxis;
		D3DXMatrixRotationAxis(&matRotAxis, &vUp, D3DXToRadian(fDegree));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRotAxis);

		m_vAt = m_vEye + vLook;

		D3DXVec3TransformNormal(&m_vMoveDir, &m_vMoveDir, &matRotAxis);
		D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);
		D3DXVec3TransformNormal(&m_vLookDir, &m_vLookDir, &matRotAxis);
		D3DXVec3Normalize(&m_vLookDir, &m_vLookDir);
	}
}

CFirstPersonCamera* CFirstPersonCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
	const _float& fFovY, const _float& fAspect,
	const _float& fNear, const _float& fFar,
	const _bool& bMainCamera)
{
	CFirstPersonCamera*	pInstance = new CFirstPersonCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar, bMainCamera)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFirstPersonCamera::Free(void)
{
	Engine::CCamera::Free();
}

