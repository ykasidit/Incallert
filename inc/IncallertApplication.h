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

#ifndef __INCALLERT_APPLICATION_H__
#define __INCALLERT_APPLICATION_H__

#include <aknapp.h>


/*!
  @class CIncallertApplication

  @discussion An instance of CIncallertApplication is the application part of the AVKON
  application framework for the Incallert example application
  */
class CIncallertApplication : public CAknApplication
    {
public:  // from CAknApplication

/*!
  @function AppDllUid

  @discussion Returns the application DLL UID value
  @result the UID of this Application/Dll
  */
    TUid AppDllUid() const;

protected: // from CAknApplication
/*!
  @function CreateDocumentL

  @discussion Create a CApaDocument object and return a pointer to it
  @result a pointer to the created document
  */
    CApaDocument* CreateDocumentL();
    };

    static const TUid KUidIncallert = {0x2001163B};

#endif // __INCALLERT_APPLICATION_H__
