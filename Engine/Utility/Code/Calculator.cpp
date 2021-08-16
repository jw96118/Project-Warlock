#include "Calculator.h"

USING(Engine)

Engine::CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CCalculator::~CCalculator(void)
{

}

HRESULT Engine::CCalculator::Ready_Calculator(void)
{
	return S_OK;
}

_float Engine::CCalculator::Compute_HeightOnTerrain(const _vec3* pPos,
													const _vec3* pTerrainVtxPos, 
													const _ulong& dwCntX, 
													const _ulong& dwCntZ)
{
	// Ÿ�� ITV = 1.f
	_ulong	dwIndex = _ulong(pPos->z / 1.f) * dwCntX + _ulong(pPos->x / 1.f);

	_float	fRatioX = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / 1.f;
	_float	fRatioZ = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / 1.f;

	_float	fHeight[4] = { 
		pTerrainVtxPos[dwIndex + dwCntX].y,
		pTerrainVtxPos[dwIndex + dwCntX + 1].y,
		pTerrainVtxPos[dwIndex + 1].y,
		pTerrainVtxPos[dwIndex].y
	};

	// ������ �� ���
	if (fRatioX > fRatioZ)
	{
		return fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}
	// ���� �Ʒ� ���
	else
	{
		return fHeight[0] + (fHeight[2] - fHeight[3]) * fRatioX + (fHeight[3] - fHeight[0]) * fRatioZ;
	}
	
}

_float Engine::CCalculator::Compute_HeightOnTerrain1(_vec3* pPos, 
													const _vec3* pTerrainVtxPos,
													const _ulong& dwCntX,
													const _ulong& dwCntZ)
{
	// Ÿ�� ITV = 1.f
	_ulong	dwIndex = _ulong(pPos->z / 1.f) * dwCntX + _ulong(pPos->x / 1.f);

	_float	fRatioX = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / 1.f;
	_float	fRatioZ = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / 1.f;

	D3DXPLANE		Plane;

	// ������ �� ���
	if (fRatioX > fRatioZ)
	{
		// ������ ��ġ �� �� ���� ���� ����� �������ִ� �Լ�
		D3DXPlaneFromPoints(&Plane, 
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + dwCntX + 1],
			&pTerrainVtxPos[dwIndex + 1]);
	}
	// ���� �Ʒ� ���
	else
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + 1],
			&pTerrainVtxPos[dwIndex]);
	}

	// ax + by + cz + d =>  y = (-ax - cz -d ) / b
	
	pPos->y = (-Plane.a * pPos->x - Plane.c * pPos->z - Plane.d) / Plane.b;
	pPos->y += 1.f;

	return pPos->y;
}

Engine::CCalculator* Engine::CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator*	pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Calculator()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCalculator::Free(void)
{
	Safe_Release(m_pGraphicDev);
}

