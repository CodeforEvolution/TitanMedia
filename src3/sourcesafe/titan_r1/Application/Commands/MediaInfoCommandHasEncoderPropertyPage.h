/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_HAS_ENCODER_PROPERTY_PAGE
#define __MEDIA_INFO_COMMAND_HAS_ENCODER_PROPERTY_PAGE

#include "EMCommand.h"

class MediaInfoCommandHasEncoderPropertyPage : public EMCommand
{
public:
	MediaInfoCommandHasEncoderPropertyPage();
	void ExecuteE() {};
	void* ExecuteE(void* p_vEncoderID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	bool m_vHasPropertyPage;

};

#endif
