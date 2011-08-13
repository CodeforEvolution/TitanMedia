

#include "EMWinGDIPlusImageLoaderSuper.h"

#include "EMWinGDIPlusImageLoader.h"

#include "EMMediaGlobals.h"

EMWinGDIPlusImageLoaderSuper* EMWinGDIPlusImageLoaderSuper::m_opGDIPlusImageLoaderSuper = NULL;

EMWinGDIPlusImageLoaderSuper::EMWinGDIPlusImageLoaderSuper()
{
	m_opGDIPlusImageLoader = EMWinGDIPlusImageLoader::Instance();
}

EMWinGDIPlusImageLoaderSuper::~EMWinGDIPlusImageLoaderSuper()
{
}

EMWinGDIPlusImageLoaderSuper* EMWinGDIPlusImageLoaderSuper::Instance()
{
	if(m_opGDIPlusImageLoaderSuper != NULL)
		return m_opGDIPlusImageLoaderSuper;

	if(m_opGDIPlusImageLoaderSuper == NULL)
		m_opGDIPlusImageLoaderSuper = new EMWinGDIPlusImageLoaderSuper();

	return m_opGDIPlusImageLoaderSuper;
}

void EMWinGDIPlusImageLoaderSuper::PrepareImageFile(unsigned short* p_pFileName)
{
	m_opGDIPlusImageLoader -> PrepareImageFile(p_pFileName);
}

int EMWinGDIPlusImageLoaderSuper::GetWidth()
{
	return m_opGDIPlusImageLoader -> GetWidth();
}

int EMWinGDIPlusImageLoaderSuper::GetHeight()
{
	return m_opGDIPlusImageLoader -> GetHeight();
}

int EMWinGDIPlusImageLoaderSuper::GetBytesPerRow()
{
	return m_opGDIPlusImageLoader -> GetBytesPerRow();
}

void EMWinGDIPlusImageLoaderSuper::CopyImageIntoBuffer(unsigned char* p_opBuffer)
{
	m_opGDIPlusImageLoader -> CopyImageIntoBuffer(p_opBuffer);
}

void EMWinGDIPlusImageLoaderSuper::CloseImageFile()
{
	m_opGDIPlusImageLoader->CloseImageFile();
}