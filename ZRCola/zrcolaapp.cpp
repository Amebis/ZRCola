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
#if defined(__WXMSW__)
#pragma comment(lib, "msi.lib")
#endif


//////////////////////////////////////////////////////////////////////////
// ZRColaApp
//////////////////////////////////////////////////////////////////////////

wxIMPLEMENT_APP(ZRColaApp);


ZRColaApp::ZRColaApp() : wxApp()
{
}


bool ZRColaApp::OnInit()
{
#if defined(__WXMSW__)
    // To compensate migration to non-advertised shortcut, do the Microsoft Installer's feature completeness check manually.
    // If execution got this far in the first place (EXE and dependent DLLs are present and loadable).
    // Furthermore, this increments program usage counter.
    if (::MsiQueryFeatureState(_T(ZRCOLA_VERSION_GUID), _T("featZRCola")) != INSTALLSTATE_UNKNOWN)
        ::MsiUseFeature(_T(ZRCOLA_VERSION_GUID), _T("featZRCola"));
#endif

    wxConfigBase *cfgPrev = wxConfigBase::Set(new wxConfig(wxT(ZRCOLA_CFG_APPLICATION), wxT(ZRCOLA_CFG_VENDOR)));
    if (cfgPrev) wxDELETE(cfgPrev);

    if (!wxApp::OnInit())
        return false;

    // Set desired locale.
    wxLanguage language = (wxLanguage)wxConfigBase::Get()->Read(wxT("Language"), wxLANGUAGE_DEFAULT);
    if (wxLocale::IsAvailable(language)) {
        wxString sPath;
        if (wxConfigBase::Get()->Read(wxT("LocalizationRepositoryPath"), &sPath))
            m_locale.AddCatalogLookupPathPrefix(sPath);
        wxVERIFY(m_locale.Init(language));
        wxVERIFY(m_locale.AddCatalog(wxT("wxExtend")));
        wxVERIFY(m_locale.AddCatalog(wxT("ZRCola")));
    }

    wxZRColaFrame* mainFrame = new wxZRColaFrame();
    wxPersistentRegisterAndRestore<wxTopLevelWindow>(mainFrame);
    mainFrame->Show();

    return true;
}
