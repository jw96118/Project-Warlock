#ifndef EventDoor_h__
#define EventDoor_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CRenderer;
class CTexture;
class CLightTex;
class CCamera;

END

class CWeaponMgr;

class CEventDoor : public Engine::CGameObject
{
private:
	explicit CEventDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEventDoor(void);

public:
	BOXCOLLIDER*			Get_BoxCollider(void);
public:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual HRESULT			LateReady_GameObject(void) override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;

	void					Update_Collider(const _float& fTimeDelta);
	void                    Set_Transform(_vec3 scale,_vec3 pos , _vec3 angle);
	void                    Set_ColSize(_vec3 colSize);
	void                    Set_Up(bool flag);
private:
	HRESULT					Add_Component(void);
private:
	BOXCOLLIDER				m_tBoxCollider;
	_vec3                   m_ColSize;
	bool                    m_bUp;
	float                   m_fUpSpeed;
public:
	static CEventDoor*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free(void) override;
};

#endif // Player_h__
