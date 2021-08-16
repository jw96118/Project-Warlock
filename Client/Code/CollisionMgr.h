#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Defines.h"
#include "GameObject.h"
#include "Layer.h"

class CollisionMgr
{
private:
	explicit CollisionMgr(void);
	virtual ~CollisionMgr(void);

public:
	static void		CollisionWallToPlayer(Engine::CLayer* pLayer, 
										  Engine::CGameObject* pObject);
	static void		CollisionDoorToPlayer(Engine::CLayer* pLayer,
		Engine::CGameObject* pObject);

	static void		CollisionWallToMonster(Engine::CLayer* pLayerWall,
										   Engine::CLayer* pLayerMonster);
	static void		CollisionWallToMiniMonster(Engine::CLayer* pLayerWall,
		Engine::CLayer* pLayerMonster, const _float & fTimeDelta);

public: // 충돌 판정
	// AABB 충돌
	static _bool	IsCollisionBoxToBox(const _vec3& vDestMin,
									    const _vec3& vDestMax,
									    const _vec3& vSrcMin,
									    const _vec3& vSrcMax);

	// 구 충돌
	static _bool	IsCollisionSphereToSphere(const SPHERECOLLIDER& tDest, 
											  const SPHERECOLLIDER& tSrc);

	// 레이-구 충돌
	static _bool	IsCollisionRayToSphere(_vec3& vIntersection, 
										   _float& fDist, 
										   const LPDIRECT3DDEVICE9 pGraphicDev, 
										   const SPHERECOLLIDER & tDest);

	static void		Ray(const LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vRayPos, _vec3& vRayDir);
};


#endif // WeaponMgr_h__