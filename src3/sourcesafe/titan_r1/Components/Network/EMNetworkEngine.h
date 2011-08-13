/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* Martin Johansson, 01-08-01
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_NETWORK_ENGINE
#define __EM_NETWORK_ENGINE

#include <windows.h>

#include "EMListenerRepository.h"
#include "EMNetworkMessageQueue.h"

#include "EMNetworkGlobals.h"

class EMNetworkUserDatabase;
class EMNetworkCommandFactory;
class EMNetworkMessageSender;
class EMNetworkMessage;
class EMNetworkFileHandler;
class EMNetworkUserHandler;
class EMSemaphore;

struct TSonorkApi;
struct TSonorkError;
struct SONORK_DWORD2;

enum SONORK_NETIO_STATUS;
enum SONORK_FUNCTION;

class __declspec(dllexport) EMNetworkEngine : public EMListenerRepository
{
public:
	~EMNetworkEngine();

	static EMNetworkEngine* Instance();
	static void Delete();

	bool Initialize(const char* p_opHost = "192.168.0.100", uint32 m_vPort = 1504 , bool p_vTCP = true , bool p_vIntranet = true);

	bool Connect(const char* p_opUID = NULL, const char* p_opPWD = NULL);
	bool Disconnect();
	bool IsConnected();

	EMNetworkUserDatabase* GetUserDatabase();
	EMNetworkMessageSender* GetMessageSender();
	EMNetworkMessageQueue<EMNetworkMessage*>* GetMessageQueue();
	EMNetworkFileHandler* GetFileHandler();
	EMNetworkUserHandler* GetUserHandler();

	EMNetworkCommandFactory* GetCommandFactory();

	TSonorkApi* GetAPI();

	static void MainAPICallback(TSonorkApi* p_opAPI);
	static void __cdecl TimerThread(void *p_vpThis);

protected:
	void PhaseTransition();
	void ClientEvent();

	void StatusChanged(SONORK_NETIO_STATUS p_oNewStatus, SONORK_NETIO_STATUS p_oOldStatus, DWORD p_vFlags, TSonorkError& p_vError);
	void GlobalTask(SONORK_FUNCTION p_oTask,bool p_vDone, TSonorkError& p_oError);
	void AddUserFromNetwork();
	void MessageReceived(const SONORK_DWORD2& userId);
	void SidChanged(const SONORK_DWORD2& userId);
	void DeleteUser(const SONORK_DWORD2& p_oReqUserId);
	void AuthorizeUser(const SONORK_DWORD2& p_oReqUserId);

protected:
	TSonorkApi*				m_opAPI;

	EMNetworkUserDatabase*	m_opUsers;
	EMNetworkMessageSender* m_opSender;
	EMNetworkMessageQueue<EMNetworkMessage*>* m_opMessageQueue;
	EMNetworkFileHandler*	m_opFileHandler;
	EMNetworkUserHandler*   m_opUserHandler;

	HANDLE m_oThreadEnd;
	HANDLE m_oUsersDownloaded;

	bool m_vIsRunning;

	EMSemaphore* m_opLock;

	EMPhase m_oCurrentPhase;
	EMPhase m_oLastPhase;
private:
	EMNetworkEngine();

private:
	static EMNetworkEngine* m_opInstance;
	static bool m_vIsDecomposed;
};

#endif
