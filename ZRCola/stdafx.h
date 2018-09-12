/*
    Copyright 2015-2018 Amebis

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
