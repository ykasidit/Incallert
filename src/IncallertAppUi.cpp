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

			    _LIT(msg,"Incallert's call monitor failed to start");
			    CAknErrorNote* informationNote = new (ELeave) CAknErrorNote(ETrue);
			    //informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
			   	informationNote->ExecuteLD(msg);
	        	Exit();
	    	}


    }

CIncallertAppUi::CIncallertAppUi()
    {
	iStartTime.HomeTime();
    }

CIncallertAppUi::~CIncallertAppUi()
    {
     delete iDecoratedTabGroup;
     delete iLineStatusHandler;
     delete iExitTimer;

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

	 switch(aCommand)
	 {
	 case EAknSoftkeyExit:
	 {
	             _LIT(msg,"Deactivating...");
	         	CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
	         	informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
	         	informationNote->ExecuteLD(msg);
	 }
	 //flow through to next case
	 case EEikCmdExit:
	 {
	    if(!iAutoStarted)
			Exit();
		else
			{
				TTime now;
				now.HomeTime();
				TTimeIntervalMicroSeconds diff = now.MicroSecondsFrom(iStartTime);

				if(diff<=TTimeIntervalMicroSeconds(5))
				{

				//auto started apps must not exit sooner than 5 secs or phone might say our app has corrupted
				iEikonEnv->RootWin().SetOrdinalPosition(-1,ECoeWinPriorityNeverAtFront);
				if(!iExitTimer)//if not already started exit timer
				{
					iExitTimer = CPeriodic::NewL(0);
					iExitTimer->Start( 5000000, 5000000, TCallBack(ExitTimerCallBack,this));
				}

				}
				else
					Exit(); //app was already started long enough
			}

	    return;
	 }
	 default:
	 break;
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


#ifdef EKA2
TBool CIncallertAppUi::ProcessCommandParametersL( CApaCommandLine &aCommandLine )
#else
TBool CIncallertAppUi::ProcessCommandParametersL(TApaCommand aCommand,TFileName& aDocumentName)
#endif
{

	#ifdef EKA2
		if(aCommandLine.OpaqueData().Length() > 0)
	#else
		if(aDocumentName.Length() > 0)
	#endif
	  {
	      // Opaque data exists, app. has been manually started from the menu
	  	iAutoStarted = EFalse;
	  	iEikonEnv->RootWin().SetOrdinalPosition(-1,ECoeWinPriorityNormal);

		TApaTask task(iEikonEnv->WsSession( ));
		task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
		task.BringToForeground();
	  }
	  else
	  {
		iAutoStarted = ETrue;

		iEikonEnv->RootWin().SetOrdinalPosition(-1,ECoeWinPriorityNormal);

		TApaTask task(iEikonEnv->WsSession( ));
		task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
		task.SendToBackground();

		if(!CIncallertSettingsView::AutoStartFilePresent())
		{
			//if noautostart file exists then prepare exit
			if(!iExitTimer)
			{
				iExitTimer = CPeriodic::NewL(0);
				iEikonEnv->RootWin().SetOrdinalPosition(-1,ECoeWinPriorityNeverAtFront);
				iExitTimer->Start( 5000000, 5000000, TCallBack(ExitTimerCallBack,this));
			}
		}

	  }

#ifdef EKA2
   return CEikAppUi::ProcessCommandParametersL( aCommandLine );
#else
   return CEikAppUi::ProcessCommandParametersL( aCommand,aDocumentName );
#endif
}

TInt CIncallertAppUi::ExitTimerCallBack(TAny* that)
{
	((CIncallertAppUi*)that)->iExitTimer->Cancel();
	((CIncallertAppUi*)that)->Exit();
	return 1;
}

void CIncallertAppUi::HandleWsEventL(const TWsEvent &aEvent, CCoeControl *aDestination)
	{
		switch (aEvent.Type())
		{
			//case KAknUidValueEndKeyCloseEvent:  0x101F87F0
			case 0x101F87F0:
			{
				TApaTask task(iEikonEnv->WsSession( ));
				task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
				task.SendToBackground();
			}
			break;

			default:
			{
			CAknAppUi::HandleWsEventL(aEvent, aDestination);
			}
		}
	}

