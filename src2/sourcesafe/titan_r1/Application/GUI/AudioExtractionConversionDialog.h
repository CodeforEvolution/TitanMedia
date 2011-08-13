/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Application, Dialog
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __AUDIO_EXTRACTION_CONVERSION_DIALOG
#define __AUDIO_EXTRACTION_CONVERSION_DIALOG

#include "EMListener.h"

class EMButton;
class EMFactory;
class EMLabel;
class EMProgressBar;
class EMView;
class EMWindow;

class EMListenerRepository;

class AudioExtractionConversionDialog : public EMListener
{
public:
	AudioExtractionConversionDialog();
	~AudioExtractionConversionDialog();
	static void Delete();
	void DisplayDialog();
	void HideDialog();
	void Init();
	static AudioExtractionConversionDialog* Instance();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetBottomLabel(const char* p_vpText);
	void SetCommand(uint32 p_vCommandID);
	void SetParent(EMWindow* p_opWindow);
	void SetTopLabel(const char* p_vpText);
	void SetValue(uint16 p_vValue);

private:
	void AddCancelButton(bool p_vAdd, uint32 p_vCommandID = 0);

private:
	EMLabel*		m_opBottomLabel;
	EMButton*		m_opCancelButton;
	EMWindow*		m_opDialogWindow;
	EMLabel*		m_opETA_Label;
	static AudioExtractionConversionDialog* m_opInstance;
	EMView*			m_opMainView;
	EMWindow*		m_opParentWindow;
	EMProgressBar*	m_opProgressBar;
	EMLabel*		m_opTopLabel;
	uint32			m_vCommandID;
	uint16			m_vCountWhen;
	int64			m_vCurrentTime;
	char*			m_vpETA_Text;
	char*			m_vpTime;
	char*			m_vpTotTime;
	int64			m_vTotTime;
	int64			m_vStartTime;
};


#endif