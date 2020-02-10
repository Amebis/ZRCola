/*
    Copyright 2015-2019 Amebis

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

#include "pch.h"


//////////////////////////////////////////////////////////////////////////
// wxZRColaAbout
//////////////////////////////////////////////////////////////////////////

wxZRColaAbout::wxZRColaAbout(wxWindow* parent) : wxZRColaAboutBase(parent)
{
    m_staticTextVersion->SetLabelText(wxT(PRODUCT_VERSION_STR));

    m_staticTextCopyright->SetLabelText(wxString::Format(_("© 2004-%s ZRC SAZU"), wxT(PRODUCT_BUILD_YEAR_STR)));
}
