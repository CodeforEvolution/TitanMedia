#include "EMRect.h"

EMRect::EMRect(int32 p_vLeft, int32 p_vTop, int32 p_vRight, int32 p_vBottom) :
m_vLeft(p_vLeft),
m_vTop(p_vTop),
m_vRight(p_vRight),
m_vBottom(p_vBottom)
{
}

EMRect::EMRect() :
m_vLeft(0),
m_vTop(0),
m_vRight(-1),
m_vBottom(-1)
{
}

EMRect::EMRect(const EMRect& p_oRect) :
m_vLeft(p_oRect.m_vLeft),
m_vTop(p_oRect.m_vTop),
m_vRight(p_oRect.m_vRight),
m_vBottom(p_oRect.m_vBottom)
{
}

bool EMRect::Contains(int32 p_vX, int32 p_vY)
{
	if(p_vX >= m_vLeft && p_vX <= m_vRight && p_vY >= m_vTop && p_vY <= m_vBottom)
		return true;
	return false;
}

bool EMRect::Contains(EMRect p_oRect)
{
	if(p_oRect.m_vLeft >= m_vLeft && p_oRect.m_vRight <= m_vRight && p_oRect.m_vTop >= m_vTop && p_oRect.m_vBottom <= m_vBottom)
		return true;
	return false;
}

int32 EMRect::GetHeight()
{
	if(!IsValid())
		return 0;
	return m_vBottom - m_vTop;
}

int32 EMRect::GetWidth()
{
	if(!IsValid())
		return 0;
	return m_vRight - m_vLeft;
}

void EMRect::InsetBy(int32 p_vX, int32 p_vY)
{
	if(!IsValid())
		return;
		
	m_vLeft += p_vX;
	m_vTop += p_vY;
	m_vRight -= p_vX;
	m_vBottom -= p_vY;
	if(m_vRight < m_vLeft)	
		m_vRight = m_vLeft;
	if(m_vBottom < m_vTop)
		m_vBottom = m_vTop;
}

bool EMRect::Intersects(const EMRect p_oRect) const
{
	if(!IsValid() || !(p_oRect.IsValid()))
		return false;
		
	if(p_oRect.m_vBottom < m_vTop) // Is above
		return false;
	if(p_oRect.m_vTop > m_vBottom) // Is below
		return false;
	if(p_oRect.m_vRight < m_vLeft) // Is to the left
		return false;
	if(p_oRect.m_vLeft > m_vRight) // Is to the right
		return false;
	return true; // Intersects
}

bool EMRect::IsValid() const
{
	if(m_vRight >= m_vLeft && m_vBottom >= m_vTop)
		return true;
	return false;
}

void EMRect::OffsetBy(int32 p_vX, int32 p_vY)
{
	if(!IsValid())
		return;
		
	m_vLeft += p_vX;
	m_vTop += p_vY;
	m_vRight += p_vX;
	m_vBottom += p_vY;
}

void EMRect::OffsetTo(int32 p_vX, int32 p_vY)
{
	if(!IsValid())
		return;
	
	int32 vOffsetX = p_vX - m_vLeft;
	int32 vOffsetY = p_vY - m_vTop;
		
	m_vLeft += vOffsetX;
	m_vTop += vOffsetY;
	m_vRight += vOffsetX;
	m_vBottom += vOffsetY;
}

EMRect EMRect::operator |(EMRect p_oMergingRect)
{
	if(!IsValid())
	{
		if(!(p_oMergingRect.IsValid()))
			return EMRect(); // None of the rects are valid, an invalid rect is returned
		// Since this rect was invalid, the union is only the merging rect
		m_vLeft = p_oMergingRect.m_vLeft;
		m_vTop = p_oMergingRect.m_vTop;
		m_vRight = p_oMergingRect.m_vRight;
		m_vBottom = p_oMergingRect.m_vBottom;
		return EMRect(*this);
	}
	
	EMRect oNewRect(*this); // Clones this rect
	if(p_oMergingRect.m_vLeft < oNewRect.m_vLeft)
		oNewRect.m_vLeft = p_oMergingRect.m_vLeft;
	if(p_oMergingRect.m_vTop < oNewRect.m_vTop)
		oNewRect.m_vTop = p_oMergingRect.m_vTop;
	if(p_oMergingRect.m_vRight > oNewRect.m_vRight)
		oNewRect.m_vRight = p_oMergingRect.m_vRight;
	if(p_oMergingRect.m_vBottom > oNewRect.m_vBottom)
		oNewRect.m_vBottom = p_oMergingRect.m_vBottom;
	return oNewRect;
}

// This is not a true xor operator, it returns the smallest rectangle
// containing the areas of this rect xor:ed with the parameter rect
// That is, the area of the parameter rect is "subtracted" from
// this rect and a smallest rect is calculated and returned
EMRect EMRect::operator ^(EMRect p_oRect) const
{
	if(*this == p_oRect || p_oRect.Contains(*this))
		return EMRect();

	EMRect oRect(*this & p_oRect);
	int vValuesInCommon = 0;
	if(oRect.m_vLeft == m_vLeft)
		vValuesInCommon++;
	if(oRect.m_vTop == m_vTop)
		vValuesInCommon++;
	if(oRect.m_vRight == m_vRight)
		vValuesInCommon++;
	if(oRect.m_vBottom == m_vBottom)
		vValuesInCommon++;
		
	if(vValuesInCommon < 3)
		return *this;
		
	if(oRect.m_vLeft < m_vLeft)
		return EMRect(oRect.m_vRight, m_vTop, m_vRight, m_vBottom);
		
	if(oRect.m_vTop < m_vTop)
		return EMRect(m_vLeft, oRect.m_vBottom, m_vRight, m_vBottom);
		
	if(oRect.m_vRight > m_vRight)
		return EMRect(m_vLeft, m_vTop, oRect.m_vLeft, m_vBottom);
		
	// oRect.m_vBottom >= m_vBottom
	return EMRect(m_vLeft, m_vTop, m_vRight, oRect.m_vTop);
}

// Is invalid if the two rects do not intersect with each other
// Returns the intersection of the two rects
EMRect EMRect::operator &(EMRect p_oRect) const
{
	// Intersection with an invalid rect always gives an invalid result
	if(!IsValid() || !p_oRect.IsValid())
		return EMRect();
/*
	if(!IsValid() && !p_oRect.IsValid())
		return EMRect(); // None of the rects are valid, an invalid rect is returned
	if(!IsValid())
		return p_oRect;
	if(!p_oRect.IsValid())
		return EMRect(*this);
*/
	EMRect oNewRect = p_oRect;
	if(m_vLeft > oNewRect.m_vLeft)
		oNewRect.m_vLeft = m_vLeft;
	if(m_vRight < oNewRect.m_vRight)
		oNewRect.m_vRight = m_vRight;
	if(m_vTop > oNewRect.m_vTop)
		oNewRect.m_vTop = m_vTop;
	if(m_vBottom < oNewRect.m_vBottom)
		oNewRect.m_vBottom = m_vBottom;
	return oNewRect;
}	

bool EMRect::operator ==(EMRect p_oRect) const
{
	if(!IsValid() && !p_oRect.IsValid()) // One invalid rect always equals another invalid rect
		return true;
	return p_oRect.m_vLeft == m_vLeft && p_oRect.m_vTop == m_vTop && 
	       p_oRect.m_vRight == m_vRight && p_oRect.m_vBottom == m_vBottom;
}

bool EMRect::operator !=(EMRect p_oRect) const
{
	if(!IsValid() && !p_oRect.IsValid()) // One invalid rect always equals another invalid rect
		return false;
	return p_oRect.m_vLeft != m_vLeft || p_oRect.m_vTop != m_vTop || 
	       p_oRect.m_vRight != m_vRight || p_oRect.m_vBottom != m_vBottom;
}

void EMRect::Print()
{
	if(!IsValid())
		eo << "EMRect: Invalid! (" << m_vLeft << ", " << m_vTop << ", " << m_vRight << ", " << m_vBottom << ")" << ef;
	else	
		eo << "EMRect: frame - left: " << m_vLeft << ", top: " << m_vTop << ", right: " << m_vRight << ", bottom: " << m_vBottom << ef;
}

