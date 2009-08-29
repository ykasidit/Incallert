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

#ifndef AboutTextContainer_H
#define AboutTextContainer_H



// System includes
#include <coecntrl.h> // CCoeControl
#include <txtfrmat.h> // TCharFormatMask
#include "ScrollRichTextContainer.h"

class CMyRichTextEditor;


class CAboutTextContainer : public  CScrollRichTextContainer
	{
public: // Constructors and destructor

	static CAboutTextContainer* NewL(const TRect& aRect,MObjectProvider *provider);
	static CAboutTextContainer* NewLC(const TRect& aRect,MObjectProvider *provider);
	CAboutTextContainer(MObjectProvider *provider);
	void AddTextToEditorL();

	};

#endif
