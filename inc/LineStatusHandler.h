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

#include <e32base.h>
#include <e32std.h>
//#include <saclient.h>
#include <aknappui.h>
#include <aknglobalnote.h>
#include <etel.h>
#include <mdaaudiosampleplayer.h>

class CIncallertAppUi;

class CLineStatusHandler : CActive, public MMdaAudioPlayerCallback
{

	public:
	static CLineStatusHandler* NewL(CIncallertAppUi *ui);
	static CLineStatusHandler* NewLC(CIncallertAppUi *ui);
	TInt Start();

	static void StartPopUp();
	static void EndPopUp();

	enum TNotifyType
	{
		EWarning,
		EInfo
	};

	void ShowDurAndMsg(const TDesC& aStr,TNotifyType aType);

	~CLineStatusHandler();

	void ConstructL();
	TBool failed;
	TBool loaded;
	TBool inCall;
	TUint callSeconds;
	RCall::TStatus iCallStatus;

	//From CActive
	TUint  preAlertTimeSeconds;
	TUint  cycleTimeMinutes;
	TUint  startMinute;


	void SetPrefs(TUint  preAlertTime,
	TUint  cycleTime,
	TUint  startMinute);

	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
	TTime startCall;
	static TInt PeriodicCycleCallBack(TAny* that);
	static TInt PeriodicPreCycleCallBack(TAny* that);
	CPeriodic *iPeriodicPreCycle;
	CPeriodic *iPeriodicCycle;
	CAknGlobalNote* globalNote;
	CIncallertAppUi *appui;

	///////////////
	RTelServer iTelServer;

	RTelServer::TPhoneInfo iPhoneInfo;
	RPhone iPhone;

	RPhone::TLineInfo iLineInfo;
	RLine iLine;
	//////////////////

	TInt curCycleTime;
	TInt curStartMinute;
	TBool curPreCycleAlertEnabled;
	TInt curPreCycleTime;

	TTime iConnectedTime;

	void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration);
	void MapcPlayComplete(TInt aError);

	private:
	CLineStatusHandler();

	CMdaAudioPlayerUtility *iAtMinutePlayer, *iPreMinutePlayer;
	TBool iSoundInitOk;


};
