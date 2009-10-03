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

#ifndef __INCALLERT_APPUI_H__
#define __INCALLERT_APPUI_H__

#include <aknviewappui.h>
#include <akntabgrp.h>
#include <aknnavide.h>


// Forward reference
class CIncallertAppView;
class CLineStatusHandler;
class CIncallertSettingsView;


/*!
  @class CIncallertAppUi

  @discussion An instance of class CIncallertAppUi is the UserInterface part of the AVKON
  application framework for the Incallert example application
  */
class CIncallertAppUi : public CAknViewAppUi
    {
public:

/*!
  @function ConstructL

  @discussion Perform the second phase construction of a CIncallertAppUi object
  this needs to be public due to the way the framework constructs the AppUi
  */
    void ConstructL();


/*!
  @function CIncallertAppUi

  @discussion Perform the first phase of two phase construction.
  This needs to be public due to the way the framework constructs the AppUi
  */
    CIncallertAppUi();


/*!
  @function ~CIncallertAppUi

  @discussion Destroy the object and release all memory objects
  */
    ~CIncallertAppUi();

    static void CompleteWithPrivatePathL(TDes& des)
    {
    #ifdef EKA2//3rd edition
    	RFs rfs;
    	TFileName fn;

    	User::LeaveIfError(rfs.Connect());
    	CleanupClosePushL(rfs);
    	User::LeaveIfError(rfs.PrivatePath(fn));
    	CleanupStack::PopAndDestroy();
    	fn += des;
    	des = fn;
    #else
    	CompleteWithAppPath(des);
    #endif
    }

#ifdef EKA2
TBool ProcessCommandParametersL( CApaCommandLine &aCommandLine );
#else
TBool ProcessCommandParametersL(TApaCommand aCommand,TFileName& aDocumentName);
#endif

void HandleWsEventL(const TWsEvent &aEvent, CCoeControl *aDestination);//handle red-key press


public: // from CAknAppUi
/*!
  @function HandleCommandL

  @discussion Handle user menu selections
  @param aCommand the enumerated code for the option selected
  */
    void HandleCommandL(TInt aCommand);

//private:
/*! @var iAppView The application view */
    CIncallertAppView* iAppView;
    CIncallertSettingsView* iSettingsView;
    CAknView *iHelpAppView;
    CAknView *iAboutAppView;

    CLineStatusHandler* iLineStatusHandler;

    TBool iLayout;
    TBool iVisible;

    CAknNavigationControlContainer* iNaviPane;
    CAknTabGroup*                   iTabGroup;
    CAknNavigationDecorator*        iDecoratedTabGroup;

    TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

    static TInt ExitTimerCallBack(TAny* that);


    TBool iAutoStarted;

    CPeriodic* iExitTimer;
    TTime iStartTime;

    void DoConstuctL();

    };


#endif // __INCALLERT_APPUI_H__

