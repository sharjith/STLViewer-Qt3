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

// ListNodeOfCTriFacet.h: interface for the CListNodeOfCTriFacet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_LISTNODEOFTRIFACET_H__0206868D_2E9C_40D2_AB3E_AD8990AA5941__INCLUDED_)
#define _LISTNODEOFTRIFACET_H__0206868D_2E9C_40D2_AB3E_AD8990AA5941__INCLUDED_

#include "TriFacet.h"

class CTriFacet;
class CListOfCTriFacet;

class CListNodeOfCTriFacet  
{
	friend class CListOfCTriFacet;
	friend class CListIteratorOfListOfCTriFacet;
public:
	CListNodeOfCTriFacet(const CTriFacet&);
	CTriFacet GetData() const;
	virtual ~CListNodeOfCTriFacet();
	bool operator==(CListNodeOfCTriFacet& rhs);
	bool operator!=(CListNodeOfCTriFacet& rhs);
private:
	CTriFacet data;
	CListNodeOfCTriFacet* nextPtr;

};

#endif
