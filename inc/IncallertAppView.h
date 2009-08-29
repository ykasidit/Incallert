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

#ifndef __INCALLERT_APPVIEW_H__
#define __INCALLERT_APPVIEW_H__

#include <aknview.h>


class CIncallertAppUi;
class CIncallertContainer;


class CWsClient;
/*!
  @class CIncallertAppView

  @discussion An instance of the Application View object for the Incallert
  example application
  */
class CIncallertAppView :  public CAknView//, CCoeControl, public MCoeView
    {
public:


	CIncallertAppUi *appui;
	TUid Id() const;
	void HandleCommandL(TInt aCommand);
  	TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
  	static CIncallertAppView* NewL(CIncallertAppUi* ui);
  	static CIncallertAppView* NewLC(CIncallertAppUi* ui);
    ~CIncallertAppView();

	void DoActivateL(const TVwsViewId& aPrevViewId,
                   TUid aCustomMessageId,
                   const TDesC8& aCustomMessage);
	void DoDeactivate();

	CIncallertContainer *iContainer;

private:
    CIncallertAppView(CIncallertAppUi *ui);
    void ConstructL();

    TUid iId;


    };


#endif // __INCALLERT_APPVIEW_H__
