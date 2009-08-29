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

#ifndef __INCALLERT_SETTINGSVIEW_H__
#define __INCALLERT_SETTINGSVIEW_H__

#include <aknview.h>
#include "SettingsListSettings.h"

class CIncallertAppUi;
class CIncallertSettingsContainer;



class CIncallertSettingsView :  public CAknView
    {
public:

	CIncallertAppUi *appui;	
	TUid Id() const;
	void HandleCommandL(TInt aCommand);
  	TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
  	static CIncallertSettingsView* NewL(CIncallertAppUi *ui);	
  	static CIncallertSettingsView* NewLC(CIncallertAppUi *ui);
    ~CIncallertSettingsView();
    static TBool AutoStartFilePresent();
	//void HandleForegroundEventL(TBool fg);
	//void HandleStatusPaneSizeChange();
	void DoActivateL(const TVwsViewId& aPrevViewId,
                   TUid aCustomMessageId,
                   const TDesC8& aCustomMessage);
	void DoDeactivate();
	void SaveSettingsL();
	TSettingsListSettings iSettings;

private:
    CIncallertSettingsView(CIncallertAppUi *ui);
    void ConstructL();
    CCoeControl *iContainer;
    TUid iId;       
    
    };


#endif // __INCALLERT_SETTINGSVIEW_H__
