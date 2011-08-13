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
// Misc stuff
/*************************************************************************/

#define EM_BIG_ENDIAN // Used by x86 architectures
//#define EM_LITTLE_ENDIAN // Used by PowerPC architectures


//#define PLATFORM_BEOS
#define PLATFORM_WINDOWS
//#define PLATFORM_WINDOWS_2000
//#define PLATFORM_MAC_OS
//#define PLATFORM_MAC_OS_X
//#define PLATFORM_LINUX
//#define PLATFORM_VIC20

/*************************************************************************/
// End of misc stuff
/*************************************************************************/

/*************************************************************************/
// BeOS stuff
/*************************************************************************/

#ifdef PLATFORM_BEOS

#include <SupportDefs.h>  // many nice BeOS typedefs

#define UINT32_MAX 0xFFFFFFFFLL
#define UINT64_MAX 0xFFFFFFFFFFFFFFFFLL
#define INT32_MAX 0x7FFFFFFFFLL
#define INT64_MAX 0x7FFFFFFFFFFFFFFFFLL

#define EM_FILE_SEPARATOR "/"

#define EM_DEBUG // If debug code should be used

#ifdef EM_DEBUG
#include <Debug.h>
#define EM_new new // No special new here as of this moment
#else
#define EM_new new
#endif

#endif // PLATFORM_BEOS

/*************************************************************************/
// End of BeOS stuff
/*************************************************************************/

/*************************************************************************/
// MS Windows stuff
/*************************************************************************/

#ifdef PLATFORM_WINDOWS

#pragma warning(disable : 4786) // Skips boring debug truncate warning
#pragma warning(disable : 4275) // Skips "...needs to have dll-interface..."
#pragma warning(disable : 4251) // Skips "...needs to have dll-interface..."

#ifdef _DEBUG
#define EM_DEBUG // If debug code should be used
#include <crtdbg.h>
#include <memory>
#define EM_new new(_NORMAL_BLOCK, __FILE__, __LINE__) // Helps when debugging memory leaks
#else
#define EM_new new
#endif

#ifdef LoadString // Stupid MS Windows macros, CreateWindow is another one
#undef LoadString
#endif

#define UINT32_MAX 0xFFFFFFFF
#define UINT64_MAX 0xFFFFFFFFFFFFFFFF
#define INT32_MAX 0x7FFFFFFF
#define INT64_MAX 0x7FFFFFFFFFFFFFFF

typedef	signed char	int8;
typedef unsigned char uint8;
typedef volatile signed char vint8;
typedef volatile unsigned char vuint8;

typedef	short int16;
typedef unsigned short uint16;
typedef volatile short vint16;
typedef volatile unsigned short	vuint16;

typedef	long int32;
typedef unsigned long uint32;
typedef volatile long vint32;
typedef volatile unsigned long vuint32;

typedef	__int64	int64;
typedef unsigned __int64 uint64;
typedef volatile __int64 vint64;
typedef volatile unsigned __int64 vuint64;

// Are we using these typedefs in any code?
typedef volatile long vlong;
typedef volatile int vint;
typedef volatile short vshort;
typedef volatile char vchar;
typedef volatile unsigned long vulong;
typedef volatile unsigned int vuint;
typedef volatile unsigned short vushort;
typedef volatile unsigned char vuchar;
typedef unsigned char uchar;
typedef unsigned short unichar;

#ifdef NULL
#undef NULL
#define NULL 0
#endif // NULL

#define EM_FILE_SEPARATOR "\\"

#endif // PLATFORM_WINDOWS

/*************************************************************************/
// End of MS Windows stuff
/*************************************************************************/

#include <new> // Needed for bad_alloc exception handling

#include "EMDebugOutput.h" // This one needs to be included last in the file

#endif // __EM_GLOBALS
