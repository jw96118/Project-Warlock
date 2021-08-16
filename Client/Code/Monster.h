#ifndef Monster_h__
#define Monster_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CCalculator;

END

class CMonster : public Engine::CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName);
	virtual ~CMonster(void);

public:
	const BOXCOLLIDER&		Get_BoxCollider() const;
	const SPHERECOLLIDER&	Get_HitCollider() const;

public:
	void					Set_Pos(_float fX, _float fY, _float fZ);
	void					SetScale(_float fScale);
	_bool					Dead_Mon() { return m_bDeath; }
	_float					Get_CurHP() { return m_tData.fCurHP; }
	void					Get_Damage(_float fDamage);
public:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	virtual HRESULT			LateReady_GameObject(void);

	virtual void			Update_HitCollider(void)PURE;
	virtual void			Update_BoxCollider(void)PURE;

	virtual	HRESULT			Add_Component(void);

protected:
	void					On_SoundFlag(_uint uiFlag, wstring AudioName);
	void					Off_SoundFlag(_uint uiFlag);
protected:
	void					SetUp_OnTerrain(void);
	virtual void			Change_Animation(_uint uiAnimationID) PURE;
	virtual void			Animator_GameObject(const _float& fTimeDelta) PURE;
	virtual void			Change_State(Animation::MONSTER State)PURE;
	virtual _int			Update_Pattern(_float fTimeDelta)PURE;


protected:
	void					Render_HitCollider(void);
	void					Render_BoxCollider(void);

protected:
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CTransform*		m_pTargetTransformCom = nullptr;
	const _tchar*			m_pLayerTag;
	wstring					m_wstrObjName;
	_matrix					m_matBill;
	_matrix					m_matWorld;
	_matrix					m_matView;
	FRAME					m_tFrame;


	_vec3					m_vTargetPos;
	_vec3					m_vPos;
	_vec3					m_vDir;
	_float					m_fSpeed;
	_float					m_fScale;
	_float					m_fDetectRange;
	_float					m_fAttackRange;
	_float					m_fTargetLength;
	_bool					m_bAttack;
	_bool					m_bShoot;
	_bool					m_bShootEnd;
	_bool					m_bHit;
	_bool					m_bDeath;

	_uint					m_uiSoundFlag;// 1.Detect 2.Death 4.Attack1 8.Attack2 ....

	Animation::MONSTER		m_eCurState;
	Animation::MONSTER		m_ePrevState;
	OBJ_DATA				m_tData;

	SPHERECOLLIDER			m_tHitColl;
	BOXCOLLIDER				m_tCollider;



protected:
	virtual void Free(void) override;

	// Test Code
public:
	void Damage(void)
	{
		m_tData.fCurHP -= 50.f;
	}
};

#endif // Monster_h__
