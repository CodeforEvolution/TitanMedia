#ifndef __EM_WIN_DX_CALLBACK_INTERFACE
#define __EM_WIN_DX_CALLBACK_INTERFACE

#include "EMMediaEngineUIDs.h"

class EMCallback
{
public:
	EMCallback() {};
	~EMCallback() {};

	virtual void Callback(int64 p_vStart, int64 p_vStop) = 0;
};


DECLARE_INTERFACE_(EMWinDXCallbackInterface, IUnknown) 
{ 

	STDMETHOD(SetCallback) (THIS_ EMCallback* p_opCallback) PURE;
};

#endif
