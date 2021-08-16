#ifndef MainApp_h__
#define MainApp_h__

#include "Defines.h"
#include "Base.h"
#include "Engine_Defines.h"
#include "Export_Function.h"

BEGIN(Engine)

class CGraphicDev;
class CManagement;

END
class CMainApp : public CBase
{
private:	// ������, �Ҹ���
	explicit CMainApp(void);
	virtual ~CMainApp(void);

	// public �Լ�, ����
public:
	HRESULT				Ready_MainApp(void);
	_int				Update_MainApp(const _float& fTimeDelta);
	_int				LateUpdate_MainApp(const _float& fTimeDelta);
	void				Render_MainApp(void);

	// protected �Լ�, ����
protected:

	// private �Լ�, ����
private:
	HRESULT				SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);

	HRESULT				Ready_Scene(LPDIRECT3DDEVICE9& pGraphicDev, 
									Engine::CManagement** ppManagement);

private:
	Engine::CGraphicDev*		m_pDeviceClass;
	Engine::CManagement*		m_pManagement;
	LPDIRECT3DDEVICE9			m_pGraphicDev;

	_int						m_iEvent;

	_bool						m_bCursor;
public:		// ���� �Լ�
	static CMainApp*	Create(void);

private: 	// �Ҹ� �Լ�
	virtual void		Free(void);


};

#endif // MainApp_h__

