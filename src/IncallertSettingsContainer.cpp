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

#include "IncallertSettingsView.h"
#include "IncallertSettingsContainer.h"
#include "IncallertAppui.h"
#include <eikapp.h>
#include "SettingsListSettingItemList.h"
#include <eiklbx.h>
#include "SettingsListSettingItemList.h"
#include <Incallert.rsg>
#include <eikenv.h>
#include <s32file.h>


CIncallertSettingsContainer* CIncallertSettingsContainer::NewL(const TRect& aRect,CIncallertAppUi *ui)
    {
    CIncallertSettingsContainer* self = CIncallertSettingsContainer::NewLC(aRect,ui);
    CleanupStack::Pop(self);
    return self;
    }

CIncallertSettingsContainer* CIncallertSettingsContainer::NewLC(const TRect& aRect,CIncallertAppUi *ui)
    {
    CIncallertSettingsContainer* self = new (ELeave) CIncallertSettingsContainer(ui);
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    return self;
    }

void CIncallertSettingsContainer::GetIniPath(TFileName& aIniFileName)
	{

	TFileName app = CEikonEnv::Static()->EikAppUi()->Application()->AppFullName();
	TParsePtr parse(app);
	#ifndef __WINS__
		aIniFileName.Copy(parse.DriveAndPath());
	#endif
	aIniFileName.Append(parse.Name());
	_LIT(KIni, ".ini");
	aIniFileName.Append(KIni);
	}

void CIncallertSettingsContainer::ConstructL(const TRect& aRect)
    {


    CreateWindowL();

    //settingsFileName = iCoeEnv->AllocReadResourceL(R_SETTINGSFILE_STR);

    TFileName fname;
    GetIniPath(fname);

    RFs fs = iCoeEnv->FsSession();
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    RFileReadStream rfrs;
    if(rfrs.Open(fs,fname,EFileRead) == KErrNone)
    {
		CleanupClosePushL(rfrs);
    	iSettings.InternalizeL(rfrs);
    	CleanupStack::PopAndDestroy();
   	}
	CleanupStack::PopAndDestroy();
	SetRect(aRect);

    iSettingItemList = new (ELeave) CSettingsListSettingItemList (iSettings);
//    iSettingItemList->SetMopParent(this);
    iSettingItemList->ConstructFromResourceL(R_SETTINGSLIST_SETTING_ITEM_LIST);
  //  iSettingItemList->ListBox()->CreateScrollBarFrameL(ETrue);
//    iSettingItemList->ListBox()->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,CEikScrollBarFrame::EOn);

//	iSettingItemList->ListBox()->UpdateScrollBarsL();

	ActivateL();

    }

TInt CIncallertSettingsContainer::CountComponentControls() const
    {
    return 1;
    //return 0;
    }

void CIncallertSettingsContainer::Draw(const TRect& aRect) const
    {
    // Get the standard graphics context
 	CWindowGc& gc = SystemGc();

    // Gets the control's extent

    gc.Clear(aRect);
//    iSettingItemList->DrawDeferred();

/*    const CFont* font = CEikonEnv::Static()->TitleFont();
	    gc.UseFont(font);
	    gc.SetPenColor(0x009911);
	    gc.SetPenStyle(CGraphicsContext::ESolidPen);
	    TPoint p(50,50);
	    _LIT(msg,"ALLAHUAKBAR!");
	    gc.DrawText(msg,p);
	    gc.DiscardFont();*/

   }

CCoeControl* CIncallertSettingsContainer::ComponentControl(TInt aIndex) const
    {
     switch (aIndex)
		{
		case 0:
			return iSettingItemList;
		default:
			return NULL;
		}

    }

    CIncallertSettingsContainer::CIncallertSettingsContainer(CIncallertAppUi*)
    {
//    	appui = ui;
//   	    this->SetMopParent(appui->iSettingsView);
    }
    CIncallertSettingsContainer::~CIncallertSettingsContainer()
    {
    	//delete settingsFileName;
    	delete iSettingItemList;
    	iSettingItemList = NULL;
    }
    TKeyResponse CIncallertSettingsContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
		TKeyResponse ret = EKeyWasNotConsumed;



		if(iSettingItemList)
			if((aKeyEvent.iCode!= EKeyLeftArrow && aKeyEvent.iCode!= EKeyRightArrow))
				ret = iSettingItemList->OfferKeyEventL(aKeyEvent, aType);



		return ret;
	}

	void CIncallertSettingsContainer::ChangeSelectedItemL()
	{
	if (iSettingItemList)
		iSettingItemList->ChangeSelectedItemL();
	}
	void CIncallertSettingsContainer::SaveSettingsL()
	{
        	TFileName fname;

    		GetIniPath(fname);
    	    TChar drivechar = fname[0];
    		RFs fs = iCoeEnv->FsSession();
        	User::LeaveIfError(fs.Connect());
        	CleanupClosePushL(fs);
        	TInt dint;
        	fs.CharToDrive(drivechar,dint);
        	TVolumeInfo vol;
        	fs.Volume(vol,dint);

        	if(vol.iFree<5) //save file is 5 bytes
					{
						fs.Close();
						User::Leave(KErrDiskFull);
					}

        	RFileWriteStream rfws;
        	TInt err=KErrNone;
        	err = rfws.Create(fs,fname,EFileWrite);
    		if(err == KErrAlreadyExists)
    			{
    				err=KErrNone;
    				err=rfws.Open(fs,fname,EFileWrite);
    			}

    		if(err==KErrNone)
    			{
    				CleanupClosePushL(rfws);
    				iSettings.ExternalizeL(rfws);
    				CleanupStack::PopAndDestroy();
    			}

    		CleanupStack::PopAndDestroy();

	}
