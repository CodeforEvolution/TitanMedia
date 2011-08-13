#include "EMMouseCursor.h"

EMMouseCursor::EMMouseCursor(EMMouseCursorType p_eType) :
m_eType(p_eType)
{

}

EMMouseCursor::~EMMouseCursor()
{
}

EMMouseCursorType EMMouseCursor::GetType()
{
	return m_eType;
}