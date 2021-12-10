/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2021 Amebis
*/

#pragma once

#include "../include/version.h"
#include "zrcolaabout.h"
#include "zrcolaapp.h"
#include "zrcolachrcatpnl.h"
#include "zrcolachrgrid.h"
#include "zrcolachrreq.h"
#include "zrcolacomppnl.h"
#include "zrcolafrm.h"
#include "zrcolakeyhndlr.h"
#include "zrcolasettings.h"
#include "zrcolatranseq.h"
#include "zrcolaupdater.h"

#include <Updater/chkthread.h>

#include <wxex/common.h>
#include <wxex/url.h>
#include <wxex/persist/auimanager.h>
#include <wxex/persist/toplevel.h>

#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include <wx/clipbrd.h>
#include <wx/dcclient.h>
#include <wx/ffile.h>
#include <wx/msgdlg.h>
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/socket.h>
#include <wx/tokenzr.h>
#include <wx/utils.h>
#include <wx/valtext.h>
#pragma warning(pop)

#include <WinStd/MSI.h>

#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include <stdex/idrec.h>

#if defined(__WXMSW__)
#include <Msi.h>
#include <ShObjIdl.h>
#include <ShlGuid.h>
#endif
