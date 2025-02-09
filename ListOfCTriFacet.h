//############################################################################

//  Author:	Sharjith Nair
//  Email:	sharjith_ssn@hotmail.com

// Program:	Geometric Class Library

//							Important Notice:

//	This Idea and the Application is Copyright(c) Sharjith Nair 2002, 2003.
//	You can freely use it as long as you credit me for it.

//	No guarantee/warranty is given on this app and I will not be responsible 
//	for any damage to you, your property or any other person from using it.
//	USE IT ON YOUR OWN RISK.

//	Thankyou
//	Sharjith Nair.

//#############################################################################

// ListOfCTriFacet.h: interface for the CListOfCTriFacet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_LISTOFCTriFacet_H__64FE1368_B1AF_44CF_8FFF_9382221176C2__INCLUDED_)
#define _LISTOFCTriFacet_H__64FE1368_B1AF_44CF_8FFF_9382221176C2__INCLUDED_

#include "ListNodeOfCTriFacet.h"

class CTriFacet;
class CListIteratorOfListOfCTriFacet;

class CListOfCTriFacet
{
	friend class CListIteratorOfListOfCTriFacet;
public:
	CListOfCTriFacet();
	virtual ~CListOfCTriFacet();
	void Append(const CTriFacet& data);
	void Prepend(const CTriFacet& data);
	CTriFacet First() const;
	CTriFacet Last() const;
	bool IsEmpty() const;
	void Clear();
private:
	CListNodeOfCTriFacet* firstPtr;
	CListNodeOfCTriFacet* lastPtr;
private:
	CListNodeOfCTriFacet* NewNode(const CTriFacet&);
};

#endif // !defined(_LISTOFCTriFacet_H__64FE1368_B1AF_44CF_8FFF_9382221176C2__INCLUDED_)
