#ifndef Terrain_h__
#define Terrain_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTerrainTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex);
	virtual ~CTerrain(void);

public:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;

private:
	HRESULT					Add_Component(void);

private:
	Engine::CTerrainTex*	m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	wstring					m_wstrTexture = L"";
	_vec3					m_vDir;
	_float					m_fSpeed;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex);

private:
	virtual void			Free(void) override;
};

#endif // Terrain_h__
