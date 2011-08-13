/*******************************************************
* Creator: Martin Rudefelt
* Component: none
* Portability: non-native
*-------------------------------------------------------
* This file contains some global typedefs used by many
* parts of the system.
* The typedefs that should be supported by including
* this file are:
* int8, uint8, int16, uint16, int32, uint32, 
* int64, uint64, NULL, bool, true, false, (status_t?), 
* char, uchar
* It also containes #define:s that should be global and
* other useful stuff.
* All headers must include this file!
*******************************************************/

#ifndef __EM_GLOBALS
#define __EM_GLOBALS

/*************************************************************************/

#define EM_BIG_ENDIAN // Used by x86 architectures
//#define EM_LITTLE_ENDIAN // Used by PowerPC architectures

#define EM_DEBUG // If debug code should be used

//#define PLATFORM_BEOS // The current platform, only one platform can be defined
#define PLATFORM_WINDOWS
//#define PLATFORM_WINDOWS_2000
//#define PLATFORM_MAC_OS
//#define PLATFORM_MAC_OS_X
//#define PLATFORM_LINUX

/*************************************************************************/

#ifdef PLATFORM_BEOS
#include "SupportDefs.h"  // many nice BeOS typedefs
#endif

#ifdef PLATFORM_WINDOWS
#include "JDConsolePrint.h"
#endif

#include "EMNetworkDefine.h" // defines for the network

#pragma warning( disable : 4786 )

/*-------------------------------------------------------------*/
/*----- Shorthand type formats --------------------------------*/

typedef	signed char				int8;
typedef unsigned char			uint8;
typedef volatile signed char   	vint8;
typedef volatile unsigned char	vuint8;

typedef	short					int16;
typedef unsigned short			uint16;
typedef volatile short			vint16;
typedef volatile unsigned short	vuint16;

typedef	long					int32;
typedef unsigned long			uint32;
typedef volatile long			vint32;
typedef volatile unsigned long	vuint32;

typedef	__int64						int64;
typedef unsigned __int64			uint64;
typedef volatile __int64			vint64;
typedef volatile unsigned __int64	vuint64;

typedef volatile long			vlong;
typedef volatile int			vint;
typedef volatile short			vshort;
typedef volatile char			vchar;

typedef volatile unsigned long	vulong;
typedef volatile unsigned int	vuint;
typedef volatile unsigned short	vushort;
typedef volatile unsigned char	vuchar;

typedef unsigned char			uchar;
typedef unsigned short          unichar;



#include <new> // Needed for bad_alloc exception handling

#include <list>
using namespace std;

#ifdef EM_DEBUG
//#include <iostream.h> // Should not be declared anywhere else
#include <conio.h>
#ifdef PLATFORM_BEOS
#include <Debug.h>
#endif
#endif

//Prints out a text with pretty separators for error messages
#ifdef EM_DEBUG
#ifndef EM_OUT
#define EM_OUT
#include <string>
using namespace std;

/*class EMOut			//For "pretty" normal debug messages!
{
public:
	virtual ostream& operator<<(char* p) {return (cout << "   > " << p);};
	virtual ostream& operator<<(string* p) {return ((*this) << p -> c_str());};
	virtual ostream& operator<<(string p) {return ((*this) << p.c_str());};
	virtual ostream& operator<<(ostream& o) {return o;};
	virtual ostream& operator<<(signed int p) {return ((*this) << "" << p);};
	virtual ostream& operator<<(unsigned int p) {return ((*this) << "" << p);};
	virtual ostream& operator<<(signed long p) {return ((*this) << "" << p);};
	virtual ostream& operator<<(unsigned long p) {return ((*this) << "" << p);};
	virtual ostream& operator<<(double p) {return ((*this) << "" << p);};
	virtual ostream& operator<<(float p) {return ((*this) << "" << p);};
	virtual ostream& operator<<(char p) {return ((*this) << "" << p);};
};

class EMErr			//For error message/debug message. Prints two ! first so they're easily spotted!
{
public:
	virtual ostream& operator<<(char* p) {return (cerr << "!! > " << p);};
	virtual ostream& operator<<(string* p) {return ((*this) << p -> c_str());};
	virtual ostream& operator<<(string p) {return ((*this) << p.c_str());};
	virtual ostream& operator<<(ostream& o) {return o;};
	virtual ostream& operator<<(signed int p) {return ((*this) << "" << p);};
	virtual ostream& operator<<(unsigned int p) {return ((*this) << "" << p);};
	virtual ostream& operator<<(signed long p) {return ((*this) << "" << p);};
	virtual ostream& operator<<(unsigned long p) {return ((*this) << "" << p);};
	virtual ostream& operator<<(double p) {return ((*this) << "" << p);};
	virtual ostream& operator<<(float p) {return ((*this) << "" << p);};
	virtual ostream& operator<<(char p) {return ((*this) << "" << p);};
};
*/
#endif

#ifndef EM_OUT_OBJECTS
#define EM_OUT_OBJECTS
//static EMOut emout;
//static EMErr emerr;
#endif
#endif

//#ifdef EM_DEBUG
/*#ifdef PLATFORM_BEOS
//Enters debugger and prints out a message (if supported by the platform)
inline void EMDebugger(const char* p_vpMessage)
{

	emerr << (char*) p_vpMessage << endl;
	debugger(p_vpMessage);
/*#ifndef PLATFORM_BEOS
	cout << p_vpMessage << endl;
	exit(0);
#endif
}
#endif //PLATFORM_BEOS
*/
//#endif

//#ifdef PLATFORM_WINDOWS
#include <stdarg.h>
inline void EMDebugger(const char *vpMessage, ...)
{
#ifdef EM_DEBUG

	va_list args;
	
	va_start(args, vpMessage);
	vfprintf(stderr, vpMessage, args);
	va_end(args);
#endif

}
//#endif //PLATFORM_WINDOWS

//&#endif //EM_DEBUG
/*************************************************************************/
/*
#define EM_BAD_ALLOC 1

#define EM_TRAP(__r,__s) int __r; { try { __s; } catch(bad_alloc) { __r = EM_BAD_ALLOC; } catch(...) { throw; } __r = 0; }
#define EM_TRAP2(__r,__s1,__2) int __r; { try { __s1; __s2; } catch(bad_alloc) { __r = EM_BAD_ALLOC; } catch(...) { throw; } __r = 0; }
#define EM_TRAP3(__r,__s1,__s2,__s3) int __r; { try { __s1; __s2; __s3; } catch(bad_alloc) { __r = EM_BAD_ALLOC; } catch(...) { throw; } __r = 0; }
#define EM_TRAP4(__r,__s1,__s2,__s3,__s4) int __r; { try { __s1; __s2; __s3; __s4; } catch(bad_alloc) { __r = EM_BAD_ALLOC; } catch(...) { throw; } __r = 0; }

#define EM_TRAP_CLEAN(__s, __o) { try { __s; } catch(bad_alloc) { delete __o; throw; } catch(...) { throw; } }
//#define EM_TRAP_CLEAN2(__s, __o1, __o2) { try { __s; } catch(bad_alloc) { delete __o1; delete __o2; throw; } catch(...) { throw; } }
//#define EM_TRAP_CLEAN3(__s, __o1, __o2, __o3) { try { __s; } catch(bad_alloc) { delete __o1; delete __o2; delete __o3; throw; } catch(...) { throw; } }
//#define EM_TRAP_CLEAN4(__s, __o1, __o2, __o3, __o4) { try { __s; } catch(bad_alloc) { delete __o1; delete __o2; delete __o3; delete __o4; throw; } catch(...) { throw; } }
*/

#endif
