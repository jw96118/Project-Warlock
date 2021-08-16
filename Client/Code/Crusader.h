#ifndef Crusader_h__
#define Crusader_h__
#include "Monster.h"
#include "Defines.h"
#include "Monster.h"

class CCrusader :
	public CMonster
{
public:
	explicit CCrusader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName, _vec3 vPos);
	virtual  ~CCrusader(void);
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
	virtual _int			Update_Pattern(_float fTimeDelta)override;
	void					Update_HitCollider()override;
	void					Update_BoxCollider()override;
private:
	_int					Idle(_float fTimeDelta);
	_int					Back(_float fTimeDelta);
	_int					Hit(_float fTimeDelta, _float fDamage);
	_int					Attack(_float fTimeDelta);
	_int					Death(_float fTimeDelta);
private:

private:
	_int					m_iPtCnt;
	wstring					m_wstrProjectile;
	_int*                   m_pEventCount;
private:
	Engine::CTexture*		m_pTextureCom[Animation::MON_END];
public:
	static CCrusader*			Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName, _vec3 vPos = { 0,0,0 }, _int* pEventCount = nullptr);

private:
	virtual void			Free(void) override;
};

#endif //Crusader_h__