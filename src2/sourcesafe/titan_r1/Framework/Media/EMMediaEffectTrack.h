/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
*
* This class is the "effect track", that holds a number
* of effects (i e plugins) that are connected in a chain.
* It has an output (e g Layla24 Analog Out 1/2) and a
* start plugin in the chain. As plugins are added to the
* track, the destinations of the respective plugins 
* (the plugins inherit from EMBufferDestination, so they
* have a destination) need to be updated to keep the
* chain valid.
*
*******************************************************/

#ifndef __EM_MEDIA_EFFECT_TRACK
#define __EM_MEDIA_EFFECT_TRACK

#include "EMBufferDestination.h"
#include "EMMediaGlobals.h"
#include "EMMediaItemContainer.h"
#include "EMMediaDataBuffer.h"

class EMBufferDestination;
class EMMediaTrack;
class EMPlugin;
class EMSemaphore;

class EMMediaEffectTrack : public EMBufferDestination
{
public:
	EMMediaEffectTrack(EMMediaType p_eType);
	EMMediaEffectTrack(EMMediaEffectTrack& oTrack);

	~EMMediaEffectTrack();

	string GetName() const;
	bool InitCheckE();
	bool PrepareToPlayE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);
	void SetName(string p_oName);

	//Processing management
	bool SetOutput(EMBufferDestination* p_opOutput);
	EMBufferDestination* GetOutput() const;

	//Slot management
	bool InsertEffect(EMPlugin* p_opPlugin, uint32 p_vSlotNumber);
	bool SetEffect(EMPlugin* p_opPlugin, uint32 p_vSlotNumber);
	bool DeleteEffect(uint32 p_vSlotNumber);
	bool IsSlotEmpty(uint32 p_vSlotNumber);
	uint32 GetNumberOfSlots() const;
	uint32 GetLastEffect();
	uint32 CountEffects();
	void DisplaySlots();
	EMPlugin* FindEffect(int32 p_vID);
	EMPlugin* GetSlot(uint32 p_vIndex);
	int32 GetSlotFor(EMPlugin* p_opPlugin);

	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);

	bool RestoreAfterLoad();

	bool IsObjectDeleted();
	void SetDeleted(bool p_vState);

private:
	bool MakeRoom(uint32 p_vSlotNumber);
	EMBufferDestination* GetNextDestination(uint32 p_vCurrentSlot);
	EMBufferDestination* GetPreviousDestination(uint32 p_vCurrentSlot);

	string m_oName;
	EMPlugin** m_opSlots;
	uint32 m_vNumSlots;
	EMBufferDestination* m_opOutput;
	EMMediaTrack* m_opSourceMediaTrack;
	EMSemaphore* m_opSemaphore;
	int32 m_vOutputId;
	bool m_vIsDeleted;
};

#endif