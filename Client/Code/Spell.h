#ifndef SPELL_H__
#define SPELL_H__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CSpell : public Engine::CGameObject
{
public:
	CSpell(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, _vec3 vPos,_float fScale);
	~CSpell();
public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	void		SetScale(_float fScale = 1.0f);


private:
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
public:
	static CSpell* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, _vec3 vPos,_float fScale = 1.0f);

protected:
	virtual void Free(void) override;
};


#endif // !SPELL_H__


