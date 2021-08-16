#ifndef HellBoss_h__
#define HellBoss_h__
#include "Monster.h"
#include "Defines.h"

class CBossGauge;

class CHellBoss :
	public CMonster
{
public:
	explicit CHellBoss(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName, _vec3 vPos);
	virtual  ~CHellBoss(void);
public:
	static enum HB_Pattern {
		DWARF_IDLE, DWARF_WALK, DWARF_SHOOT, DWARF_EYEBLAST, DWARF_MORPH,
		BIG_WALK, BIG_ATTACK_START, BIG_ATTACK_SPIN, BIG_ATTACK_FIRE, BIG_ATTACK_END,
		BIG_FIRST_HIT, BIG_FIRST_WALK, BIG_FIRST_ATTACK, BIG_MORPH,
		CACO_WALK, CACO_NOVA, CACO_EYELASER, CACO_ATTACK, CACO_MORPH,
		FINAL_IDLE, FINAL_SPIKE1, FINAL_SPIKE2, FINAL_TENTACLE, FINAL_DEATH, HB_END
	};


public:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	virtual HRESULT			LateReady_GameObject(void)override;

private:
	virtual	HRESULT			Add_Component(void);
	virtual void			Change_Animation(_uint uiAnimationID)override;
	virtual void			Animator_GameObject(const _float& fTimeDelta) override;
	virtual void			Change_State(Animation::MONSTER State)override;
	void					Change_Pattern(HB_Pattern ePattern);
	virtual _int			Update_Pattern(_float fTimeDelta)override;
	void					Update_HitCollider()override;
	void					Update_BoxCollider()override;

private:
	void					Dwarf_Pattern(_float fTimeDelta);
	void					Dwaf_Idle(_float fTimeDelta);
	void					Dwaf_Walk(_float fTimeDelta);
	void					Dwaf_Shoot(_float fTimeDelta);
	void					Dwaf_EyeBlast(_float fTimeDelta);
	void					Dwaf_Morph(_float fTimeDelta);

	void					Big_Pattern(_float fTimeDelta);
	void					Big_Walk(_float fTimeDelta);
	void					Big_Attack_Start(_float fTimeDelta);
	void					Big_Attack_Spin(_float fTimeDelta);
	void					Big_Attack_Fire(_float fTimeDelta);
	void					Big_Attack_End(_float fTimeDelta);

	void					Big_First_Pattern(_float fTimeDelta);
	void					Big_First_Hit(_float fTimeDelta);
	void					Big_First_Walk(_float fTimeDelta);
	void					Big_First_Attack(_float fTimeDelta);
	void					Big_Morph(_float fTimeDelta);

	void					Caco_Pattern(_float fTimeDelta);
	void					Caco_Walk(_float fTimeDelta);
	void					Caco_Nova(_float fTimeDelta);
	void					Caco_EyeLaser(_float fTimeDelta);
	void					Caco_Attack(_float fTimeDelta);
	void					Caco_Morph(_float fTimeDelta);

	void					Final_Pattern(_float fTimeDelta);
	void					Final_Idle(_float fTimeDelta);
	void					Final_Spike1(_float fTimeDelta);
	void					Final_Spike2(_float fTimeDelta);
	void					Final_Tentacle(_float fTimeDelta);
	void					Final_Death(_float fTimeDelta);

private:
	CBossGauge*				m_pGuage = nullptr;
	_int					m_iPtCnt;
	wstring					m_wstrProjectile;
	CHellBoss::HB_Pattern	m_eCurPattern;
	CHellBoss::HB_Pattern	m_ePrevPattern;
	_int					m_iMorphFlag;
	_int					m_iPatternNum;

	_int					m_iBigAttackNum;
private:
	Engine::CTexture*		m_pTextureCom[HB_END];
public:
	static CHellBoss*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName, _vec3 vPos = { 0,0,0 });

private:
	virtual void			Free(void) override;



};

#endif //HellBoss_h__