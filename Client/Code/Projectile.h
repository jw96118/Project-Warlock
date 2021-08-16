#ifndef PROJECTILE_H__
#define PROJECTILE_H__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CProjectile : public Engine::CGameObject
{
public:
	static enum PROJECTILE_PATTERN { PROJECTILE_STRAIGHT, PROJECTILE_TRACKING, PROJECTILE_SINE, PRJECTILE_TWISTER };
public:
	CProjectile(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, _vec3 vPos, _vec3 vDir, _float fSpeed, _float fScale, _float fDamage, PROJECTILE_PATTERN ePattern, D3DXCOLOR lightColor, _float fIntaval);
	~CProjectile();
public:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	void		SetScale(_float fScale = 1.0f);
	void		Update_Pattern(_vec3 vToTarget, _float fLength, _float fTimeDelta);
	void		Set_Intaval(_float fIntaval);
private:
	Engine::CTransform*		m_pTargetTransformCom = nullptr;
	D3DLIGHT9				m_tLightInfo;
	PROJECTILE_PATTERN		m_ePattern;

	_matrix					m_matBill;
	_matrix					m_matWorld;
	_matrix					m_matView;
	FRAME					m_tFrame;
	wstring					m_wstrTex;
	_vec3					m_vPos;
	_vec3					m_vDir;
	wstring					m_wstrObjName;
	_float					m_fTimer;
	_float					m_fScale;
	_float					m_fSpeed;
	_bool					m_bIsTraking;
	_float					m_fDamage;
	_float					m_fPatternVal;
	_float					m_fIntaval;

public:
	static CProjectile* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, _vec3 vPos, _vec3 vDir, _float fSpeed = 1.0f, _float fScale = 1.0f, _float fDamage = 10.f, PROJECTILE_PATTERN ePattern = PROJECTILE_STRAIGHT, D3DXCOLOR lightColor = (1.0f, 1.0f, 1.0f, 1.0f), _float fIntaval = 1.f);

protected:
	virtual void Free(void) override;
};


#endif // !PROJECTILE_H__


