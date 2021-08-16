#ifndef MiniMonster_h__
#define MiniMonster_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CRenderer;
class CTexture;
class CLightTex;
class CCamera;
class CLayer;

END

class CPlayer;
class CMiniMonsterEvent;
class CMiniMonster : public Engine::CGameObject
{
private:
	explicit CMiniMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniMonster(void);

public:
	BOXCOLLIDER*			Get_BoxCollider(void);
public:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual HRESULT			LateReady_GameObject(void) override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;

	void					Update_Collider();
	void                    Set_Transform(_vec3 scale, _vec3 pos, _vec3 angle);
	void                    Set_ColSize(_vec3 colSize);
	void                    Set_Layer(Engine::CLayer*         pColObject);
	const float&            Get_Speed() const;
private:
	HRESULT					Add_Component(void);
	void                    Animaition(const _float& fTimeDelta);
private:
	BOXCOLLIDER				m_tBoxCollider;
	SPHERECOLLIDER          m_tSphereCollider;
	_vec3                   m_ColSize;
	//	Engine::CTransform*		m_pTargetTransformCom;
	CPlayer*                m_pPlayerObj;
	CMiniMonsterEvent*      m_pEventUi;
	Engine::CLayer*         m_ColObject;
	float                   m_fSpeed;
	FRAME					m_tFrame;
	bool temp = false;
	bool temp2 = false;
	bool m_bSoundAllStop = false;
	float                   m_fEventCount = 0.f;
public:
	static CMiniMonster*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free(void) override;
};

#endif // Player_h__
