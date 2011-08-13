/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_FILE
#define __EM_MEDIA_FILE

#include "EMMediaGlobals.h"

class EMMediaFile 
{
public:
	static EMMediaFile* Create(const string* p_opName);
	virtual ~EMMediaFile();

	virtual bool Close() = 0;
	const string* GetFileName();
	int32 GetID() const;
	int32 GetType() const;
	bool InitCheckE();
	bool IsOpen() const;
	virtual bool Open(int32 p_vDirection) = 0;
	virtual int64 Read(void* p_upBuffer, int64 p_vLength, int64 p_vStartPosition) = 0;
	virtual bool SeekFromEnd(int64 p_vByteNumber) = 0;
	virtual bool SeekFromStart(int64 p_vByteNumber) = 0;
	virtual int64 GetPosition() const = 0;
	virtual void Write(void* p_upBuffer, int64 p_vLength) = 0;
	virtual int64 GetLength() = 0;

protected:
	EMMediaFile(const string* p_opName);
	void SetOpen(bool p_vFlag);
	int32 m_vDirection;

private:
	int32 m_vID;
	bool m_vIsOpen;
	string m_oFileName;
};

#endif
