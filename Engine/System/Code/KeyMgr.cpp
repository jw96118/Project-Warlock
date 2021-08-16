#include "KeyMgr.h"

USING(Engine)

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
	: m_dwKeyDown(0), m_dwKeyUp(0), m_dwCurKey(0)
{

}
CKeyMgr::~CKeyMgr()
{
	Free();
}

void CKeyMgr::Update()
{
	m_dwCurKey = 0;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwCurKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwCurKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwCurKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwCurKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwCurKey |= KEY_SPACE;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwCurKey |= KEY_RETURN;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwCurKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwCurKey |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		m_dwCurKey |= KEY_LCTRL;
	if (GetAsyncKeyState(VK_DELETE) & 0x8000)
		m_dwCurKey |= KEY_DELETE;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwCurKey |= KEY_A;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwCurKey |= KEY_S;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwCurKey |= KEY_Q;
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwCurKey |= KEY_W;
	if (GetAsyncKeyState('R') & 0x8000)
		m_dwCurKey |= KEY_R;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwCurKey |= KEY_D;
	if (GetAsyncKeyState('1') & 0x8000)
		m_dwCurKey |= KEY_NUM1;
	if (GetAsyncKeyState('2') & 0x8000)
		m_dwCurKey |= KEY_NUM2;
	if (GetAsyncKeyState('3') & 0x8000)
		m_dwCurKey |= KEY_NUM3;
	if (GetAsyncKeyState('4') & 0x8000)
		m_dwCurKey |= KEY_NUM4;
	if (GetAsyncKeyState('I') & 0x8000)
		m_dwCurKey |= KEY_I;
}

bool CKeyMgr::KeyDown(DWORD dwKey)
{
	// ���� ������ ���� ���� ������ �� TRUE
	if (!(m_dwKeyDown & dwKey) && (m_dwCurKey & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	// ���� ������ �ְ� ���� ������ �ʾ��� �� FALSE
	if ((m_dwKeyDown & dwKey) && !(m_dwCurKey & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyUp(DWORD dwKey)
{
	// ���� ������ �ְ� ���� ������ �ʾ��� �� TRUE
	if ((m_dwKeyUp & dwKey) && !(m_dwCurKey & dwKey))
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}
	// ���� ������ ���� ���� ������ �� FALSE
	if (!(m_dwKeyUp & dwKey) && (m_dwCurKey & dwKey))
	{
		m_dwKeyUp |= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyPressing(DWORD dwKey)
{
	if (m_dwCurKey & dwKey)
		return true;

	return false;
}

void CKeyMgr::Free(void)
{
}
