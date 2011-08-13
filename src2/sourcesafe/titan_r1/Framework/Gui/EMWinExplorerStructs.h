#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_EXPLORER_STRUCTS
#define __EM_WIN_EXPLORER_STRUCTS

typedef struct EMWinListViewItemData
{
	LPSHELLFOLDER	m_spParentShellFolder;
	LPITEMIDLIST	m_spItemIdList;
	char			m_vPath[EM_MAX_STRING_BUFFER_SIZE];
	ULONG			m_vAttributes;
}LVITEMDATA, *LPLVITEMDATA;

typedef struct EMWinTreeViewItemData
{
	LPSHELLFOLDER	m_spParentShellFolder;
	LPITEMIDLIST	m_spItemIdList;
	LPITEMIDLIST	m_spItemIdListSecond;
}TVITEMDATA, *LPTVITEMDATA;

#endif

#endif