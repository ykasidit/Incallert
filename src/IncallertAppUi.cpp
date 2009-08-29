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

#include <avkon.hrh>
#include <aknnotewrappers.h>
#include <e32base.h>
#include <aknnotedialog.h>
//#include <apgtask.h>
#include <aknglobalmsgquery.h>
#include "IncallertAppui.h"
#include "IncallertAppView.h"
#include "IncallertSettingsView.h"
#include <Incallert.rsg>
#include "Incallert.hrh"
#include "LineStatusHandler.h"
#include "IncallertApplication.h"
#include "HelpAppView.h"

#include <aknmessagequerydialog.h>
#include "SettingsListSettings.h"

#include "AboutAppView.h"
//#include <plpvariant.h>
#include <s32file.h>

void CIncallertAppUi::ConstructL()
    {

#ifdef EKA2
  BaseConstructL(EAknEnableSkin);
#else
  //BaseConstructL(KEnableSkinFlag | KLayoutAwareFlag);
  BaseConstructL(0x08 | 0x1000);
#endif


    iAppView = CIncallertAppView::NewL(this);
    iSettingsView = CIncallertSettingsView::NewL(this);
    iLineStatusHandler = CLineStatusHandler::NewL(this);

    iHelpAppView = CHelpAppView::NewL( ENavigationPaneHelpViewId);
    iAboutAppView = CAboutAppView::NewL( ENavigationPaneAboutViewId);

	_LIT(KIni,"Incallert.ini");
    TFileName fname(KIni);
	#ifndef __WINS__
    CompleteWithPrivatePathL(fname);
	#endif
    TSettingsListSettings settings;

    TRAPD(err,
    RFs fs = iCoeEnv->FsSession();

    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    RFileReadStream rfrs;
    if(rfrs.Open(fs,fname,EFileRead) == KErrNone)
    {
	    CleanupClosePushL(rfrs);
    	settings.InternalizeL(rfrs);
    	CleanupStack::PopAndDestroy();
   	}
	CleanupStack::PopAndDestroy();
	);

    iLineStatusHandler->SetPrefs(settings.iPreCycleTime,settings.iCycleTime,settings.iStartMinute);


    AddViewL(iAboutAppView);
    AddViewL(iSettingsView); // transfer ownership to base class
    AddViewL(iHelpAppView);
    AddViewL(iAppView);    // transfer ownership to base class







 	CEikStatusPane* sp = StatusPane();
    iNaviPane = (CAknNavigationControlContainer *)sp->ControlL(TUid::Uid(EEikStatusPaneUidNavi));
    sp->SetDimmed(ETrue);

	iDecoratedTabGroup = iNaviPane->ResourceDecorator();

    if (iDecoratedTabGroup)
        {
        iTabGroup = (CAknTabGroup*) iDecoratedTabGroup->DecoratedControl();
        }

    if (iTabGroup)
        {
    	//iTabGroup->SetActiveTabById(ENavigationPaneStatusViewId);
    	//ActivateViewL(TVwsViewId(KUidIncallertApp,TUid::Uid(iTabGroup->ActiveTabId())));
        }



	    SetDefaultViewL(*iAppView);
	    _LIT(msg,"Activating...");
	    CAknInformationNote* activateInformationNote = new (ELeave) CAknInformationNote(ETrue);
		activateInformationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
		activateInformationNote->ExecuteLD(msg);

		TInt ret = iLineStatusHandler->Start();
		if(ret!=KErrNone)
	    	{

			    _LIT(msg,"Incallert's system inspector failed to start");
			    CAknErrorNote* informationNote = new (ELeave) CAknErrorNote(ETrue);
			    //informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
			   	informationNote->ExecuteLD(msg);
	        	Exit();
	    	}


    }

CIncallertAppUi::CIncallertAppUi()
    {

    }

CIncallertAppUi::~CIncallertAppUi()
    {
     delete iDecoratedTabGroup;
     delete iLineStatusHandler;

    }


 TKeyResponse CIncallertAppUi::HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
 {

 	 TKeyResponse ret = iTabGroup->OfferKeyEventL(aKeyEvent,aType);

 	 if(ret == EKeyWasConsumed)
 	 	ActivateLocalViewL(TUid::Uid(iTabGroup->ActiveTabId()));

 	 return ret;
 }

 void CIncallertAppUi::HandleCommandL(TInt aCommand)
    {

	    if(aCommand == EEikCmdExit)
    	{
	        Exit();
	        return;
	    }
	    if(aCommand ==  EAknSoftkeyExit)
	    {
            _LIT(msg,"Deactivating...");
        	CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
        	informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
        	informationNote->ExecuteLD(msg);
        	Exit();
	        return;
	   	}

    if(CAknViewAppUi::iView == iAppView)
    {
	    switch(aCommand)
	        {


	        case EIncallertSettingsCommand:
	        	{

		iTabGroup->SetActiveTabById(ENavigationPaneSettingsViewId);
	    ActivateLocalViewL(TUid::Uid(iTabGroup->ActiveTabId()));

	        	};
	        	break;

	        	case EAknSoftkeyDone:
		        {

				 _LIT(msg,"Switching to Background");
	        	CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
	        	informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
	        	informationNote->ExecuteLD(msg);

				 TApaTask task(iEikonEnv->WsSession( ));
				 task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
				 task.SendToBackground();
		        }
	        break;


	        case EIncallertHelpCommand:
	        	{

				iTabGroup->SetActiveTabById(ENavigationPaneHelpViewId);
				ActivateLocalViewL(TUid::Uid(iTabGroup->ActiveTabId()));


	        	};
	        	break;
	        case EIncallertAboutCommand:
	        	{

	        	iTabGroup->SetActiveTabById(ENavigationPaneAboutViewId);
				ActivateLocalViewL(TUid::Uid(iTabGroup->ActiveTabId()));

	        	};
	        	break;

	        default:
	            break;
	        }
    	}
    	else
    	if(CAknViewAppUi::iView == iHelpAppView || CAknViewAppUi::iView == iAboutAppView)
    	{
			iTabGroup->SetActiveTabById(ENavigationPaneStatusViewId);
			ActivateLocalViewL(TUid::Uid(iTabGroup->ActiveTabId()));
    	}

    }

