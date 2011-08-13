/*******************************************************
* Creator: Jesper Svensson
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/


#ifndef __EM_WIN_GDI_PLUS_IMAGE_LOADER_SUPER
#define __EM_WIN_GDI_PLUS_IMAGE_LOADER_SUPER

class EMWinGDIPlusImageLoader;

class EMWinGDIPlusImageLoaderSuper
{
public:
	EMWinGDIPlusImageLoaderSuper();
	~EMWinGDIPlusImageLoaderSuper();
	static EMWinGDIPlusImageLoaderSuper* Instance();
	void PrepareImageFile(unsigned short* p_pFileName);
	int GetWidth();
	int GetHeight();
	int GetBytesPerRow();
	void CopyImageIntoBuffer(unsigned char* p_opBuffer);
	void CloseImageFile();

private:
	static EMWinGDIPlusImageLoaderSuper* m_opGDIPlusImageLoaderSuper;
	EMWinGDIPlusImageLoader* m_opGDIPlusImageLoader;

};

#endif