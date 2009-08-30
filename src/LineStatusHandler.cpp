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


#include "LineStatusHandler.h"

#include <Incallert.rsg>
#include "Incallert.hrh"
#include <aknnotewrappers.h>
#include <commdb.h>

#define MILLION 1000000

enum TMyCall
	{
	CallNone,
	CallVoice,
	CallFax,
	CallData,
	CallAlerting,
	CallRinging,
	CallAlternating,
	//3 new consts
	CallDialling,
	CallAnswering,
	CallDisconnecting
	};

CLineStatusHandler* CLineStatusHandler::NewL(CIncallertAppUi* ui)
{

	CLineStatusHandler* ret = NewLC(ui);
	CleanupStack::Pop(ret);
	return ret;
}
CLineStatusHandler* CLineStatusHandler::NewLC(CIncallertAppUi *ui)
{
	//thehandler = handler;

	CLineStatusHandler* ret =  new (ELeave) CLineStatusHandler;
	CleanupStack::PushL(ret);
	ret->ConstructL();
	ret->appui = ui;
	return ret;
}

TInt CLineStatusHandler::Start()
{


	Cancel();
	iLine.NotifyStatusChange(iStatus,iCallStatus);
    SetActive();

    return KErrNone;

}
CLineStatusHandler::CLineStatusHandler() : CActive(EPriorityStandard)
{

	failed = EFalse;
	loaded = EFalse;
	inCall = EFalse;
	callSeconds=0;
	preAlertTimeSeconds = 10;
	cycleTimeMinutes = 1;
	startMinute = 1;



}

void CLineStatusHandler::ConstructL()
{
	CActiveScheduler::Add(this);

	iPeriodicPreCycle = CPeriodic::NewL(EPriorityHigh);
	iPeriodicCycle = CPeriodic::NewL(EPriorityHigh);

	TBuf<96> aTsyName;
	CCommsDatabase* db = CCommsDatabase::NewL(EDatabaseTypeUnspecified);
	CleanupStack::PushL(db);
	CCommsDbTableView* table = db->OpenTableLC(TPtrC(MODEM));
	table->GotoFirstRecord();
	table->ReadTextL(TPtrC(MODEM_TSY_NAME),aTsyName);
	CleanupStack::PopAndDestroy(2); // table,db


		User::LeaveIfError(iTelServer.Connect());
		User::LeaveIfError(iTelServer.LoadPhoneModule(aTsyName));
		User::LeaveIfError(iTelServer.GetPhoneInfo(0,iPhoneInfo));
		User::LeaveIfError(iPhone.Open(iTelServer,iPhoneInfo.iName));
		User::LeaveIfError(iPhone.GetLineInfo(0,iLineInfo));
		User::LeaveIfError(iLine.Open(iPhone,iLineInfo.iName));


}

CLineStatusHandler::~CLineStatusHandler()
{
	Cancel();

	iLine.Close();
	iPhone.Close();
	iTelServer.Close();

	if(iPeriodicPreCycle)
	{
		iPeriodicPreCycle->Cancel();
		delete iPeriodicPreCycle;
		iPeriodicPreCycle = NULL;
	}
	if(iPeriodicCycle)
	{
		iPeriodicCycle->Cancel();
		delete iPeriodicCycle;
		iPeriodicCycle = NULL;
	}
}






//From CActive
void CLineStatusHandler::RunL()
{
	if (iStatus != KErrNone)
	{
		Start();
		return;
	}




				switch(iCallStatus)
				{
					case RCall::EStatusHangingUp:
					case RCall::EStatusIdle:
					{
						inCall = EFalse;
						iPeriodicPreCycle->Cancel();
						iPeriodicCycle->Cancel();

					}
					break;
					case RCall::EStatusConnected:
					{

						iConnectedTime.HomeTime();

						inCall = ETrue;
						iPeriodicPreCycle->Cancel();
						iPeriodicCycle->Cancel();
						startCall.HomeTime();
						if(preAlertTimeSeconds>0)
							iPeriodicPreCycle->Start(   ( ((startMinute)*60)-preAlertTimeSeconds)*MILLION   ,cycleTimeMinutes*60*MILLION,TCallBack(PeriodicPreCycleCallBack,this));

						iPeriodicCycle->Start((startMinute)*60*MILLION,cycleTimeMinutes*60*MILLION,TCallBack(PeriodicCycleCallBack,this));


					}
					break;

					default:
					{
					}
					break;
				}



		Start();
}


void CLineStatusHandler::DoCancel()
{
	iLine.NotifyStatusChangeCancel();

	if(iPeriodicPreCycle)
		{
			iPeriodicPreCycle->Cancel();
		}
	if(iPeriodicCycle)
		{
			iPeriodicCycle->Cancel();
		}
}

void CLineStatusHandler::ShowDurAndMsg(const TDesC& aStr, TNotifyType aType)
{
	TTime now;
	now.HomeTime();
	TTimeIntervalSeconds secs(0);
	if(KErrNone == now.SecondsFrom(iConnectedTime,secs))
	{
		TBuf<96> buf;
		_LIT(KDurFormatStr,"%02d:%02d - ");
		buf.Format(KDurFormatStr,(secs.Int()/60),(secs.Int()%60));
		buf += aStr;
		if(aType == EWarning)
		{
		CAknWarningNote* informationNote = new (ELeave) CAknWarningNote(ETrue);
		informationNote->ExecuteLD(buf);
		}
		else
		{
		CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(ETrue);
		informationNote->ExecuteLD(buf);
		}
	}
	else
	{

		CAknWarningNote* informationNote = new (ELeave) CAknWarningNote(ETrue);
		informationNote->ExecuteLD(aStr);
	}
}
TInt CLineStatusHandler::RunError(TInt aError)
{
	if(aError)
		;
	return KErrNone;
}

TInt CLineStatusHandler::PeriodicPreCycleCallBack(TAny* that)
{

	if(((CLineStatusHandler*)that)->inCall)
	{
		RCall::TStatus status;
		((CLineStatusHandler*)that)->iLine.GetStatus(status);
		if(status == RCall::EStatusConnected)
			{

			((CLineStatusHandler*)that)->inCall = ETrue;

			///////////////////
			TBool isvisible = EFalse;
			if(CEikonEnv::Static()->RootWin().OrdinalPosition() == 0 && CEikonEnv::Static()->RootWin().OrdinalPriority() >= ECoeWinPriorityNormal)
				isvisible = ETrue;
			if(!isvisible)
			{
				StartPopUp();
			}
			///////////////////

			_LIT(msg,"new minute coming...");
			((CLineStatusHandler*)that)->ShowDurAndMsg(msg, EWarning);

			///////////////////
			if(!isvisible)
			{
				EndPopUp();
			}
			///////////////////

				return 1;
			}
	}
		((CLineStatusHandler*)that)->iPeriodicPreCycle->Cancel();
		((CLineStatusHandler*)that)->inCall = EFalse;

		return 1;
}

void CLineStatusHandler::StartPopUp()
{
	CCoeEnv::Static()->RootWin().SetOrdinalPosition(0, ECoeWinPriorityAlwaysAtFront+114);
	TApaTask task(CEikonEnv::Static()->WsSession( ));
	task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
	task.BringToForeground();
}

void CLineStatusHandler::EndPopUp()
{
	CCoeEnv::Static()->RootWin().SetOrdinalPosition(0, ECoeWinPriorityNormal);
	TApaTask task(CEikonEnv::Static()->WsSession( ));
	task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
	task.SendToBackground();
}

TInt CLineStatusHandler::PeriodicCycleCallBack(TAny* that)
	{
			if(((CLineStatusHandler*)that)->inCall)
			{
					RCall::TStatus status;
					((CLineStatusHandler*)that)->iLine.GetStatus(status);
					if(status == RCall::EStatusConnected)
					{
						((CLineStatusHandler*)that)->inCall = ETrue;

						///////////////////
						TBool isvisible = EFalse;
						if(CEikonEnv::Static()->RootWin().OrdinalPosition() == 0 && CEikonEnv::Static()->RootWin().OrdinalPriority() >= ECoeWinPriorityNormal)
							isvisible = ETrue;

						if(!isvisible)
						{
							StartPopUp();
						}
						////////////////////////

						_LIT(msg,"New Minute");
						((CLineStatusHandler*)that)->ShowDurAndMsg(msg, EInfo);

						///////////////////
						if(!isvisible)
						{
							EndPopUp();
						}
						///////////////////

						return 1;
					}
			}

		((CLineStatusHandler*)that)->iPeriodicCycle->Cancel();
		((CLineStatusHandler*)that)->inCall = EFalse;

		return 1;
	}

void CLineStatusHandler::SetPrefs(TUint  preSeconds,
	TUint  cycleMinutes,
	TUint  startMin)
	{
		this->preAlertTimeSeconds = preSeconds;
		this->cycleTimeMinutes = cycleMinutes;
		this->startMinute = startMin;

	}
