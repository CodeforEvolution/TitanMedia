#ifndef __EM_WIN_NODE_CONNECTION
#define __EM_WIN_NODE_CONNECTION

#include "EMWinMediaInputFilterInterface.h"
//#include "EMWinMediaOutputFilterInterface.h"
//#include "EMWinVideoVirtualRendererFilter.h"
#include <streams.h>

class EMWinNodeConnection
{
public:
	EMWinNodeConnection() 
	{ 
		m_opGraphBuilder = NULL;
		m_opSourceFilter = NULL; 
		m_opTargetFilter = NULL; 
		m_opOutputPin = NULL; 
		m_opInputPin = NULL; 
		m_opTargetFilterInterface = NULL; 
//		m_opTargetVideoFilterInterface = NULL;
//		m_opSourceFilterInterface = NULL; 
		m_opControl = NULL;
	};
	~EMWinNodeConnection() {};
	IGraphBuilder* m_opGraphBuilder;
	EMWinMediaInputFilterInterface* m_opTargetFilterInterface;
//	EMWinVideoVirtualRendererFilter* m_opTargetVideoFilterInterface;
//	EMWinMediaOutputFilterInterface* m_opSourceFilterInterface;
	IBaseFilter* m_opSourceFilter;
	IBaseFilter* m_opTargetFilter;
	IPin* m_opOutputPin;
	IPin* m_opInputPin;
	IMediaControl* m_opControl;
};

#endif