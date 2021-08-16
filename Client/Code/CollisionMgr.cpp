#include "stdafx.h"
#include "CollisionMgr.h"

#include "Player.h"
#include "ColObject.h"
#include "FirstPersonCamera.h"
#include "Monster.h"
#include "MiniMonster.h"
#include "Projectile.h"
#include "EventDoor.h"
#include "Export_Function.h"

CollisionMgr::CollisionMgr(void)
{
}

CollisionMgr::~CollisionMgr(void)
{
}

void CollisionMgr::CollisionWallToPlayer(Engine::CLayer* pLayer,
										 Engine::CGameObject* pObject)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObject);
	NULL_CHECK(pPlayer);

	BOXCOLLIDER& tPlayer = *pPlayer->Get_BoxCollider();

	auto mapObject = pLayer->GetMapObj();

	for (auto iter : mapObject)
	{
		CColObject* pColObject = dynamic_cast<CColObject*>(iter.second);
		NULL_CHECK(pColObject);

		BOXCOLLIDER tCollider = *pColObject->Get_Collider();

		if (IsCollisionBoxToBox(tPlayer.vMin, tPlayer.vMax, tCollider.vMin, tCollider.vMax))
		{
			// x���� ������ �հ� �Ÿ��� ���Ѵ�.
			// �� �� x���� ������ ���� x���� �Ÿ����� ũ�� x������ ��ħ.
			_float fSumX = (tPlayer.vMax.x - tPlayer.vMin.x) * 0.5f + (tCollider.vMax.x - tCollider.vMin.x) * 0.5f;
			_float fDistX = fabs((tPlayer.vMax.x + tPlayer.vMin.x) * 0.5f - (tCollider.vMax.x + tCollider.vMin.x) * 0.5f);

			// z���� ������ �հ� �Ÿ��� ���Ѵ�.
			// �� �� z���� ������ ���� z���� �Ÿ����� ũ�� z������ ��ħ.
			_float fSumZ = (tPlayer.vMax.z - tPlayer.vMin.z) * 0.5f + (tCollider.vMax.z - tCollider.vMin.z) * 0.5f;
			_float fDistZ = fabs((tPlayer.vMax.z + tPlayer.vMin.z) * 0.5f - (tCollider.vMax.z + tCollider.vMin.z) * 0.5f);

			// ��ģ������ ũ�� ���ϱ�.
			_float fMoveX = fSumX - fDistX;
			_float fMoveZ = fSumZ - fDistZ;

			Engine::CTransform* pPlayerTransform = dynamic_cast<Engine::CTransform*>(
				pPlayer->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
			NULL_CHECK(pPlayerTransform);

			CFirstPersonCamera* pCamera = dynamic_cast<CFirstPersonCamera*>(
				Engine::Get_GameObject(L"01.Environment", L"MainCamera"));
			NULL_CHECK(pCamera);

			_float fX = (tPlayer.vMax.x + tPlayer.vMin.x) * 0.5f;
			_float fZ = (tPlayer.vMax.z + tPlayer.vMin.z) * 0.5f;

			if (fMoveX > fMoveZ) // Z������ �о
			{
				if ((tCollider.vMax.z + tCollider.vMin.z) * 0.5f < fZ)
				{
					_vec3 vPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);
					vPos = { vPos.x, vPos.y, vPos.z + fMoveZ };

					pPlayerTransform->Set_Pos(&vPos);
					pPlayerTransform->m_vInfo[Engine::INFO_POS] = vPos;
					pPlayer->Update_Collider();

					pCamera->Move_View(_vec3(0.f, 0.f, fMoveZ));
				}
				else
				{
					_vec3 vPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);
					vPos = { vPos.x, vPos.y, vPos.z - fMoveZ };

					pPlayerTransform->Set_Pos(&vPos);
					pPlayerTransform->m_vInfo[Engine::INFO_POS] = vPos;
					pPlayer->Update_Collider();

					pCamera->Move_View(_vec3(0.f, 0.f, -fMoveZ));
				}
			}
			else // X������ �о
			{
				if ((tCollider.vMax.x + tCollider.vMin.x) * 0.5f < fX)
				{
					_vec3 vPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);
					vPos = { vPos.x + fMoveX, vPos.y, vPos.z };

					pPlayerTransform->Set_Pos(&vPos);
					pPlayerTransform->m_vInfo[Engine::INFO_POS] = vPos;
					pPlayer->Update_Collider();

					pCamera->Move_View(_vec3(fMoveX, 0.f, 0.f));
				}
				else
				{
					_vec3 vPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);
					vPos = { vPos.x - fMoveX, vPos.y, vPos.z };

					pPlayerTransform->Set_Pos(&vPos);
					pPlayerTransform->m_vInfo[Engine::INFO_POS] = vPos;
					pPlayer->Update_Collider();

					pCamera->Move_View(_vec3(-fMoveX, 0.f, 0.f));
				}
			}
		}
	}
}

void CollisionMgr::CollisionDoorToPlayer(Engine::CLayer * pLayer, Engine::CGameObject * pObject)
{

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObject);
	NULL_CHECK(pPlayer);

	BOXCOLLIDER& tPlayer = *pPlayer->Get_BoxCollider();

	auto mapObject = pLayer->GetMapObj();

	for (auto iter : mapObject)
	{
		BOXCOLLIDER& tCollider = *(dynamic_cast<CEventDoor*>(iter.second)->Get_BoxCollider());


		if (IsCollisionBoxToBox(tPlayer.vMin, tPlayer.vMax, tCollider.vMin, tCollider.vMax))
		{
			// x���� ������ �հ� �Ÿ��� ���Ѵ�.
			// �� �� x���� ������ ���� x���� �Ÿ����� ũ�� x������ ��ħ.
			_float fSumX = (tPlayer.vMax.x - tPlayer.vMin.x) * 0.5f + (tCollider.vMax.x - tCollider.vMin.x) * 0.5f;
			_float fDistX = fabs((tPlayer.vMax.x + tPlayer.vMin.x) * 0.5f - (tCollider.vMax.x + tCollider.vMin.x) * 0.5f);

			// z���� ������ �հ� �Ÿ��� ���Ѵ�.
			// �� �� z���� ������ ���� z���� �Ÿ����� ũ�� z������ ��ħ.
			_float fSumZ = (tPlayer.vMax.z - tPlayer.vMin.z) * 0.5f + (tCollider.vMax.z - tCollider.vMin.z) * 0.5f;
			_float fDistZ = fabs((tPlayer.vMax.z + tPlayer.vMin.z) * 0.5f - (tCollider.vMax.z + tCollider.vMin.z) * 0.5f);

			// ��ģ������ ũ�� ���ϱ�.
			_float fMoveX = fSumX - fDistX;
			_float fMoveZ = fSumZ - fDistZ;

			Engine::CTransform* pPlayerTransform = dynamic_cast<Engine::CTransform*>(
				pPlayer->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
			NULL_CHECK(pPlayerTransform);

			CFirstPersonCamera* pCamera = dynamic_cast<CFirstPersonCamera*>(
				Engine::Get_GameObject(L"01.Environment", L"MainCamera"));
			NULL_CHECK(pCamera);

			_float fX = (tPlayer.vMax.x + tPlayer.vMin.x) * 0.5f;
			_float fZ = (tPlayer.vMax.z + tPlayer.vMin.z) * 0.5f;

			if (fMoveX > fMoveZ) // Z������ �о
			{
				if ((tCollider.vMax.z + tCollider.vMin.z) * 0.5f < fZ)
				{
					_vec3 vPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);
					vPos = { vPos.x, vPos.y, vPos.z + fMoveZ };

					pPlayerTransform->Set_Pos(&vPos);
					pPlayerTransform->m_vInfo[Engine::INFO_POS] = vPos;
					pPlayer->Update_Collider();

					pCamera->Move_View(_vec3(0.f, 0.f, fMoveZ));
				}
				else
				{
					_vec3 vPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);
					vPos = { vPos.x, vPos.y, vPos.z - fMoveZ };

					pPlayerTransform->Set_Pos(&vPos);
					pPlayerTransform->m_vInfo[Engine::INFO_POS] = vPos;
					pPlayer->Update_Collider();

					pCamera->Move_View(_vec3(0.f, 0.f, -fMoveZ));
				}
			}
			else // X������ �о
			{
				if ((tCollider.vMax.x + tCollider.vMin.x) * 0.5f < fX)
				{
					_vec3 vPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);
					vPos = { vPos.x + fMoveX, vPos.y, vPos.z };

					pPlayerTransform->Set_Pos(&vPos);
					pPlayerTransform->m_vInfo[Engine::INFO_POS] = vPos;
					pPlayer->Update_Collider();

					pCamera->Move_View(_vec3(fMoveX, 0.f, 0.f));
				}
				else
				{
					_vec3 vPos = *pPlayerTransform->Get_Info(Engine::INFO_POS);
					vPos = { vPos.x - fMoveX, vPos.y, vPos.z };

					pPlayerTransform->Set_Pos(&vPos);
					pPlayerTransform->m_vInfo[Engine::INFO_POS] = vPos;
					pPlayer->Update_Collider();

					pCamera->Move_View(_vec3(-fMoveX, 0.f, 0.f));
				}
			}
		}
	}
}

void CollisionMgr::CollisionWallToMonster(Engine::CLayer * pLayerWall, Engine::CLayer * pLayerMonster)
{
	auto mapWalls = pLayerWall->GetMapObj();
	auto mapMonsters = pLayerMonster->GetMapObj();

	for (auto iterWall : mapWalls)
	{
		CColObject * pWall = dynamic_cast<CColObject*>(iterWall.second);
		NULL_CHECK(pWall);

		const BOXCOLLIDER& tWallCollider = *pWall->Get_Collider();

		for (auto iterMonsters : mapMonsters)
		{
			CMonster * pMonster = dynamic_cast<CMonster*>(iterMonsters.second);
			NULL_CHECK(pMonster);
			if (pMonster->Get_CurHP()<=0.f)
				continue;
			const BOXCOLLIDER& tMonsterCollider = pMonster->Get_BoxCollider();

			if (IsCollisionBoxToBox(tMonsterCollider.vMin, tMonsterCollider.vMax, tWallCollider.vMin, tWallCollider.vMax))
			{

				// x���� ������ �հ� �Ÿ��� ���Ѵ�.
				// �� �� x���� ������ ���� x���� �Ÿ����� ũ�� x������ ��ħ.
				_float fSumX = (tMonsterCollider.vMax.x - tMonsterCollider.vMin.x) * 0.5f + (tWallCollider.vMax.x - tWallCollider.vMin.x) * 0.5f;
				_float fDistX = fabs((tMonsterCollider.vMax.x + tMonsterCollider.vMin.x) * 0.5f - (tWallCollider.vMax.x + tWallCollider.vMin.x) * 0.5f);

				// z���� ������ �հ� �Ÿ��� ���Ѵ�.
				// �� �� z���� ������ ���� z���� �Ÿ����� ũ�� z������ ��ħ.
				_float fSumZ = (tMonsterCollider.vMax.z - tMonsterCollider.vMin.z) * 0.5f + (tWallCollider.vMax.z - tWallCollider.vMin.z) * 0.5f;
				_float fDistZ = fabs((tMonsterCollider.vMax.z + tMonsterCollider.vMin.z) * 0.5f - (tWallCollider.vMax.z + tWallCollider.vMin.z) * 0.5f);

				// ��ģ������ ũ�� ���ϱ�.
				_float fMoveX = fSumX - fDistX;
				_float fMoveZ = fSumZ - fDistZ;

				Engine::CTransform* pMonsterTransform = dynamic_cast<Engine::CTransform*>(
					pMonster->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
				NULL_CHECK(pMonsterTransform);


				_float fX = (tMonsterCollider.vMax.x + tMonsterCollider.vMin.x) * 0.5f;
				_float fZ = (tMonsterCollider.vMax.z + tMonsterCollider.vMin.z) * 0.5f;

				if (fMoveX > fMoveZ) // Z������ �о
				{
					if ((tWallCollider.vMax.z + tWallCollider.vMin.z) * 0.5f < fZ)
					{
						_vec3 vPos = *pMonsterTransform->Get_Info(Engine::INFO_POS);
						vPos = { vPos.x, pMonsterTransform->Get_WorldMatrix()->_42, vPos.z + fMoveZ };

						pMonsterTransform->Set_Pos(&vPos);
						pMonsterTransform->m_vInfo[Engine::INFO_POS] = vPos;
						pMonster->Update_BoxCollider();
					}
					else
					{
						_vec3 vPos = *pMonsterTransform->Get_Info(Engine::INFO_POS);
						vPos = { vPos.x, pMonsterTransform->Get_WorldMatrix()->_42, vPos.z - fMoveZ };

						pMonsterTransform->Set_Pos(&vPos);
						pMonsterTransform->m_vInfo[Engine::INFO_POS] = vPos;
						pMonster->Update_BoxCollider();
					}
				}
				else // X������ �о
				{
					if ((tWallCollider.vMax.x + tWallCollider.vMin.x) * 0.5f < fX)
					{
						_vec3 vPos = *pMonsterTransform->Get_Info(Engine::INFO_POS);
						vPos = { vPos.x + fMoveX, pMonsterTransform->Get_WorldMatrix()->_42, vPos.z };

						pMonsterTransform->Set_Pos(&vPos);
						pMonsterTransform->m_vInfo[Engine::INFO_POS] = vPos;
						pMonster->Update_BoxCollider();
					}
					else
					{
						_vec3 vPos = *pMonsterTransform->Get_Info(Engine::INFO_POS);
						vPos = { vPos.x - fMoveX, pMonsterTransform->Get_WorldMatrix()->_42, vPos.z };

						pMonsterTransform->Set_Pos(&vPos);
						pMonsterTransform->m_vInfo[Engine::INFO_POS] = vPos;
						pMonster->Update_BoxCollider();
					}
				}
			}

		}
	}
}
void CollisionMgr::CollisionWallToMiniMonster(Engine::CLayer * pLayerWall, Engine::CLayer * pLayerMonster , const _float & fTimeDelta)
{
	auto mapWalls = pLayerWall->GetMapObj();
	auto mapMonsters = pLayerMonster->GetMapObj();

	Engine::CTransform* m_pPlayerTransformCom = dynamic_cast<Engine::CTransform*>(
		Engine::Get_Component(L"02.GameLogic", L"Obj_Player", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pPlayerTransformCom);
	for (auto iterWall : mapWalls)
	{
		CColObject * pWall = dynamic_cast<CColObject*>(iterWall.second);
		NULL_CHECK(pWall);

		const BOXCOLLIDER& tWallCollider = *pWall->Get_Collider();

		for (auto iterMonsters : mapMonsters)
		{
			CMiniMonster * pMonster = dynamic_cast<CMiniMonster*>(iterMonsters.second);
			NULL_CHECK(pMonster);
			const BOXCOLLIDER& tMonsterCollider = *(pMonster->Get_BoxCollider());

			if (IsCollisionBoxToBox(tMonsterCollider.vMin, tMonsterCollider.vMax, tWallCollider.vMin, tWallCollider.vMax))
			{

				// x���� ������ �հ� �Ÿ��� ���Ѵ�.
				// �� �� x���� ������ ���� x���� �Ÿ����� ũ�� x������ ��ħ.
				_float fSumX = (tMonsterCollider.vMax.x - tMonsterCollider.vMin.x) * 0.5f + (tWallCollider.vMax.x - tWallCollider.vMin.x) * 0.5f;
				_float fDistX = fabs((tMonsterCollider.vMax.x + tMonsterCollider.vMin.x) * 0.5f - (tWallCollider.vMax.x + tWallCollider.vMin.x) * 0.5f);

				// z���� ������ �հ� �Ÿ��� ���Ѵ�.
				// �� �� z���� ������ ���� z���� �Ÿ����� ũ�� z������ ��ħ.
				_float fSumZ = (tMonsterCollider.vMax.z - tMonsterCollider.vMin.z) * 0.5f + (tWallCollider.vMax.z - tWallCollider.vMin.z) * 0.5f;
				_float fDistZ = fabs((tMonsterCollider.vMax.z + tMonsterCollider.vMin.z) * 0.5f - (tWallCollider.vMax.z + tWallCollider.vMin.z) * 0.5f);

				// ��ģ������ ũ�� ���ϱ�.
				_float fMoveX = fSumX - fDistX;
				_float fMoveZ = fSumZ - fDistZ;

				Engine::CTransform* pMonsterTransform = dynamic_cast<Engine::CTransform*>(
					pMonster->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
				NULL_CHECK(pMonsterTransform);


				_float fX = (tMonsterCollider.vMax.x + tMonsterCollider.vMin.x) * 0.5f;
				_float fZ = (tMonsterCollider.vMax.z + tMonsterCollider.vMin.z) * 0.5f;

				if (fMoveX > fMoveZ) // Z������ �о
				{
					if ((tWallCollider.vMax.z + tWallCollider.vMin.z) * 0.5f < fZ)
					{
						_vec3 vPos = *pMonsterTransform->Get_Info(Engine::INFO_POS);
						vPos = { vPos.x, pMonsterTransform->Get_WorldMatrix()->_42, vPos.z + fMoveZ };

						pMonsterTransform->Set_Pos(&vPos);
						pMonsterTransform->m_vInfo[Engine::INFO_POS] = vPos;
						if (vPos.x - m_pPlayerTransformCom->m_vInfo[Engine::INFO_POS].x >= 0)
						{
							pMonsterTransform->Move_Pos(&_vec3(-(pMonster->Get_Speed() * fTimeDelta), 0.f, 0.f));
						}
						else
						{
							pMonsterTransform->Move_Pos(&_vec3((pMonster->Get_Speed() * fTimeDelta), 0.f, 0.f));
						}
						pMonster->Update_Collider();
						
					}
					else
					{
						_vec3 vPos = *pMonsterTransform->Get_Info(Engine::INFO_POS);
						vPos = { vPos.x, pMonsterTransform->Get_WorldMatrix()->_42, vPos.z - fMoveZ };

						pMonsterTransform->Set_Pos(&vPos);
						pMonsterTransform->m_vInfo[Engine::INFO_POS] = vPos;
						if (vPos.x - m_pPlayerTransformCom->m_vInfo[Engine::INFO_POS].x >= 0)
						{
							pMonsterTransform->Move_Pos(&_vec3(-((pMonster->Get_Speed() * fTimeDelta)), 0.f, 0.f));
						}
						else
						{
							pMonsterTransform->Move_Pos(&_vec3((pMonster->Get_Speed() * fTimeDelta), 0.f, 0.f));
						}
						pMonster->Update_Collider();
						
					}
				}
				else // X������ �о
				{
					if ((tWallCollider.vMax.x + tWallCollider.vMin.x) * 0.5f < fX)
					{
						_vec3 vPos = *pMonsterTransform->Get_Info(Engine::INFO_POS);
						vPos = { vPos.x + fMoveX, pMonsterTransform->Get_WorldMatrix()->_42, vPos.z };

						pMonsterTransform->Set_Pos(&vPos);
						pMonsterTransform->m_vInfo[Engine::INFO_POS] = vPos;
						if (vPos.z - m_pPlayerTransformCom->m_vInfo[Engine::INFO_POS].z >= 0)
						{
							pMonsterTransform->Move_Pos(&_vec3(0.f, 0.f, -(pMonster->Get_Speed() * fTimeDelta)));
						}
						else
						{
							pMonsterTransform->Move_Pos(&_vec3(0.f, 0.f, (pMonster->Get_Speed() * fTimeDelta)));
						}
						pMonster->Update_Collider();
					
					}
					else
					{
						_vec3 vPos = *pMonsterTransform->Get_Info(Engine::INFO_POS);
						vPos = { vPos.x - fMoveX, pMonsterTransform->Get_WorldMatrix()->_42, vPos.z };

						pMonsterTransform->Set_Pos(&vPos);
						pMonsterTransform->m_vInfo[Engine::INFO_POS] = vPos;
						if (vPos.z - m_pPlayerTransformCom->m_vInfo[Engine::INFO_POS].z >= 0)
						{
							pMonsterTransform->Move_Pos(&_vec3(0.f, 0.f, -(pMonster->Get_Speed() * fTimeDelta)));
						}
						else
						{
							pMonsterTransform->Move_Pos(&_vec3(0.f, 0.f, (pMonster->Get_Speed() * fTimeDelta)));
						}

						pMonster->Update_Collider();
					}
				}
			}

		}
	}

}

_bool CollisionMgr::IsCollisionBoxToBox(const _vec3 & vDestMin, 
									    const _vec3 & vDestMax, 
									    const _vec3 & vSrcMin, 
									    const _vec3 & vSrcMax)
{
	if (vDestMin.x <= vSrcMax.x && vDestMax.x >= vSrcMin.x &&
		vDestMin.y <= vSrcMax.y && vDestMax.y >= vSrcMin.y &&
		vDestMin.z <= vSrcMax.z && vDestMax.z >= vSrcMin.z)
	{
		return true;
	}

	return false;
}

_bool CollisionMgr::IsCollisionSphereToSphere(const SPHERECOLLIDER & tDest, const SPHERECOLLIDER & tSrc)
{
	_float fLength = D3DXVec3Length(&(tDest.vCenter - tSrc.vCenter));

	if (fLength <= tDest.fRadius + tSrc.fRadius)
		return true;

	return false;
}

_bool CollisionMgr::IsCollisionRayToSphere(_vec3 & vIntersection,
										   _float & fDist, 
										   const LPDIRECT3DDEVICE9 pGraphicDev, 
										   const SPHERECOLLIDER & tDest)
{
	//SetCursorPos(WINCX * 0.5f, WINCY * 0.5f);
	
	//if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_LBUTTON))
	//{
		POINT			ptMouse{};

		// ������ ���� == ����Ʈ ����
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		// �� ��Ʈ���� ��������

		D3DVIEWPORT9			ViewPort;
		ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
		pGraphicDev->GetViewport(&ViewPort);

		_vec3	vMousePos;

		vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
		vMousePos.y = -ptMouse.y / (ViewPort.Height * 0.5f) + 1.f;
		vMousePos.z = 0.f;

		// �������� �� �����̽���
		_matrix		matProj;

		pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		D3DXMatrixInverse(&matProj, NULL, &matProj);
		D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

		_vec3	vRayPos, vRayDir;

		vRayPos = _vec3(0.f, 0.f, 0.f);
		vRayDir = vMousePos - vRayPos;

		// �� �����̽����� ���� �����̽���
		_matrix		matView;

		pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, NULL, &matView);
		D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
		D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
		D3DXVec3Normalize(&vRayDir, &vRayDir);

		_vec3 l = tDest.vCenter - vRayPos;

		_float s = D3DXVec3Dot(&l, &vRayDir);

		_float l2 = D3DXVec3Dot(&l, &l);

		_float r2 = pow(tDest.fRadius, 2);

		if (s < 0 && l2 > r2)
			return false;                       // ������ ���� �ݴ� ������ ���ϰų� ���� ����ģ ���

		_float m2 = l2 - pow(s, 2);


		if (m2 > r2)
			return false;                      // ������ ���� �񲸰��� ���

		_float q = sqrt(r2 - m2);

		// �� ���� ������ �� ��� ���� ���ϴ°�?   
		fDist = 0.f;

		if (l2 > r2) 
			fDist = s - q;
		else 
			fDist = s + q;

		//static int iCnt = 1000;

		//wstring wstrProjectile = L"Bat_spit" + to_wstring(iCnt);
		//Engine::CGameObject * pGo = CProjectile::Create(pGraphicDev, L"Texture_Bat_Spit", vRayPos, vRayDir);


		//Engine::Get_Layer(L"02.GameLogic")->Add_GameObject(wstrProjectile.c_str(), pGo);
		//++iCnt;

		vIntersection = vRayPos + vRayDir * fDist;

		return true;

	//}

	//return false;
}

void CollisionMgr::Ray(const LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vRayPos, _vec3 & vRayDir)
{
	POINT			ptMouse{};

	// ������ ���� == ����Ʈ ����
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	// �� ��Ʈ���� ��������

	D3DVIEWPORT9			ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	pGraphicDev->GetViewport(&ViewPort);

	_vec3	vMousePos;

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = -ptMouse.y / (ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// �������� �� �����̽���
	_matrix		matProj;

	pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	// �� �����̽����� ���� �����̽���
	_matrix		matView;

	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	D3DXVec3Normalize(&vRayDir, &vRayDir);
}
