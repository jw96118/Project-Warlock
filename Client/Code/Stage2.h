#ifndef Stage2_h__
#define Stage2_h__

#include "Defines.h"
#include "Scene.h"

#include "SkyBox.h"
#include "Terrain.h"
#include "TempObject.h"
#include "Player.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"
#include "HUD.h"
#include "Gauge.h"
#include "Bat.h"
class CStage2 : public Engine::CScene
{
private:
	explicit CStage2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage2(void);

public:
	virtual HRESULT		Ready_Scene(void) override;
	virtual HRESULT		LateReady_Scene(void) override;
	virtual _int		Update_Scene(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void		Render_Scene(void) override;

private:
	HRESULT				Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT				Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT				Ready_UI_Layer(const _tchar* pLayerTag);
	HRESULT				Ready_Weapon_Layer(const _tchar* pLayerTag);
	HRESULT				Ready_Col_Layer(const _tchar* pLayerTag);
	HRESULT				Ready_Effect_Layer(const _tchar* pLayerTag);

	void                Load_Map(Engine::CLayer* pLayer, const _tchar* pLayerTag, const _tchar* pMonsterTag, const _tchar* pItemTag, const TCHAR * pFilePath,  bool bRoofFlag);
	void                Load_Col(Engine::CLayer* pLayer, const _tchar* pLayerTag, const TCHAR * pFilePath);
	void                Open();
private:
	_matrix				m_matView, m_matProj;
	_tchar				m_szScrollPos[128];

	// FPS Ȯ��
	_tchar				m_szFPS[256];
	_float				m_fTime;
	_ulong				m_dwRenderCnt;

	_int                m_MonsterCount = 0;
	_bool               m_bMiddleBossMove = false;
public:
	static CStage2*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void) override;
};


#endif // Stage_h__
