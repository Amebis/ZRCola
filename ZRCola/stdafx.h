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

#include "../include/zrcola.h"
#include "zrcolaapp.h"
#include "zrcolacomppnl.h"
#include "zrcolafrm.h"
#include "zrcolakeyhndlr.h"

#include <wx/ffile.h>
#include <wx/msgdlg.h>
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wxex/common.h>

#include <fstream>
#include <string>
#include <vector>

#include <stdex/idrec.h>

#if defined(__WXMSW__)
#include <Msi.h>
#include <ShObjIdl.h>
#include <ShlGuid.h>
#endif
