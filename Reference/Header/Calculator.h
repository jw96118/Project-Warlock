#ifndef Calculator_h__
#define Calculator_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCalculator : public CComponent
{
private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCalculator(void);

public:
	HRESULT		Ready_Calculator(void);

	_float		Compute_HeightOnTerrain(const _vec3* pPos, 
										const _vec3* pTerrainVtxPos,
										const _ulong& dwCntX,
										const _ulong& dwCntZ);

	_float		Compute_HeightOnTerrain1(_vec3* pPos,
										const _vec3* pTerrainVtxPos,
										const _ulong& dwCntX,
										const _ulong& dwCntZ);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;

public:
	static CCalculator*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void				Free(void);

};

END
#endif // Calculator_h__
