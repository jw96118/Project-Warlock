#include "Texture.h"

USING(Engine)

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
{

}

Engine::CTexture::CTexture(const CTexture& rhs)
	: CResources(rhs)
{
	_uint	iContainerSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iContainerSize);

	m_vecTexture = rhs.m_vecTexture;
	m_vecImageInfo = rhs.m_vecImageInfo;

	for (_uint i = 0; i < iContainerSize; ++i)
		m_vecTexture[i]->AddRef();
	m_wContainerIdx = rhs.m_wContainerIdx;
	ZeroMemory(m_pResourceTag, 20 * sizeof(_tchar));
	lstrcpy(m_pResourceTag, rhs.m_pResourceTag);
}

Engine::CTexture::~CTexture(void)
{

}

const D3DXIMAGE_INFO & CTexture::Get_ImageInfo(const _uint& iIndex /* = 0*/) const
{
	if (iIndex < 0 || iIndex >= m_vecImageInfo.size())
		return m_vecImageInfo[0];

	return m_vecImageInfo[iIndex];
}

const _uint CTexture::Get_TextureFrameSize(void) const
{
	return m_vecTexture.size();
}

HRESULT Engine::CTexture::Ready_Texture(const _tchar* pPath, 
										TEXTURETYPE eType, 
										const _uint& iCnt /*= 1*/)
{
	m_vecTexture.reserve(iCnt);
	m_vecImageInfo.reserve(iCnt);

	D3DXIMAGE_INFO	tImgInfo; // �̹��� ������ �����ϱ� ���� ����ü ����.
	ZeroMemory(&tImgInfo, sizeof(D3DXIMAGE_INFO));
	
	IDirect3DBaseTexture9*		pTexture = nullptr;

	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR		szFileName[256] = L"";
		wsprintf(szFileName, pPath, i);

		// �̹��� ���� ������ �Լ�.
		FAILED_CHECK_RETURN_MSG(D3DXGetImageInfoFromFile(szFileName, &tImgInfo), E_FAIL, L"D3DXGetImageInfoFromFile Failed");

		switch (eType)
		{
		case TEX_NORMAL:
			//FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			
			FAILED_CHECK_RETURN(
				D3DXCreateTextureFromFileEx(
					m_pGraphicDev,			
					szFileName,				
					tImgInfo.Width,			
					tImgInfo.Height,		
					tImgInfo.MipLevels,
					0,
					tImgInfo.Format,
					D3DPOOL_MANAGED,		
					D3DX_DEFAULT,
					D3DX_DEFAULT,
					0,
					nullptr,
					nullptr,
					(LPDIRECT3DTEXTURE9*)&pTexture),
				E_FAIL);	

			break;

		case TEX_CUBE:
			//FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			
			FAILED_CHECK_RETURN(
				D3DXCreateCubeTextureFromFileEx(
					m_pGraphicDev,			
					szFileName,				
					D3DX_DEFAULT,
					tImgInfo.MipLevels,
					0,
					tImgInfo.Format,
					D3DPOOL_MANAGED,		
					D3DX_DEFAULT,
					D3DX_DEFAULT,
					0,
					nullptr,
					nullptr,
					(LPDIRECT3DCUBETEXTURE9*)&pTexture),
				E_FAIL);

			break;
		}

		m_vecImageInfo.push_back(tImgInfo);
		m_vecTexture.push_back(pTexture);
	}

	return S_OK;
}

void Engine::CTexture::Render_Texture(const _uint& iIndex /*= 0*/)
{
	if (m_vecTexture.size() < iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
}

IDirect3DBaseTexture9 * CTexture::Get_Texture(const _uint & iIndex)
{
	return m_vecTexture[iIndex];
}

const int & CTexture::GetVecSize() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_vecTexture.size();
}



Engine::CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt /*= 1*/)
{
	CTexture*		pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Texture(pPath, eType, iCnt)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CResources* Engine::CTexture::Clone(void)
{
	return new CTexture(*this);
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTexture* pInstance = new CTexture(pGraphicDev);
	return pInstance;
}

void Engine::CTexture::Free(void)
{
	for_each(m_vecTexture.begin(), m_vecTexture.end(), CDeleteObj());
	m_vecTexture.clear();
	m_vecImageInfo.clear();

	CResources::Free();
}

