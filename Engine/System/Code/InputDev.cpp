#include "InputDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputDev)

Engine::CInputDev::CInputDev(void)
	: m_pInputSDK(nullptr)
	, m_pKeyBoard(nullptr)
	, m_pMouse(nullptr)
{

}	 

Engine::CInputDev::~CInputDev(void)
{
	Free();
}

_byte Engine::CInputDev::Get_DIKeyState(_ubyte byKeyID) 
{ 
	return m_byKeyState[byKeyID]; 
}

_byte Engine::CInputDev::Get_DIMouseState(MOUSEKEYSTATE eMouse) 
{ 
	return m_tMouseState.rgbButtons[eMouse]; 
}

_long Engine::CInputDev::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return *(((_long*)&m_tMouseState) + eMouseState);
}

_byte CInputDev::Get_DIMouseWheel()
{
	return m_tMouseState.lZ;
}

HRESULT Engine::CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	// DInput �İ�ü�� �����ϴ� �Լ�
	FAILED_CHECK_RETURN(DirectInput8Create(hInst,
											DIRECTINPUT_VERSION,
											IID_IDirectInput8,
											(void**)&m_pInputSDK,
											NULL), E_FAIL);

	// Ű���� ��ü ����
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);

	// ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pKeyBoard->Acquire();


	// ���콺 ��ü ����
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pMouse->Acquire();


	return S_OK;
}

void Engine::CInputDev::Set_InputDev(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}

void Engine::CInputDev::Free(void)
{
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);
}

