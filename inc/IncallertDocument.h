/*  
    Copyright (C) 2009 Kasidit Yusuf.

    This file is part of Incallert.

    Incallert is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Incallert is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Incallert.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __INCALLERT_DOCUMENT_H__
#define __INCALLERT_DOCUMENT_H__


#include <akndoc.h>

// Forward references
class CIncallertAppUi;
class CEikApplication;


/*! 
  @class CIncallertDocument
  
  @discussion An instance of class CIncallertDocument is the Document part of the AVKON
  application framework for the Incallert example application
  */
class CIncallertDocument : public CAknDocument
    {
public:

/*!
  @function NewL
  
  @discussion Construct a CIncallertDocument for the AVKON application aApp 
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CIncallertDocument
  */
    static CIncallertDocument* NewL(CEikApplication& aApp);

/*!
  @function NewLC
  
  @discussion Construct a CIncallertDocument for the AVKON application aApp 
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CIncallertDocument
  */
    static CIncallertDocument* NewLC(CEikApplication& aApp);

/*!
  @function ~CIncallertDocument
  
  @discussion Destroy the object and release all memory objects
  */
    ~CIncallertDocument();

public: // from CAknDocument
/*!
  @function CreateAppUiL 
  
  @discussion Create a CIncallertAppUi object and return a pointer to it
  @result a pointer to the created instance of the AppUi created
  */
    CEikAppUi* CreateAppUiL();

private:

/*!
  @function ConstructL
  
  @discussion Perform the second phase construction of a CIncallertDocument object
  */
    void ConstructL();

/*!
  @function CIncallertDocument
  
  @discussion Perform the first phase of two phase construction 
  @param aApp application creating this document
  */
    CIncallertDocument(CEikApplication& aApp);

    };


#endif // __INCALLERT_DOCUMENT_H__
