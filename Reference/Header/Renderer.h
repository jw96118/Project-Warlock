#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer(void);
	virtual ~CRenderer(void);

public:
	void		Set_Perspective(const _matrix& matPerspective);
	 
public:
	void		Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject);
	void		Render_GameObject(void);
	void		Clear_RenderGroup(void);

private:
	void		RenderState_Projection(PROJECTIONTYPE eProjType);

private:
	list<CGameObject*>		m_RenderGroup[RENDER_END];
	_matrix					m_matOrthograph, m_matPerspective;

private:
	virtual void	Free(void);

};

END
#endif // Renderer_h__
