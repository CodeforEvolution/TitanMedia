#include "EMException.h"

// Does not get ownership of p_opMessage
EMException::EMException(EMExceptionType p_eType, const string* p_opMessage)
{
	m_eType = p_eType;
	m_oMessage = *p_opMessage;
}

string* EMException::GetMessage()
{
	return &m_oMessage;
}

EMExceptionType EMException::GetType()
{
	return m_eType;
}
