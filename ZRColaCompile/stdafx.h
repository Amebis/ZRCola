/*
    Copyright 2015-2016 Amebis

    This file is part of ZRCola.

    ZRCola is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ZRCola is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ZRCola. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#define _ATL_NO_AUTOMATIC_NAMESPACE

#include "../include/zrcola.h"
#include "dbsource.h"

#include <zrcola/language.h>
#include <zrcola/translate.h>
#include <zrcolaui/chargroup.h>
#include <zrcolaui/keyboard.h>

#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/config.h>
#include <wx/intl.h>

#include <wxex/common.h>
#include <wxex/comutils.h>

#include <stdex/idrec.h>

#include <initguid.h> // GUID helper to prevent LNK2001 errors (unresolved external symbol IID_IADO...)
#include <adoint.h>
#include <adoid.h>
#include <atlcomcli.h>

#include <tchar.h>

#include <stdlib.h>

#include <algorithm>
#include <codecvt>
#include <cwctype>
#include <fstream>
#include <memory>
#include <set>
#include <vector>
