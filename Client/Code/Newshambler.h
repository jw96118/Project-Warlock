#ifndef Newshambler_h__
#define Newshambler_h__
#include "Monster.h"
#include "Defines.h"

class CBossGauge;
class CNewshambler :
	public CMonster
{
public:
	explicit CNewshambler(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName, _vec3 vPos);
	virtual  ~CNewshambler(void);
public:
	static enum NS_Pattern { NS_IDLE, NS_BASH, NS_MAGIC, NS_SLASH, NS_FIRST_HIT, NS_FIRST_WALK, NS_FIRST_CAST, NS_FIRST_SLASH, NS_SECOND_CHOMP, NS_SECOND_HIT, NS_SECONDWALK, NS_WALK, NS_DEATH, NS_PATTERN_END };

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
	void					Change_Pattern(NS_Pattern ePattern);
	virtual _int			Update_Pattern(_float fTimeDelta)override;
	void					Update_HitCollider()override;
	void					Update_BoxCollider()override;

private:
	_int					Bash(_float fTimeDelta);
	_int					Magic(_float fTimeDelta);
	_int					Slash(_float fTimeDelta);
	_int					Walk(_float fTimeDelta);
	_int					First_Cast(_float fTimeDelta);
	_int					First_Hit(_float fTimeDelta);
	_int					First_Slash(_float fTimeDelta);
	_int					First_Walk(_float fTimeDelta);
	_int					Second_Chomp(_float fTimeDelta);
	_int					Second_Hit(_float fTimeDelta);
	_int					Second_Walk(_float fTimeDelta);
	_int					Death(_float fTimeDelta);
	_int					Phase1(_float fTimeDelta);
	_int					Phase2(_float fTimeDelta);
	_int					Phase3(_float fTimeDelta);


private:
	CBossGauge*					m_pGuage = nullptr;
	_int						m_iPtCnt;
	wstring						m_wstrProjectile;
	CNewshambler::NS_Pattern	m_eCurPattern;
	CNewshambler::NS_Pattern	m_ePrevPattern;
	_int						m_iMorphFlag;
	_int						m_iPatternNum;
private:
	Engine::CTexture*		m_pTextureCom[NS_PATTERN_END];
public:
	static CNewshambler*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName, _vec3 vPos = { 0,0,0 });

private:
	virtual void			Free(void) override;



};

#endif //Newshambler_h__