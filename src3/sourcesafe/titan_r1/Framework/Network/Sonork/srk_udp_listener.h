#if !defined(SONORK_UDP_LISTENER_H)
#define SONORK_UDP_LISTENER_H
#include "srk_thread.h"
#include "srk_udpio.h"

#define SONORK_UDPL_DCB_PROCESSED			1
#define SONORK_UDPL_DCB_NOT_PROCESSED		0

typedef void (SONORK_CALLBACK fnUdpListenerDataCB)(void *param,sockaddr_in*,void*data,UINT size);
typedef 	fnUdpListenerDataCB* pfnUdpListenerDataCB;

class TSonorkUdpListener
:public TSonorkThread
{
	TSonorkUdpEngine*	net_io;
	UINT			select_msecs;
	struct {
		void*					param;
		pfnUdpListenerDataCB	data;
	}CB;
	int	Run();

public:
	TSonorkUdpListener(  TSonorkUdpEngine*			net_io
						, void*						cb_param
						, pfnUdpListenerDataCB		cb_data
						, UINT						select_msecs);
	int		Listen( DWORD host , WORD port );
	int		SetSocketBufferSize(long send_size,long recv_size);

};

#endif