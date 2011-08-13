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

#ifndef __CLIP_PROPERTIES_DIALOG
#define __CLIP_PROPERTIES_DIALOG

#include "EMListener.h"
#include <map>

class ClipPropertiesDialog;
class EMButton;
class EMFactory;
class EMGroupBox;
class EMLabel;
class EMListenerRepository;
class EMView;
class EMWindow;

const uint32 AUDIO = 1;
const uint32 MIDI = 2;
const uint32 VIDEO = 3;
const uint32 TRANSITION = 4;

class ClipPropertiesDialogComponents
{
public:
	ClipPropertiesDialogComponents();
	~ClipPropertiesDialogComponents();

public:
	EMLabel* m_opAudioInfoLabel;
	EMLabel* m_opAudioInfoValueLabel;
	EMLabel* m_opBeginLabel;
	EMLabel* m_opBeginValueLabel;
	
	EMLabel* m_opBitDepthLabel;
	EMLabel* m_opBitDepthValueLabel;
	EMLabel* m_opChannelsLabel;
	EMLabel* m_opChannelsValueLabel;
	
	EMWindow* m_opDialogWindow;
	EMLabel* m_opDimentionLabel;
	EMLabel* m_opDimentionValueLabel;
	EMLabel* m_opDurationLabel;
	EMLabel* m_opDurationValueLabel;
	
	EMLabel* m_opEndLabel;
	EMLabel* m_opEndValueLabel;
	EMLabel* m_opEventLabel;
	EMLabel* m_opEventValueLabel;
	
	EMLabel* m_opFileLabel;
	EMLabel* m_opFileValueLabel;
	EMLabel* m_opFileLengthLabel;
	EMLabel* m_opFileLengthValueLabel;
	
	EMLabel* m_opFramesLabel;
	EMLabel* m_opFramesValueLabel;
	EMView* m_opMainView;
	EMLabel* m_opMarkInLabel;
	EMLabel* m_opMarkInValueLabel;
	
	EMLabel* m_opMarkOutLabel;
	EMLabel* m_opMarkOutValueLabel;
	EMLabel* m_opNameLabel;
	EMLabel* m_opNameValueLabel;
	
	EMLabel* m_opSamplesLabel;
	EMLabel* m_opSamplesValueLabel;
	EMGroupBox* m_opSeparator;
	EMLabel* m_opTypeLabel;
	EMLabel* m_opTypeValueLabel;

	uint32 m_vFormat;

	ClipPropertiesDialogComponents* m_opPrevFocus;
	ClipPropertiesDialogComponents* m_opNextFocus;

	ClipPropertiesDialog* m_opDialog;
};

class ClipPropertiesDialog : public EMListener
{
public:
	ClipPropertiesDialog(/*float p_vXpoint, float v_pYpoint, const uint32 p_vFormat*/);
	~ClipPropertiesDialog();
	void ConvertToScreenCoordinates(int32 &p_vX, int32 &p_vY);
	void DeleteAllControls();
	void DisplayDialog();
	EMWindow* GetDialogWindow();
	void HideDialog(EMWindow* p_opDialogWindow);
	void Init(float p_vXpoint, float p_vYpoint, const uint32 p_vFormat);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog(float p_vX, float p_vY);
	void SetAudioInfoValue(string p_oValue);
	void SetBeginValue(string p_oValue);
	void SetBitDepthValue(string p_oValue);
	void SetChannelsValue(string p_oValue);
	void SetDimentionValue(string p_oValue);
	void SetDurationValue(string p_oValue);
	void SetEndValue(string p_oValue);
	void SetEventValue(string p_oValue);
	void SetFileValue(string p_oValue);
	void SetFileLengthValue(string p_oValue);
	void SetFramesValue(string p_oValue);
	void SetMarkInValue(string p_oValue);
	void SetMarkOutValue(string p_oValue);
	void SetNameValue(string p_oValue);
	void SetSamplesValue(string p_oValue);
	void SetTypeValue(string p_oValue);

protected:
	int32 GetMaxStringWidth();

protected:
	static map<EMWindow*, ClipPropertiesDialogComponents*> m_oComponentsMap;
	static ClipPropertiesDialogComponents* m_opGotFocus;
	static ClipPropertiesDialogComponents* m_opHadFocus;
	static uint32 m_vDisplayCount;
	static bool m_vDoNotChangeLastFocus;

	static ClipPropertiesDialogComponents* m_opFirstDialog;
	static ClipPropertiesDialogComponents* m_opCurrentDialog;
	static ClipPropertiesDialogComponents* m_opLastDialog;

private:
	void AddDialog(ClipPropertiesDialogComponents* p_opAddDialog);
	void DeleteDialog(ClipPropertiesDialogComponents* p_opDeleteDialog);
	void MoveDialog(ClipPropertiesDialogComponents* p_opMoveDialog);

private:
	string m_oAudioInfoValue;
	string m_oBeginValue;
	string m_oBitDepthValue;
	string m_oChannelsValue;
	string m_oDimentionValue;
	string m_oDurationValue;
	string m_oEndValue;
	string m_oEventValue;
	string m_oFileValue;
	string m_oFileLengthValue;
	string m_oFramesValue;
	string m_oMarkInValue;
	string m_oMarkOutValue;
	string m_oNameValue;
	string m_oSamplesValue;
	string m_oTypeValue;

	ClipPropertiesDialogComponents* m_opComponents;
//	map<EMWindow*, ClipPropertiesDialogComponents*> m_oComponentsMap;
	map<EMWindow*, ClipPropertiesDialogComponents*>::iterator m_oComponentsIterator;
/*	EMLabel* m_opAudioInfoLabel;
	EMLabel* m_opAudioInfoValueLabel;
	EMLabel* m_opBeginLabel;
	EMLabel* m_opBeginValueLabel;
	
	EMLabel* m_opBitDepthLabel;
	EMLabel* m_opBitDepthValueLabel;
	EMLabel* m_opChannelsLabel;
	EMLabel* m_opChannelsValueLabel;
	
	EMWindow* m_opDialogWindow;
	EMLabel* m_opDimentionLabel;
	EMLabel* m_opDimentionValueLabel;
	EMLabel* m_opDurationLabel;
	EMLabel* m_opDurationValueLabel;
	
	EMLabel* m_opEndLabel;
	EMLabel* m_opEndValueLabel;
	EMLabel* m_opEventLabel;
	EMLabel* m_opEventValueLabel;
	
	EMLabel* m_opFileLabel;
	EMLabel* m_opFileValueLabel;
	EMLabel* m_opFileLengthLabel;
	EMLabel* m_opFileLengthValueLabel;
	
	EMLabel* m_opFramesLabel;
	EMLabel* m_opFramesValueLabel;
	EMView* m_opMainView;
	EMLabel* m_opMarkInLabel;
	EMLabel* m_opMarkInValueLabel;
	
	EMLabel* m_opMarkOutLabel;
	EMLabel* m_opMarkOutValueLabel;
	EMLabel* m_opNameLabel;
	EMLabel* m_opNameValueLabel;
	
	EMLabel* m_opSamplesLabel;
	EMLabel* m_opSamplesValueLabel;
	EMGroupBox* m_opSeparator;
	EMLabel* m_opTypeLabel;
	EMLabel* m_opTypeValueLabel;
*/
	float m_vFirstTab;
//	uint32 m_vFormat;
	float m_vSecondTab;
};


#endif