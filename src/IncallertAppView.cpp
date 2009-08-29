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
#include <aknnotewrappers.h>
#include <eikmenup.h>
#include "IncallertAppui.h"

#include "LineStatusHandler.h"
#include "IncallertAppView.h"
#include "IncallertApplication.h"
#include "Incallert.hrh"
#include "IncallertContainer.h"



TUid CIncallertAppView::Id() const
	{
	   return iId;
	}

// Standard construction sequence
CIncallertAppView* CIncallertAppView::NewL(CIncallertAppUi* ui)
	{
	    CIncallertAppView* self = CIncallertAppView::NewLC(ui);
	    CleanupStack::Pop(self);
	    return self;
    }

CIncallertAppView* CIncallertAppView::NewLC(CIncallertAppUi* ui)
    {

	    CIncallertAppView* self = new (ELeave) CIncallertAppView(ui);
	    CleanupStack::PushL(self);
	    self->ConstructL();
	    return self;
    }


CIncallertAppView::CIncallertAppView(CIncallertAppUi *ui)
    {
		iId = TUid::Uid(ENavigationPaneStatusViewId);
		appui = ui;
    }

CIncallertAppView::~CIncallertAppView()
    {
	// no implementation required
    }

void CIncallertAppView::ConstructL()
    {
	    BaseConstructL(R_STATUS_VIEW);

    }


void CIncallertAppView::HandleCommandL(TInt aCommand)
    {
         appui->HandleCommandL(aCommand);
    }

void CIncallertAppView::DoActivateL(const TVwsViewId& /*aPrevViewId*/,
                                    TUid /*aCustomMessageId*/,
                                    const TDesC8& /*aCustomMessage*/)
    {
	    ASSERT(iContainer == NULL);



		iContainer = CIncallertContainer::NewL(ClientRect(),appui);
		AppUi()->AddToStackL(iContainer);
    }

void CIncallertAppView::DoDeactivate()
    {
       if (iContainer)
        {

	    AppUi()->RemoveFromStack(iContainer);
        delete iContainer;
        iContainer = NULL;
        }
    }

