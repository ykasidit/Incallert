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

#ifndef SETTINGSLISTSETTINGS_H
#define SETTINGSLISTSETTINGS_H

// INCLUDES

// System Includes
#include <e32base.h>
#include <in_sock.h>

// User Includes
#include "Incallert.hrh"
#include <s32strm.h>
// FORWARD DECLARATIONS


// CLASS DECLARATION


class TSettingsListSettings
	{

public: // accessors



	TInt iCycleTime;//minutes
	TInt iStartMinute;
	TInt iPreCycleTime;//seconds


	TSettingsListSettings();


	void ExternalizeL(RWriteStream &stream);

	void InternalizeL(RReadStream &stream);


	};

#endif	// #ifndef SETTINGSLISTSETTINGS_H

// End of File
