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

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////////
// ZRColaApp
//////////////////////////////////////////////////////////////////////////

wxIMPLEMENT_APP(ZRColaApp);


bool ZRColaApp::OnInit()
{
    if (!wxAppEx::OnInit())
        return false;

    if (wxLocale::IsAvailable(wxLANGUAGE_SLOVENIAN))
        wxVERIFY(m_locale.AddCatalog(wxT("ZRCola")));

    wxZRColaFrame* mainFrame = new wxZRColaFrame();
    if (!mainFrame->Create()) {
        delete mainFrame;
        return false;
    }

    mainFrame->Show();

    return true;
}
