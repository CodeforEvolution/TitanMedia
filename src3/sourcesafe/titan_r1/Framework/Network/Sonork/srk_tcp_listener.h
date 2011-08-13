#if !defined(SONORK_TCP_LISTENER_H)
#define SONORK_TCP_LISTENER_H

#include "srk_defs.h"

#include "srk_thread.h"
#include "srk_tcpio.h"

enum SONORK_TCPL_EVENT
{
	SONORK_TCPL_EVENT_NONE
,	SONORK_TCPL_EVENT_ACCEPT

};

#define SONORK_TCPL_ECB_PROCESSED			1
#define SONORK_TCPL_ECB_NOT_PROCESSED		0

typedef int (SONORK_CALLBACK fnTcpListenerEventCB)(void *param,SOCKET,UINT,void*);
typedef 	fnTcpListenerEventCB* pfnTcpListenerEventCB;

class TSonorkTcpListener
:public TSonorkThread
{
	TSonorkTcpEngine	net_io;
	UINT				select_msecs;
	struct {
		void*					param;
		pfnTcpListenerEventCB   event;
	}CB;
	int Run();

public:
	TSonorkTcpListener(void*param,pfnTcpListenerEventCB CB,UINT select_msecs);

	int	Listen( DWORD addr, WORD port );
};

#endif
