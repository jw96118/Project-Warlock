#include "VIBuffer.h"

USING(Engine)

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwVtxFVF(0)
	, m_dwTriCnt(0)
	, m_dwIdxSize(0)
{

}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CResources(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwVtxFVF(rhs.m_dwVtxFVF)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwIdxSize(rhs.m_dwIdxSize)

{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

Engine::CVIBuffer::~CVIBuffer(void)
{

}

void CVIBuffer::SendToClient(void * pVertex)
{
	void*	pOriVtx = nullptr;

	m_pVB->Lock(0, 0, &pOriVtx, 0);

	memcpy(pVertex, pOriVtx,m_dwVtxSize * m_dwVtxCnt);

	m_pVB->Unlock();
}

void CVIBuffer::ReceiveToEngine(void * pVertex)
{
	void*	pOriVtx = nullptr;

	m_pVB->Lock(0, 0, &pOriVtx, 0);

	memcpy(pOriVtx, pVertex, m_dwVtxSize * m_dwVtxCnt);

	m_pVB->Unlock();
}

void CVIBuffer::SendToClient(_float fL, _float fR, _float fT, _float fB)
{
	VTXTEX* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = _vec2(fL, fT);

	pVertex[1].vPos = _vec3(0.5f - (1 - fR), 0.5f, 0.f);
	pVertex[1].vTexUV = _vec2(fR, fT);

	pVertex[2].vPos = _vec3(0.5f - (1 - fR), -0.5f, 0.f);
	pVertex[2].vTexUV = _vec2(fR, fB);

	pVertex[3].vPos = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].vTexUV = _vec2(fL, fB);
	m_pVB->Unlock();
}

void CVIBuffer::SendToClient(_float fL)
{
	VTXTEX* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = _vec2(0.5f - fL, 0.f);

	pVertex[1].vPos = _vec3(fL, 0.5f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.0f);

	pVertex[2].vPos = _vec3(fL, -0.5f, 0.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.0f);

	pVertex[3].vPos = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].vTexUV = _vec2(0.5f - fL, 1.f);
	//
	pVertex[4].vPos = _vec3(fL, 0.5f, 0.f);
	pVertex[4].vTexUV = _vec2(0.f, 0.f);

	pVertex[5].vPos = _vec3(0.5f, 0.5f, 0.f);
	pVertex[5].vTexUV = _vec2(0.5f - fL, 0.0f);

	pVertex[6].vPos = _vec3(0.5f, -0.5f, 0.f);
	pVertex[6].vTexUV = _vec2(0.5f - fL, 1.0f);

	pVertex[7].vPos = _vec3(fL, -0.5f, 0.f);
	pVertex[7].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();
}

HRESULT Engine::CVIBuffer::Ready_Buffer(void)
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,	// ������ ������ ũ��
															0,					// ������ ����(���ڰ� 0�� ��� ���� ����)
															m_dwVtxFVF,			// �����ϴ� ���ؽ��� �Ӽ���
															D3DPOOL_MANAGED,	// �޸� Ǯ�� ����
															&m_pVB, 
															NULL), 
															E_FAIL);


	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,	// ������ �ε��� ������ ũ��
															0,					// ������ ����(���ڰ� 0�� ��� ���� ����)
															m_IdxFmt,			// �����ϴ� �ε����� �Ӽ���
															D3DPOOL_MANAGED,	// �޸� Ǯ�� ����
															&m_pIB,
															NULL),
															E_FAIL);

	

	return S_OK;
}

void Engine::CVIBuffer::Render_Buffer(void)
{
	// ����̽� ��ġ�� ���ؽ� ���۸� ��ũ�ϴ� �Լ�
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);	// 3���� : �ﰢ���� ����


}

void Engine::CVIBuffer::Free(void)
{
	CResources::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}

