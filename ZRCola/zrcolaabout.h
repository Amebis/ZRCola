/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

///
/// Forward declarations
///
class wxZRColaAbout;


#pragma once

#include <wxex/common.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include "zrcolagui.h"
#pragma warning(pop)


///
/// About ZRCola dialog
///
class wxZRColaAbout : public wxZRColaAboutBase
{
public:
    wxZRColaAbout(wxWindow* parent);
};
