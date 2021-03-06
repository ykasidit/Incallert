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

#include "IncallertDocument.h"
#include "IncallertApplication.h"

CApaDocument* CIncallertApplication::CreateDocumentL()
    {
    CApaDocument* document = CIncallertDocument::NewL(*this);
    return document;
    }

TUid CIncallertApplication::AppDllUid() const
    {
    return KUidIncallert;
    }

