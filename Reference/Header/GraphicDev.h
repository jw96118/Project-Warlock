#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev : public CBase
{
	DECLARE_SINGLETON(CGraphicDev)

private:
	explicit CGraphicDev(void);
	virtual	~CGraphicDev(void);

public:
	LPDIRECT3DDEVICE9		Get_Device(void);
	void					Get_Resolution(RESOLUTION& tResolution) const;

public:
	HRESULT					Ready_GraphicDev(HWND hWnd,
											 WINMODE eMode,
											 const _uint& iSizeX,
											 const _uint& iSizeY,
											 Engine::CGraphicDev** ppGraphicDev);

	void					Render_Begin(D3DXCOLOR Color);
	void					Render_End(void);

	void		Render_End(HWND hWnd);
private:
	LPDIRECT3D9				m_pSDK;
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	RESOLUTION				m_tResolution;

public:
	inline	virtual void	Free(void);
};

END

#endif // GraphicDev_h__
