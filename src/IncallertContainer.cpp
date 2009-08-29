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

#include "IncallertContainer.h"
#include "IncallertAppui.h"
#include "LineStatusHandler.h"
#include <iclmain.mbg>

CIncallertContainer* CIncallertContainer::NewL(const TRect& aRect, CIncallertAppUi *ui)
    {
		CIncallertContainer* self = CIncallertContainer::NewLC(aRect,ui);
	    CleanupStack::Pop(self);
	    return self;
    }

CIncallertContainer* CIncallertContainer::NewLC(const TRect& aRect, CIncallertAppUi *ui)
    {
	    CIncallertContainer* self = new (ELeave) CIncallertContainer(ui);
	    CleanupStack::PushL(self);
	    self->ConstructL(aRect);
	    return self;
    }

void CIncallertContainer::ConstructL(const TRect& aRect)
    {
    	LoadBitMap();

    CreateWindowL();
	SetRect(aRect);
	ActivateL();
    }

TInt CIncallertContainer::CountComponentControls() const
    {
    return 0;
    }

CIncallertContainer::~CIncallertContainer()
{
	delete iBitmap;
	iBitmap = NULL;
}


void CIncallertContainer::Draw(const TRect&) const
    {
    // Get the standard graphics context
 	CWindowGc& gc = SystemGc();
    // Gets the control's extent
    TRect aRect = Rect();
    gc.Clear(aRect);

	aRect.Width();
    TPoint p;
    TSize picsz = iBitmap->SizeInPixels();

    p.iX = aRect.Width() - picsz.iWidth;
    p.iX/=2;

    p.iY = aRect.Height() - picsz.iHeight;
    p.iY/=2;

    if(iBitmap)
    {
		gc.BitBlt(p,iBitmap);
    }

    }

CCoeControl* CIncallertContainer::ComponentControl(TInt) const
    {
   		return NULL;
    }

CIncallertContainer::CIncallertContainer(CIncallertAppUi* ui)
{
	appui = ui;
}
TKeyResponse CIncallertContainer::HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
	return appui->HandleKeyEventL(aKeyEvent,aType);
}
void CIncallertContainer::LoadBitMap()
	{

		if(iBitmap==NULL)
		{
		TFileName filePath(_L("iclmain.mbm"));
	    #ifndef __WINS__
		CIncallertAppUi::CompleteWithPrivatePathL(filePath);
	    #endif

		 iBitmap= new( ELeave )CFbsBitmap;
	  	 iBitmap->Load( filePath, EMbmIclmainMain, EMbmIclmainMain_mask);
		}


	}

