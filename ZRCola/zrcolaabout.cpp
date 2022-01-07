/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

#include "pch.h"


//////////////////////////////////////////////////////////////////////////
// wxZRColaAbout
//////////////////////////////////////////////////////////////////////////

wxZRColaAbout::wxZRColaAbout(wxWindow* parent) : wxZRColaAboutBase(parent)
{
    m_staticTextVersion->SetLabelText(wxT(PRODUCT_VERSION_STR));

    m_staticTextCopyright->SetLabelText(wxString::Format(_("© 2004-%s ZRC SAZU"), wxT(PRODUCT_BUILD_YEAR_STR)));
}
