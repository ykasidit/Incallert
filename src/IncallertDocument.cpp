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

#include "IncallertAppui.h"
#include "IncallertDocument.h"

// Standard Symbian OS construction sequence
CIncallertDocument* CIncallertDocument::NewL(CEikApplication& aApp)
    {
    CIncallertDocument* self = NewLC(aApp);
    CleanupStack::Pop(self);
    return self;
    }

CIncallertDocument* CIncallertDocument::NewLC(CEikApplication& aApp)
    {
    CIncallertDocument* self = new (ELeave) CIncallertDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

void CIncallertDocument::ConstructL()
    {
	// no implementation required
    }

CIncallertDocument::CIncallertDocument(CEikApplication& aApp) : CAknDocument(aApp)
    {
	// no implementation required
    }

CIncallertDocument::~CIncallertDocument()
    {
	// no implementation required
    }

CEikAppUi* CIncallertDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object
    CEikAppUi* appUi = new (ELeave) CIncallertAppUi;
    return appUi;
    }

