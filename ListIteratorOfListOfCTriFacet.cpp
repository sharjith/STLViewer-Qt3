// ListIteratorOfListOfCTriFacet.cpp: implementation of the CListIteratorOfListOfCTriFacet class.
//
//////////////////////////////////////////////////////////////////////

#include "ListIteratorOfListOfCTriFacet.h"
#include "ListException.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListIteratorOfListOfCTriFacet::CListIteratorOfListOfCTriFacet(CListOfCTriFacet* aList) : theList(aList)
{
}

CListIteratorOfListOfCTriFacet::~CListIteratorOfListOfCTriFacet()
{
}

void CListIteratorOfListOfCTriFacet::SetList(CListOfCTriFacet* aList)
{
	theList = aList;
}

void CListIteratorOfListOfCTriFacet::Init()
{
	curPtr = theList->firstPtr;
}

bool CListIteratorOfListOfCTriFacet::More() const
{
	return (curPtr != 0);
}

void CListIteratorOfListOfCTriFacet::Next()
{
	curPtr = (curPtr) ? curPtr->nextPtr : 0;
}

CTriFacet CListIteratorOfListOfCTriFacet::Current() const
{
	return (curPtr) ? curPtr->data : CTriFacet();
}

CTriFacet CListIteratorOfListOfCTriFacet::ValueAt(const int index)
{
	int cnt =0;
	Init();
	while(curPtr != 0)
	{
		Next();
		cnt++;
	}
	
	if(cnt < index)
		throw CListException(LIST_BOUND_UPPER);
		
	else if(index<1)
		throw CListException(LIST_BOUND_LOWER);
		
	/*else*/ if(index ==1 )
		return theList->firstPtr->data;
	else
	{
		Init();
		for(int i=1; i < index; i++)
		{
			if(curPtr->nextPtr == 0)
				break;
			Next();
		}
		
		return curPtr->data;
	}
		
}
 
