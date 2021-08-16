#ifndef Effect_h__
#define Effect_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRenderer;
class CTexture;
class CRcTex;

END

class CEffectMgr;

class CEffect : public Engine::CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect(void);

protected:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;

	virtual	HRESULT			Add_Component(void);
	virtual void			Update_Collider(void) PURE;


protected:
	CEffectMgr*				m_pEffectMgr;

	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CRenderer*		m_pRendererCom;

	_vec3					m_vMoveDir;
	SPHERECOLLIDER			m_tCollider;

	_matrix					m_matWorld;
	_matrix					m_matScale;
	_matrix					m_matRotX;
	_matrix					m_matRotY;
	_matrix					m_matCamRotY;
	_matrix					m_matCamRotX;
	_matrix					m_matTrans;
	_vec3					m_vPos;

protected:
	virtual void			Free(void) override;
};

#endif // Effect_h__
