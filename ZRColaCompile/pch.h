/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

#pragma once

#include "../include/version.h"
#include "dbsource.h"
#include "parse.h"

#include <wxex/common.h>
#include <wxex/comutils.h>

#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/config.h>
#include <wx/intl.h>
#pragma warning(pop)

#include <zrcola/idrec.h>

#include <WinStd/Common.h>

#include <initguid.h> // GUID helper to prevent LNK2001 errors (unresolved external symbol IID_IADO...)
#pragma warning(push)
#pragma warning(disable: 4091)
#include <adoint.h>
#include <adoid.h>
#pragma warning(pop)

#include <process.h>
#include <tchar.h>

#include <stdlib.h>

#include <algorithm>
#include <codecvt>
#include <cwctype>
#include <fstream>
#include <memory>
#include <set>
#include <vector>
