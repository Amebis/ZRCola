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

///
/// Forward declarations
///
class ZRColaApp;


#pragma once

#include <wx/app.h>
#include <wx/config.h>
#include <wx/intl.h>


///
/// ZRCola application
///
class ZRColaApp : public wxApp
{
public:
    ZRColaApp();

    ///
    /// Called when application initializes.
    ///
    /// \returns
    /// - true if initialization succeeded
    /// - false otherwise
    virtual bool OnInit();

protected:
    wxConfig m_config;  ///< Application configuration
    wxLocale m_locale;  ///< Current locale
};


wxDECLARE_APP(ZRColaApp);
