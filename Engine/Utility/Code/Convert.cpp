#include "Convert.h"

USING(Engine)

Engine::CConvert::CConvert(void)
{

}

Engine::CConvert::~CConvert(void)
{

}

void CConvert::D3DToClientCoordinate2D(const _float& fWinCX, const _float& fWinCY, _float & fX, _float & fY)
{
	// ��ī��Ʈ ��ǥ��(D3D) -> ������ ��ǥ��(Ŭ���̾�Ʈ ���� ����) ��ȯ
	// x��ǥ = x +  ������ ���� ũ�� * 0.5
	// y��ǥ = (-y) + ������ ���� ũ�� * 0.5;

	fX = fX + fWinCX * 0.5f;
	fY = (-fY) + fWinCY * 0.5f;
}

void CConvert::ClientToD3DCoordinate2D(const _float& fWinCX, const _float& fWinCY, _float & fX, _float & fY)
{	
	// ��ī��Ʈ ��ǥ��(D3D) <- ������ ��ǥ��(Ŭ���̾�Ʈ ���� ����) ��ȯ
	// x��ǥ = x - ������ ���� ũ�� * 0.5
	// y��ǥ = (-y) + ������ ���� ũ�� * 0.5;

	fX = fX - fWinCX * 0.5f;
	fY = (-fY) + fWinCY * 0.5f;
}
