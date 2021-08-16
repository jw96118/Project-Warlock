#ifndef FirstPersonCamera_h__
#define FirstPersonCamera_h__

#include "Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;

END
class CPlayer;
class CShop;

class CFirstPersonCamera : public Engine::CCamera
{
private:
	explicit CFirstPersonCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFirstPersonCamera(void);

public:
	const _vec3& Get_Eye() const;
	const _vec3& Get_At() const;
	const _vec3& Get_MoveDir() const;
	const _vec3& Get_LookDir() const;
	const _float& Get_RotationX() const;
	const _float& Get_RotationY() const;

public:
	HRESULT						Ready_GameObject(const _vec3* pEye,
												 const _vec3* pAt,
												 const _vec3* pUp,
												 const _float& fFovY,
												 const _float& fAspect,
												 const _float& fNear,
												 const _float& fFar,
												 const _bool& bMainCamera);
	HRESULT						LateReady_GameObject() override;
	virtual	_int				Update_GameObject(const _float& fTimeDelta) override;

	void						Move_View(const _vec3& vMove);

private:
	void						Key_Input(const _float& fTimeDelta);
	void						Mouse_Move(const _float& fTimeDelta);

private:
	//Engine::CTransform*			m_pPlayerTransformCom;
	CPlayer*                    m_pPlayerObj;
	_vec3						m_vMoveDir;
	_vec3						m_vLookDir;
	_float						m_fRotationX;
	_float						m_fRotationY;

	//Α¶Έν
	D3DLIGHT9					m_tLightInfo;
	_float						m_fLightVal;
	_float						m_fToggleVal;

	CShop*						m_pShop;

public:
	static CFirstPersonCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFovY,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar,
		const _bool& bMainCamera);

private:
	virtual void				Free(void) override;

};

#endif // FirstPersonCamera_h__
