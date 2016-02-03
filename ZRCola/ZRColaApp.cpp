/*
    Copyright 2016 Amebis

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

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////////
// ZRColaApp
//////////////////////////////////////////////////////////////////////////

wxIMPLEMENT_APP(ZRColaApp);


bool ZRColaApp::OnInit()
{
    if (wxLocale::IsAvailable(wxLANGUAGE_SLOVENIAN)) {
        wxString sPath(wxPathOnly(argv[0]));
        sPath << wxT("\\..\\locale");
        m_locale.AddCatalogLookupPathPrefix(sPath);
        averify(m_locale.Init(wxLANGUAGE_SLOVENIAN));
        averify(m_locale.AddCatalog(wxT("ZRCola")));
    }

    ZRColaFrame *frame = new ZRColaFrame(_("Hello World"), wxPoint(50, 50), wxSize(450, 340));
    frame->Show(true);
    return true;
}
