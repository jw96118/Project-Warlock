#ifndef TriCol_h__
#define TriCol_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriCol : public CVIBuffer
{
private:
	explicit CTriCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTriCol(const CTriCol& rhs);
	virtual ~CTriCol(void);

public:
	virtual HRESULT		Ready_Buffer(void) override;

public:
	static CTriCol*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResources* Clone(void) override;

private:
	virtual void		Free(void);
};

END
#endif // TriCol_h__
