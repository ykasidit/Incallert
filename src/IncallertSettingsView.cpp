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

#include <eikenv.h>
#include <coemain.h>
#include <Incallert.rsg>

#include "IncallertAppui.h"

#include "LineStatusHandler.h"
#include "IncallertSettingsView.h"
#include "IncallertApplication.h"
#include "SettingsListSettingItemList.h"

#include "Incallert.hrh"
#include <s32file.h>
#include <aknnotewrappers.h>
#include <aknmessagequerydialog.h>
#include <e32std.h>
#include "SettingsListSettingItemList.h"
#include "SettingsListSettings.h"
#include "SettingsListSettings.h"
#include <f32file.h>
#include <eikfutil.h>

#include <coeutils.h>




TUid CIncallertSettingsView::Id() const
	{
	   return iId;
	}

// Standard construction sequence
CIncallertSettingsView* CIncallertSettingsView::NewL(CIncallertAppUi *ui)
	{
	    CIncallertSettingsView* self = CIncallertSettingsView::NewLC( ui);
	    CleanupStack::Pop(self);
	    return self;
    }

CIncallertSettingsView* CIncallertSettingsView::NewLC(CIncallertAppUi *ui)
    {
	    CIncallertSettingsView* self = new (ELeave) CIncallertSettingsView(ui);
	    CleanupStack::PushL(self);
	    self->ConstructL();

	    return self;
    }


CIncallertSettingsView::CIncallertSettingsView(CIncallertAppUi *ui)
    {
		iId = TUid::Uid(ENavigationPaneSettingsViewId);
		appui = ui;
    }

CIncallertSettingsView::~CIncallertSettingsView()
    {
	// no implementation required
    }

void CIncallertSettingsView::ConstructL()
    {

	BaseConstructL(R_SETTINGS_VIEW);

    }

// Draw this application's view to the screen




TKeyResponse CIncallertSettingsView::HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
	return appui->HandleKeyEventL(aKeyEvent,aType);
}


void CIncallertSettingsView::HandleCommandL(TInt aCommand)
    {
    switch(aCommand)
        {
        case EEikCmdExit: //handle same as avkon exit case
        case EAknSoftkeyExit:
            {
            	appui->HandleCommandL(aCommand);
            }
            break;

        case EAknSoftkeyBack:
            {

            	//data saved in DeactivateL
            	appui->iTabGroup->SetActiveTabById(ENavigationPaneStatusViewId);
    			appui->ActivateLocalViewL(TUid::Uid(appui->iTabGroup->ActiveTabId()));
            }
            break;
        case EIncallertSetToDefaultsCommand:
        	{
    				if(iContainer)
    		 			{
//((CSettingsListSettingItemList*)iContainer)->iSettings.iAutoStart = 1;
//change to check file
((CSettingsListSettingItemList*)iContainer)->iSettings.iCycleTime = 1;
((CSettingsListSettingItemList*)iContainer)->iSettings.iStartMinute =1;
((CSettingsListSettingItemList*)iContainer)->iSettings.iPreCycleTime = 10;
((CSettingsListSettingItemList*)iContainer)->LoadSettingsL();
    					}
        	};
        	break;
        case EIncallertHelpSettingsCommand:
        	{
            	appui->iTabGroup->SetActiveTabById(ENavigationPaneHelpViewId);
    			appui->ActivateLocalViewL(TUid::Uid(appui->iTabGroup->ActiveTabId()));

        	};
        	break;
        case EIncallertSelectCommand:
        	{
((CSettingsListSettingItemList*)iContainer)->ChangeSelectedItemL();
		    }
  		    break;
        default:
            //Panic(EIncallertBasicUi);
            break;
        }
    }

void CIncallertSettingsView::DoActivateL(const TVwsViewId& /*aPrevViewId*/,
                                    TUid /*aCustomMessageId*/,
                                    const TDesC8& /*aCustomMessage*/)
    {


    _LIT(KIni,"Incallert.ini");
    TFileName fname(KIni);
	#ifndef __WINS__
		CIncallertAppUi::CompleteWithPrivatePathL(fname);
	#endif

    RFs fs = iCoeEnv->FsSession();
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    RFileReadStream rfrs;

    if(rfrs.Open(fs,fname,EFileRead) == KErrNone)
    {
		TRAPD(err,

		CleanupClosePushL(rfrs);
    	iSettings.InternalizeL(rfrs);
    	CleanupStack::PopAndDestroy();

    	);
   	}
	CleanupStack::PopAndDestroy();



		 iContainer = new (ELeave) CSettingsListSettingItemList (iSettings);
		 iContainer->SetMopParent(this);

		 ((CSettingsListSettingItemList*)iContainer)->ConstructFromResourceL(R_SETTINGSLIST_SETTING_ITEM_LIST);
		 //iContainer->CreateScrollBarFrameL();
//	     iContainer->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
		 iContainer->SetRect(ClientRect());

		 iContainer->ActivateL();

	    //iContainer = CIncallertSettingsContainer::NewL(ClientRect(),appui);
	    AppUi()->AddToStackL(*this,iContainer);

    }

TBool CIncallertSettingsView::AutoStartEnabled()
	{
#ifndef EKA2
		#ifdef __WINS__
  		_LIT(path,"D:\\Symbian\\Series60_1_2_CW\\epoc32\\release\\WINSCW\\UDEB\\Z\\SYSTEM\\RECOGS\\Iclrstrt.mdl");
  		#else
	    _LIT(path,"\\system\\Recogs\\Iclrstrt.mdl");
	    #endif

   	    TFileName mdlFile(path);

		#ifndef __WINS__
   	 CIncallertAppUi::CompleteWithPrivatePathL(mdlFile);
	    #endif

		return ConeUtils::FileExists(mdlFile);
#else
		_LIT(KNoASFileFlag,"noautostart.dat");
		TFileName fn(KNoASFileFlag);
		CIncallertAppUi::CompleteWithPrivatePathL(fn);

		return !(ConeUtils::FileExists(fn)); //in 3rd, if we have this file then hide and exit app
#endif
	}

void CIncallertSettingsView::SaveSettingsL()
	{

		if(iContainer)
        {


    			_LIT(KIni,"Incallert.ini");
    TFileName fname(KIni);
	#ifndef __WINS__
    CIncallertAppUi::CompleteWithPrivatePathL(fname);
	#endif


    		RFs fs = iCoeEnv->FsSession();
        	User::LeaveIfError(fs.Connect());
        	CleanupClosePushL(fs);

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
    				((CSettingsListSettingItemList*)iContainer)->iSettings.ExternalizeL(rfws);
    				CleanupStack::PopAndDestroy();
    			}

    		CleanupStack::PopAndDestroy();


        }

	}

void CIncallertSettingsView::DoDeactivate()
    {
    if (iContainer)
        {



		TRAPD(err,SaveSettingsL());

		//apply auto start settings







		if(err!=KErrNone)
			{

				_LIT(msg,"Save Failed. Please free some space.");
        		CAknErrorNote* informationNote = new (ELeave) CAknErrorNote(ETrue);
        		informationNote->ExecuteLD(msg);

			}
		else if(appui->iLineStatusHandler->inCall)
			{
				_LIT(msg,"Changes apply to next calls");
        		CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
        		informationNote->ExecuteLD(msg);
			}

     	appui->iLineStatusHandler->SetPrefs(((CSettingsListSettingItemList*)iContainer)->iSettings.iPreCycleTime,((CSettingsListSettingItemList*)iContainer)->iSettings.iCycleTime,((CSettingsListSettingItemList*)iContainer)->iSettings.iStartMinute);
		AppUi()->RemoveFromStack(iContainer);
        delete iContainer;
        iContainer = NULL;
        }


    }
