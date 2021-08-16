#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "Transform.h"
#include "Renderer.h"
#include "Camera.h"
#include "Pipeline.h"
#include "Calculator.h"
#include "LightMgr.h"

BEGIN(Engine)

// Management Instance
// Get
inline 	CComponent*			Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline 	CGameObject*		Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
inline 	CLayer*				Get_Layer(const _tchar* pLayerTag);

// Set
inline HRESULT			SetUp_Scene(CScene* pScene);
// General
inline HRESULT		Create_Management(CManagement** ppManagement);


// Renderer
inline CRenderer*		Get_Renderer(void);

// LightMgr
// Get
// Set
// General
inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex);

// Utility Release
inline void			Release_Utility(void);


#include "Export_Utility.inl"

END
#endif // Export_Utility_h__
