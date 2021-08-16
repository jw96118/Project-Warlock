#include "stdafx.h"
#include "Shop.h"
#include "Convert.h"
#include "Player.h"
#include "WeaponMgr.h"

#include "Export_Function.h"

CShop::CShop(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_bStaffOnSlot(false)
	, m_bCrossBowOnSlot(false)
	, m_eUpgrade(UPGRADE_END)
	, m_bUpgradePressed(false)
	, m_uiColdStaffPoint(0)
	, m_uiCrossBow3Point(0)
	, m_bIsUpgradeColdStaff(false)
	, m_bIsUpgradeCrossBow(false)
{
	ZeroMemory(&m_tszUpgradePoint, sizeof(m_tszUpgradePoint));
}

CShop::~CShop(void)
{

}

void CShop::Set_Enable(_bool bEnable)
{
	CGameObject::Set_Enable(bEnable);

	g_bCursorFix = !g_bCursorFix;
}

void CShop::ToString_UpgradePoint(_uint uiUpgradePoint)
{
	swprintf_s(m_tszUpgradePoint, L"%d", uiUpgradePoint);
}

HRESULT CShop::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vPivotPos = { WINCX * 0.5f, WINCY * 0.5f, 0.f };

	Engine::CConvert::ClientToD3DCoordinate2D(WINCX, WINCY, m_vPivotPos.x, m_vPivotPos.y);

	_float fWidth, fHeight, fScale;
	_vec3 vInterval = { 0.f, 0.f, 0.f };
	_vec3 vPos = { 0.f, 0.f, 0.f };;
	
	// Shop
	fScale = 2.f;
	fWidth = (_float)m_pTextureCom->Get_ImageInfo().Width;
	fHeight = (_float)m_pTextureCom->Get_ImageInfo().Height;
	D3DXMatrixScaling(&m_matShopView, fWidth * fScale, fHeight * fScale, 0.f);
	memcpy(&m_matShopView.m[3][0], &m_vPivotPos, sizeof(_vec3));

	// ColdStaff_HUD
	vInterval = { -350.f, 240.f, 0.f };
	fScale = 3.5f;
	vPos = m_vPivotPos + vInterval;
	fWidth = (_float)m_pColdStaffHudCom->Get_ImageInfo().Width;
	fHeight = (_float)m_pColdStaffHudCom->Get_ImageInfo().Height;
	D3DXMatrixScaling(&m_matColdStaffView, fWidth * fScale, fHeight * fScale, 0.f);
	memcpy(&m_matColdStaffView.m[3][0], &vPos, sizeof(_vec3));

	// CrossBow_HUD
	vInterval = { -20.f, 240.f, 0.f };
	fScale = 3.f;
	vPos = m_vPivotPos + vInterval;
	fWidth = (_float)m_pCrossBow3HudCom->Get_ImageInfo().Width;
	fHeight = (_float)m_pCrossBow3HudCom->Get_ImageInfo().Height;
	D3DXMatrixScaling(&m_matCrossBow3View, fWidth * fScale, fHeight * fScale, 0.f);
	memcpy(&m_matCrossBow3View.m[3][0], &vPos, sizeof(_vec3));

	// Staff Slot Selected
	vInterval = { -370.f, 225.f, 0.f };
	fScale = 2.f;
	vPos = m_vPivotPos + vInterval;
	fWidth = (_float)m_pSlotSelectedCom->Get_ImageInfo().Width;
	fHeight = (_float)m_pSlotSelectedCom->Get_ImageInfo().Height;
	D3DXMatrixScaling(&m_matSlotStaffView, fWidth * fScale, fHeight * fScale, 0.f);
	memcpy(&m_matSlotStaffView.m[3][0], &vPos, sizeof(_vec3));

	// CrossBow Slot Selected
	vInterval = { -30.f, 225.f, 0.f };
	fScale = 2.f;
	vPos = m_vPivotPos + vInterval;
	fWidth = (_float)m_pSlotSelectedCom->Get_ImageInfo().Width;
	fHeight = (_float)m_pSlotSelectedCom->Get_ImageInfo().Height;
	D3DXMatrixScaling(&m_matSlotCrossBowView, fWidth * fScale, fHeight * fScale, 0.f);
	memcpy(&m_matSlotCrossBowView.m[3][0], &vPos, sizeof(_vec3));


	// Upgrade Available
	vInterval = { 350.f, -155.f, 0.f };
	fScale = 2.f;
	vPos = m_vPivotPos + vInterval;
	fWidth = (_float)m_pUpgradeAvailableCom->Get_ImageInfo().Width;
	fHeight = (_float)m_pUpgradeAvailableCom->Get_ImageInfo().Height;
	D3DXMatrixScaling(&m_matUpgradeAvailableView, fWidth * fScale, fHeight * fScale, 0.f);
	memcpy(&m_matUpgradeAvailableView.m[3][0], &vPos, sizeof(_vec3));

	// Upgrade Available
	vInterval = { 350.f, -155.f, 0.f };
	fScale = 2.f;
	vPos = m_vPivotPos + vInterval;
	fWidth = (_float)m_pUpgradePressedCom->Get_ImageInfo().Width;
	fHeight = (_float)m_pUpgradePressedCom->Get_ImageInfo().Height;
	D3DXMatrixScaling(&m_matUpgradePressedView, fWidth * fScale, fHeight * fScale, 0.f);
	memcpy(&m_matUpgradePressedView.m[3][0], &vPos, sizeof(_vec3));

	m_uiColdStaffPoint = 1;
	m_uiCrossBow3Point = 1;

	return S_OK;
}

_int CShop::Update_GameObject(const _float & fTimeDelta)
{
	POINT ptMouse = {};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_bStaffOnSlot = false;
	m_bCrossBowOnSlot = false;
	m_bUpgradePressed = false;

	if (ptMouse.x >= 425.f && ptMouse.x <= 755.f &&
		ptMouse.y >= 245.f && ptMouse.y <= 350.f)
	{
		m_bStaffOnSlot = true;

		_bool bKeyDown = false;

		if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_LBUTTON))
			bKeyDown = true;

		if (bKeyDown &&
			m_eUpgrade != UPGRADE_COLDSTAFF)
		{
			m_eUpgrade = UPGRADE_COLDSTAFF;
		}
		else if (bKeyDown &&
				 m_eUpgrade == UPGRADE_COLDSTAFF)
		{
			m_eUpgrade = UPGRADE_END;
		}
	}

	else if (ptMouse.x >= 765.f && ptMouse.x <= 1095.f &&
			 ptMouse.y >= 245.f && ptMouse.y <= 350.f)
	{
		m_bCrossBowOnSlot = true;

		_bool bKeyDown = false;

		if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_LBUTTON))
			bKeyDown = true;

		if (bKeyDown &&
			m_eUpgrade != UPGRADE_CROSSBOW3)
		{
			m_eUpgrade = UPGRADE_CROSSBOW3;
		}
		else if (bKeyDown &&
				 m_eUpgrade == UPGRADE_CROSSBOW3)
		{
			m_eUpgrade = UPGRADE_END;
		}
	}

	else if (ptMouse.x >= 1130.f && ptMouse.x <= 1490.f &&
			 ptMouse.y >= 655.f && ptMouse.y <= 735.f)
	{
		if (Engine::Get_KeyMgr()->KeyPressing(Engine::KEY_LBUTTON))
		{
			m_bUpgradePressed = true;
		}
		if (Engine::Get_KeyMgr()->KeyUp(Engine::KEY_LBUTTON))
		{
			Upgrade_Weapon();
		}
	}


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return NO_EVENT;
}

void CShop::Render_GameObject(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matShopView);
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	if (m_bStaffOnSlot || m_eUpgrade == UPGRADE_COLDSTAFF)
	{
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matSlotStaffView);
		m_pSlotSelectedCom->Render_Texture();
		m_pBufferCom->Render_Buffer();
	}

	if (m_bCrossBowOnSlot || m_eUpgrade == UPGRADE_CROSSBOW3)
	{
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matSlotCrossBowView);
		m_pSlotSelectedCom->Render_Texture();
		m_pBufferCom->Render_Buffer();
	}

	if (m_bIsUpgradeColdStaff)
	{
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matSlotStaffView);
		m_pSlotUpgradeCom->Render_Texture();
		m_pBufferCom->Render_Buffer();
	}

	if (m_bIsUpgradeCrossBow)
	{
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matSlotCrossBowView);
		m_pSlotUpgradeCom->Render_Texture();
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matColdStaffView);
	m_pColdStaffHudCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matCrossBow3View);
	m_pCrossBow3HudCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	if (m_eUpgrade != UPGRADE_END)
	{
		switch (m_eUpgrade)
		{
		case UPGRADE_COLDSTAFF:
			if (!m_bIsUpgradeColdStaff)
			{
				m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matUpgradeAvailableView);
				m_pUpgradeAvailableCom->Render_Texture();
				m_pBufferCom->Render_Buffer();
			}
			break;
		case UPGRADE_CROSSBOW3:
			if (!m_bIsUpgradeCrossBow)
			{
				m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matUpgradeAvailableView);
				m_pUpgradeAvailableCom->Render_Texture();
				m_pBufferCom->Render_Buffer();
			}
			break;
		}
	}

	if (m_bUpgradePressed)
	{
		switch (m_eUpgrade)
		{
		case UPGRADE_COLDSTAFF:
			if (!m_bIsUpgradeColdStaff)
			{
				m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matUpgradePressedView);
				m_pUpgradePressedCom->Render_Texture();
				m_pBufferCom->Render_Buffer();
			}
			break;
		case UPGRADE_CROSSBOW3:
			if (!m_bIsUpgradeCrossBow)
			{
				m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matUpgradePressedView);
				m_pUpgradePressedCom->Render_Texture();
				m_pBufferCom->Render_Buffer();
			}
			break;
		}

		//m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matUpgradePressedView);
		//m_pUpgradePressedCom->Render_Texture();
		//m_pBufferCom->Render_Buffer();
	}


	Engine::Render_Font(L"Font_Third", m_tszUpgradePoint, &_vec2(1380.f, 285.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	Engine::Render_Font(L"Font_Third", L"7", &_vec2(715.f, 360.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Engine::Render_Font(L"Font_Third", L"3", &_vec2(1055.f, 360.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CShop::OnEnable_GameObject(void)
{
	m_bStaffOnSlot = false;
	m_bCrossBowOnSlot = false;
	m_bUpgradePressed = false;

	m_eUpgrade = UPGRADE_END;
}


HRESULT CShop::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Shop"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Shop", pComponent);

	pComponent = m_pSlotSelectedCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Shop_Slot_Selected"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Shop_Slot_Selected", pComponent);

	pComponent = m_pUpgradeAvailableCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Shop_UpgradeButton_Available"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Shop_UpgradeButton_Available", pComponent);

	pComponent = m_pUpgradePressedCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Shop_UpgradeButton_Pressed"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Shop_UpgradeButton_Pressed", pComponent);

	pComponent = m_pColdStaffHudCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Shop_ColdStaff_HUD"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Shop_ColdStaff_HUD", pComponent);

	pComponent = m_pCrossBow3HudCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Shop_CrossBow3_HUD"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Shop_CrossBow3_HUD", pComponent);

	pComponent = m_pSlotUpgradeCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Shop_Slot_Upgrade"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Shop_Slot_Upgrade", pComponent);

	return S_OK;
}

void CShop::Upgrade_Weapon(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"02.GameLogic", L"Obj_Player"));
	NULL_CHECK(pPlayer);

	_uint uiUpgradePoint = *pPlayer->Get_UpgradePoint();

	switch (m_eUpgrade)
	{
	case UPGRADE_COLDSTAFF:
		if (m_uiColdStaffPoint <= uiUpgradePoint &&
			!m_bIsUpgradeColdStaff)
		{
			pPlayer->Set_UpgradePoint(uiUpgradePoint - m_uiColdStaffPoint);

			swprintf_s(m_tszUpgradePoint, L"%d", uiUpgradePoint - m_uiColdStaffPoint);

			m_bIsUpgradeColdStaff = true;

			m_eUpgrade = UPGRADE_END;

			CWeaponMgr::GetInstance()->Set_UnLock(WEAPONTYPE_STAFF, WEAPONID_02, true);
		}
		break;
	case UPGRADE_CROSSBOW3:
		if (m_uiCrossBow3Point <= uiUpgradePoint &&
			!m_bIsUpgradeCrossBow)
		{
			uiUpgradePoint - m_uiCrossBow3Point;
			pPlayer->Set_UpgradePoint(uiUpgradePoint - m_uiCrossBow3Point);

			swprintf_s(m_tszUpgradePoint, L"%d", uiUpgradePoint - m_uiCrossBow3Point);

			m_bIsUpgradeCrossBow = true;

			m_eUpgrade = UPGRADE_END;


			CWeaponMgr::GetInstance()->Set_UnLock(WEAPONTYPE_CROSSBOW, WEAPONID_02, true);
		}
		break;
	}
}

CShop * CShop::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShop*	pInstance = new CShop(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CShop::Free(void)
{
	Engine::CGameObject::Free();
}
