#include "GameObject.h"
#include "Texture.h"
#include "Transform.h"
#include "Renderer.h"
#include "Export_Resources.h"


USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();

	Set_Enable(true);
}

Engine::CGameObject::~CGameObject()
{

}

void CGameObject::Set_Enable(bool bEnable)
{
	m_bEnable = bEnable;

	if (m_bEnable)
		OnEnable_GameObject();
}

void CGameObject::Set_ObjType(OBJECTTYPE objType)
{
	m_ObjType = objType;
}

void CGameObject::Set_MonsterType(MONSTERTYPE monsterType)
{
	m_MonsterType = monsterType;
}

void CGameObject::Set_ItemType(ITEMTYPE itemType)
{
	m_ItemType = itemType;
}

void CGameObject::Set_ObjName(const _tchar * objName)
{
	m_ObjName = objName;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	//auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder(pComponentTag));
	auto	iter = m_mapComponent[eID].begin();

	for (; iter != m_mapComponent[eID].end(); iter++)
	{
		if (iter->first == pComponentTag)
			return iter->second;
	}
	return nullptr;
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	CComponent*		pComponent = Find_Component(pComponentTag, eID);
	NULL_CHECK_RETURN(pComponent, nullptr);
	
	return pComponent;
}

bool CGameObject::Get_Enable(void) const
{
	return m_bEnable;
}

bool * CGameObject::Get_EnableP(void)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return &m_bEnable;
}

OBJECTTYPE CGameObject::Get_ObjType()
{
	return m_ObjType;
}

MONSTERTYPE CGameObject::Get_MonsterType()
{
	return m_MonsterType;
}

ITEMTYPE CGameObject::Get_ItemType()
{
	return m_ItemType;
}

wstring CGameObject::Get_ObjName()
{
	return m_ObjName;
}

HRESULT Engine::CGameObject::Ready_GameObject(void)
{
	return S_OK;
}

HRESULT CGameObject::LateReady_GameObject(void)
{
	return S_OK;
}

_int Engine::CGameObject::Update_GameObject(const _float& fTimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);

	return 0;
}

_int CGameObject::LateUpdate_GameObject(const _float & fTimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component(fTimeDelta);

	return 0;
}

void Engine::CGameObject::Render_GameObject(void)
{

}

void CGameObject::OnEnable_GameObject(void)
{
}

HRESULT CGameObject::Insert_Component(int index)
{
	CComponent* pComponent = nullptr;
	Engine::COMPONENTID tempComID;
	TCHAR tempStr[20] = {};

	switch (index)
	{
	case 0:
		wcscpy_s(tempStr, L"Com_Texture");
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;

	default:
		break;
	}

	for (int i = 0; i < Engine::ID_END; i++)
	{
		if (!m_mapComponent[i].empty())
		{
			auto iter = m_mapComponent[i].begin();

			for (; iter != m_mapComponent[i].end(); iter++)
			{
				if (iter->first == tempStr)
				{
					return E_FAIL;
				}
			}
		}
	}
	switch (index)
	{
	case 0:
		//pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Terrain"));
		pComponent = m_pTextureCom = nullptr;
		tempComID = Engine::ID_STATIC;

		m_mapComponent[tempComID].emplace(L"Com_Texture", pComponent);

		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;

	default:
		break;
	}

	return S_OK;
}

HRESULT CGameObject::Set_Insert_Component(const _tchar * str, CComponent * Com)
{
	bool failflag = false;
	for (int i = 0; i < Engine::ID_END; i++)
	{
		if (!m_mapComponent[i].empty())
		{
			auto iter = m_mapComponent[i].begin();

			for (; iter != m_mapComponent[i].end(); iter++)
			{
				if (iter->first == str)
				{
					if (iter->first == L"Com_Texture")
					{
						iter->second = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Com));
						failflag = false;
					}
				}
				else
				{
					failflag = true;
				}
			}
		}
	}
	if (failflag)
	{
		return E_FAIL;
	}
	return S_OK;
}

const map<wstring, CComponent*>* CGameObject::Get_MapComponent(COMPONENTID eID) const
{
	if (m_mapComponent[eID].empty())
	{
		return nullptr;
	}
	return m_mapComponent;
}
const int & CGameObject::Get_TexCount() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_iTexCount;
}

const _tchar * CGameObject::Get_TexName() const
{
	return m_TexName;
}

void CGameObject::Save_Component(DWORD& dwBytes, const HANDLE& hFile, bool flag)
{
	int tempNameLen = (wcslen(m_ObjName.c_str()) * sizeof(_tchar));
	WriteFile(hFile, &tempNameLen, sizeof(int), &dwBytes, nullptr);
	WriteFile(hFile, m_ObjName.c_str(), tempNameLen * sizeof(_tchar), &dwBytes, nullptr);
	if (flag)
	{
		WriteFile(hFile, &m_ObjType, sizeof(OBJECTTYPE), &dwBytes, nullptr);
		switch (m_ObjType)
		{
		case Engine::OBJ_TERRAIN:
		case Engine::OBJ_WALL:
			for (int i = 0; i < ID_END; i++)
			{
				auto iter = m_mapComponent[i].begin();

				int tempSize = m_mapComponent[i].size();
				WriteFile(hFile, &tempSize, sizeof(int), &dwBytes, nullptr);
				for (; iter != m_mapComponent[i].end(); iter++)
				{
					int tempLen = (wcslen(iter->first.c_str()) * sizeof(_tchar));
					WriteFile(hFile, &tempLen, sizeof(int), &dwBytes, nullptr);
					WriteFile(hFile, iter->first.c_str(), tempLen * sizeof(_tchar), &dwBytes, nullptr);
					if (wcscmp(iter->first.c_str(), L"Com_Buffer") == 0)
					{
						//WriteFile(hFile, iter->second, sizeof(CRcTex), &dwBytes, nullptr);
					}
					else if (wcscmp(iter->first.c_str(), L"Com_Transform") == 0)
					{
						m_pTransformCom->Save_Transform(dwBytes, hFile);
					}
					else if (wcscmp(iter->first.c_str(), L"Com_Renderer") == 0)
					{
						//WriteFile(hFile, iter->second, sizeof(CRenderer), &dwBytes, nullptr);
					}
					else if (wcscmp(iter->first.c_str(), L"Com_Texture") == 0)
					{
						int tempTexLen = (wcslen(m_TexName) * sizeof(_tchar));

						WriteFile(hFile, &m_iTexCount, sizeof(int), &dwBytes, nullptr);

						WriteFile(hFile, &tempTexLen, sizeof(int), &dwBytes, nullptr);
						WriteFile(hFile, m_TexName, tempTexLen * sizeof(_tchar), &dwBytes, nullptr);
						bool flag = false;
						if (m_pTextureCom != nullptr)
						{
							flag = true;
							WriteFile(hFile, &flag, sizeof(bool), &dwBytes, nullptr);
							_ushort tempTexIdx = dynamic_cast<CTexture*>(iter->second)->m_wContainerIdx;
							int tempTagLen = (wcslen(dynamic_cast<CTexture*>(iter->second)->m_pResourceTag) * sizeof(_tchar));
							WriteFile(hFile, &tempTexIdx, sizeof(_ushort), &dwBytes, nullptr);
							WriteFile(hFile, &tempTagLen, sizeof(int), &dwBytes, nullptr);
							WriteFile(hFile, dynamic_cast<CTexture*>(iter->second)->m_pResourceTag, tempTagLen * sizeof(_tchar), &dwBytes, nullptr);
						}
						else
						{
							WriteFile(hFile, &flag, sizeof(bool), &dwBytes, nullptr);
						}
					}
				}
			}
			break;
		case Engine::OBJ_MONSTER:
			WriteFile(hFile, &m_MonsterType, sizeof(MONSTERTYPE), &dwBytes, nullptr);
			for (int i = 0; i < ID_END; i++)
			{
				auto iter = m_mapComponent[i].begin();

				int tempSize = m_mapComponent[i].size();
				WriteFile(hFile, &tempSize, sizeof(int), &dwBytes, nullptr);
				for (; iter != m_mapComponent[i].end(); iter++)
				{
					int tempLen = (wcslen(iter->first.c_str()) * sizeof(_tchar));
					WriteFile(hFile, &tempLen, sizeof(int), &dwBytes, nullptr);
					WriteFile(hFile, iter->first.c_str(), tempLen * sizeof(_tchar), &dwBytes, nullptr);

					if (wcscmp(iter->first.c_str(), L"Com_Transform") == 0)
					{
						m_pTransformCom->Save_Transform(dwBytes, hFile);
					}

				}
			}
			break;

		case Engine::OBJ_PLAYER:
			for (int i = 0; i < ID_END; i++)
			{
				auto iter = m_mapComponent[i].begin();

				int tempSize = m_mapComponent[i].size();
				WriteFile(hFile, &tempSize, sizeof(int), &dwBytes, nullptr);
				for (; iter != m_mapComponent[i].end(); iter++)
				{
					int tempLen = (wcslen(iter->first.c_str()) * sizeof(_tchar));
					WriteFile(hFile, &tempLen, sizeof(int), &dwBytes, nullptr);
					WriteFile(hFile, iter->first.c_str(), tempLen * sizeof(_tchar), &dwBytes, nullptr);

					if (wcscmp(iter->first.c_str(), L"Com_Transform") == 0)
					{
						m_pTransformCom->Save_Transform(dwBytes, hFile);
					}

				}
			}
			break;
		case Engine::OBJ_ITEM:
			WriteFile(hFile, &m_ItemType, sizeof(ITEMTYPE), &dwBytes, nullptr);
			for (int i = 0; i < ID_END; i++)
			{
				auto iter = m_mapComponent[i].begin();

				int tempSize = m_mapComponent[i].size();
				WriteFile(hFile, &tempSize, sizeof(int), &dwBytes, nullptr);
				for (; iter != m_mapComponent[i].end(); iter++)
				{
					int tempLen = (wcslen(iter->first.c_str()) * sizeof(_tchar));
					WriteFile(hFile, &tempLen, sizeof(int), &dwBytes, nullptr);
					WriteFile(hFile, iter->first.c_str(), tempLen * sizeof(_tchar), &dwBytes, nullptr);

					if (wcscmp(iter->first.c_str(), L"Com_Transform") == 0)
					{
						m_pTransformCom->Save_Transform(dwBytes, hFile);
					}

				}
			}
			break;
		case Engine::OBJ_END:
			break;
		default:
			break;
		}
	}
	else
	{
		m_pTransformCom->Save_Transform(dwBytes, hFile);
	}

}

CGameObject* CGameObject::Load_Component(DWORD& dwBytes, const HANDLE& hFile, bool _flag)
{

	wstring StempKey = L"";
	int tempNameLen = 0;
	int     templen = 0;
	int tempSize = 0;
	int TexLen = 0;
	bool flag = false;
	_tchar* tempstr = nullptr;
	_ushort tempTexIdx = 0;
	int tempTagLen = 0;

	ReadFile(hFile, &tempNameLen, sizeof(int), &dwBytes, nullptr);

	ZeroMemory(m_tempObjName, 40 * sizeof(_tchar));
	ReadFile(hFile, m_tempObjName, tempNameLen * sizeof(_tchar), &dwBytes, nullptr);
	m_ObjName = m_tempObjName;
	if (_flag)
	{
		ReadFile(hFile, &m_ObjType, sizeof(OBJECTTYPE), &dwBytes, nullptr);

		switch (m_ObjType)
		{
		case Engine::OBJ_TERRAIN:
		case Engine::OBJ_WALL:
			for (int i = 0; i < ID_END; i++)
			{
				if (!m_mapComponent[i].empty())
				{
					for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
					m_mapComponent[i].clear();
				}

				ReadFile(hFile, &tempSize, sizeof(int), &dwBytes, nullptr);
				for (int j = 0; j < tempSize; j++)
				{
					ReadFile(hFile, &templen, sizeof(int), &dwBytes, nullptr);

					ZeroMemory(m_LoadTempKey, 40 * sizeof(_tchar));

					ReadFile(hFile, m_LoadTempKey, templen * sizeof(_tchar), &dwBytes, nullptr);
					StempKey = m_LoadTempKey;
					if (wcscmp(StempKey.c_str(), L"Com_Buffer") == 0)
					{
						m_pBufferCom = dynamic_cast<Engine::CLightTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_LightTex"));
						m_mapComponent[i].emplace(StempKey, m_pBufferCom);
					}
					else if (wcscmp(StempKey.c_str(), L"Com_Transform") == 0)
					{
						m_pTransformCom = Engine::CTransform::Create();
						m_pTransformCom->Load_Transform(dwBytes, hFile);
						m_mapComponent[i].emplace(StempKey, m_pTransformCom);
					}
					else if (wcscmp(StempKey.c_str(), L"Com_Renderer") == 0)
					{
						m_pRendererCom = CRenderer::GetInstance();
						m_pRendererCom->AddRef();
						m_mapComponent[i].emplace(StempKey, m_pRendererCom);
					}
					else if (wcscmp(StempKey.c_str(), L"Com_Texture") == 0)
					{
						ReadFile(hFile, &m_iTexCount, sizeof(int), &dwBytes, nullptr);

						ReadFile(hFile, &TexLen, sizeof(int), &dwBytes, nullptr);

						ZeroMemory(m_TexName, 40 * sizeof(_tchar));
						ReadFile(hFile, m_TexName, TexLen * sizeof(_tchar), &dwBytes, nullptr);

						ReadFile(hFile, &flag, sizeof(bool), &dwBytes, nullptr);
						if (flag)
						{

							ReadFile(hFile, &tempTexIdx, sizeof(_ushort), &dwBytes, nullptr);
							ReadFile(hFile, &tempTagLen, sizeof(int), &dwBytes, nullptr);
							tempstr = new _tchar[40];
							ZeroMemory(tempstr, 40 * sizeof(_tchar));
							ReadFile(hFile, tempstr, tempTagLen * sizeof(_tchar), &dwBytes, nullptr);

							m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(tempTexIdx, tempstr));
							m_pTextureCom->SetSaveType(tempTexIdx, tempstr);
							m_mapComponent[i].emplace(StempKey, m_pTextureCom);
							delete[] tempstr;
						}
						else
						{
							m_mapComponent[i].emplace(StempKey, m_pTextureCom);
						}
					}

				}
			}
			break;
		case Engine::OBJ_MONSTER:
			ReadFile(hFile, &m_MonsterType, sizeof(MONSTERTYPE), &dwBytes, nullptr);
			for (int i = 0; i < ID_END; i++)
			{
				if (!m_mapComponent[i].empty())
				{
					auto deleteIter = m_mapComponent[i].begin();
					for (; deleteIter != m_mapComponent[i].end(); deleteIter++)
					{
						if (deleteIter->first == L"Com_Transform")
						{
							m_pTransformCom = dynamic_cast<CTransform*>(deleteIter->second);
						}
					}
				}
				ReadFile(hFile, &tempSize, sizeof(int), &dwBytes, nullptr);
				for (int j = 0; j < tempSize; j++)
				{
					ReadFile(hFile, &templen, sizeof(int), &dwBytes, nullptr);
					ZeroMemory(m_LoadTempKey, 40 * sizeof(_tchar));
					ReadFile(hFile, m_LoadTempKey, templen * sizeof(_tchar), &dwBytes, nullptr);
					StempKey = m_LoadTempKey;

					if (wcscmp(StempKey.c_str(), L"Com_Transform") == 0)
					{
						m_pTransformCom->Load_Transform(dwBytes, hFile);
						m_mapComponent[i].emplace(StempKey, m_pTransformCom);
					}
				}
			}
			break;
		case Engine::OBJ_PLAYER:
			for (int i = 0; i < ID_END; i++)
			{
				if (!m_mapComponent[i].empty())
				{
					auto deleteIter = m_mapComponent[i].begin();
					for (; deleteIter != m_mapComponent[i].end(); deleteIter++)
					{
						if (deleteIter->first == L"Com_Transform")
						{
							m_pTransformCom = dynamic_cast<CTransform*>(deleteIter->second);
							//Engine::Safe_Release(deleteIter->second);
							//deleteIter = m_mapComponent[i].erase(deleteIter);
						}
						else
						{
							//deleteIter++;
						}
					}
				}
				ReadFile(hFile, &tempSize, sizeof(int), &dwBytes, nullptr);
				for (int j = 0; j < tempSize; j++)
				{
					ReadFile(hFile, &templen, sizeof(int), &dwBytes, nullptr);

					ZeroMemory(m_LoadTempKey, 40 * sizeof(_tchar));

					ReadFile(hFile, m_LoadTempKey, templen * sizeof(_tchar), &dwBytes, nullptr);
					StempKey = m_LoadTempKey;

					if (wcscmp(StempKey.c_str(), L"Com_Transform") == 0)
					{
						//m_pTransformCom = Engine::CTransform::Create();
						m_pTransformCom->Load_Transform(dwBytes, hFile);
						m_mapComponent[i].emplace(StempKey, m_pTransformCom);
					}
				}
			}

			break;
		case Engine::OBJ_ITEM:
			ReadFile(hFile, &m_ItemType, sizeof(ITEMTYPE), &dwBytes, nullptr);
			for (int i = 0; i < ID_END; i++)
			{
				if (!m_mapComponent[i].empty())
				{
					auto deleteIter = m_mapComponent[i].begin();
					for (; deleteIter != m_mapComponent[i].end(); deleteIter++)
					{
						if (deleteIter->first == L"Com_Transform")
						{
							m_pTransformCom = dynamic_cast<CTransform*>(deleteIter->second);
						}
					}
				}
				ReadFile(hFile, &tempSize, sizeof(int), &dwBytes, nullptr);
				for (int j = 0; j < tempSize; j++)
				{
					ReadFile(hFile, &templen, sizeof(int), &dwBytes, nullptr);
					ZeroMemory(m_LoadTempKey, 40 * sizeof(_tchar));
					ReadFile(hFile, m_LoadTempKey, templen * sizeof(_tchar), &dwBytes, nullptr);
					StempKey = m_LoadTempKey;

					if (wcscmp(StempKey.c_str(), L"Com_Transform") == 0)
					{
						m_pTransformCom->Load_Transform(dwBytes, hFile);
						m_mapComponent[i].emplace(StempKey, m_pTransformCom);
					}
				}
			}
			break;
		case Engine::OBJ_END:
			break;
		default:
			break;
		}
	}
	else
	{
	   m_pTransformCom->Load_Transform(dwBytes, hFile);
	}
	return this;
}





Engine::CTexture * CGameObject::GetTextureCom()
{
	if (m_pTextureCom != nullptr)
		return m_pTextureCom;
	return nullptr;
}

Engine::CTransform * CGameObject::GetTransformCom()
{
	if (m_pTransformCom != nullptr)
		return m_pTransformCom;
	return nullptr;
}

map<wstring, CComponent*>& CGameObject::GetMapCom(int i)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_mapComponent[i];
}

CGameObject * CGameObject::Create(LPDIRECT3DDEVICE9 device)
{
	CGameObject* pInstance = new CGameObject(device);
	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;
	return pInstance;
}

void Engine::CGameObject::Free(void)
{

	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}
	Safe_Release(m_pGraphicDev);
}

void CGameObject::SetSelect(bool flag)
{
	m_bSelect = flag;
}

