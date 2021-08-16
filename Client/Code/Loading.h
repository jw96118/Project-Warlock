#ifndef Loading_h__
#define Loading_h__

#include "Defines.h"
#include "Base.h"

class CLoading : public CBase 
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading(void);

public:
	LOADINGID			Get_LoadingID(void) const;
	CRITICAL_SECTION*	Get_Crt(void);
	_bool				Get_Finish(void) const;
	const _tchar*		Get_LoadString(void);

public:
	static _uint		CALLBACK Thread_Main(void* pArg);

public:
	HRESULT				Ready_Loading(LOADINGID eLoading);
	_uint				Loading_ForStage(void);

private:
	HANDLE				m_hThread;

	CRITICAL_SECTION	m_Crt;
	LOADINGID			m_eLoading;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_bool				m_bFinish;
	_tchar				m_szLoading[256];

	_float *             m_pLoadCount = nullptr;
public:
	static CLoading*	Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading, float* m_pLoadCount);
private:
	virtual void		Free(void);

};


#endif // Loading_h__
