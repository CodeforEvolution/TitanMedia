/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
	time = 
	0	1	2	3	4	n
	Slot content = 
	[ ]	[ ]	[ ]	[ ]	[ ] . . . [ ]
	 4	 5	 9	 2	
		 R	 		 W
		 e			 r
		 a			 i
		 d			 t
		 			 e
		 			 
	Readpos  = 1 % maxlength
	Writepos = 4 % maxlength

*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_AUDIO_SIGNAL_METER
#define __EM_MEDIA_AUDIO_SIGNAL_METER

#include "EMMediaGlobals.h"

#include "EMBufferDestination.h"
#include "EMListener.h"

#include "EMMediaSignalMeter.h"
#include "EMMediaSignalMeterRepository.h"

class EMSemaphore;

class EMMediaSignal
{
public:
	EMMediaSignal() 
	{ 
		m_vpValues[0] = -1;
		m_vpValues[1] = -1;
		m_vpValues[2] = -1;
		m_vpValues[3] = -1;
		m_vpValues[4] = -1;
		Clear();
	};

	~EMMediaSignal()
	{
	};

	void Clear() 
	{ 
		m_vTime = 0; 
		for(register int8 i = 0; i < 5; i++)
			m_vpValues[i] = -1;
		m_vFree = true;
	};
	int64 m_vTime;
	float m_vpValues[5];
	bool m_vFree;
};

class EMMediaAudioSignalMeter : public EMMediaSignalMeter //, public EMListener
{
public:
	EMMediaAudioSignalMeter();
	virtual ~EMMediaAudioSignalMeter();
	void Get(float* p_vpSamplesOut);
	void Put(const float* p_vpSamples, int32 p_vNumChannels);
	void Clear();

	bool PrepareToPlayE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);
	EMMediaDataBuffer* ProcessBufferE(EMMediaDataBuffer** p_opBufferList);

	void OnStop();
	void OnStart();
	void OnSeek();
//	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);

private:
	EMMediaSignal** m_opSignalArray;
	EMMediaSignal* m_opZeroSignal;
	int32 m_vMax;
	int32 m_vMostRecentPut;
	int32 m_vMostRecentGet;
//	bool m_vIsPaused;
	EMSemaphore* m_opSemaphore;

	friend class EMMediaSignalMeterRepository; //Hehe... ;-)
};

#endif
