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
class wxZRColaUpdater;


#pragma once

#include "zrcolagui.h"

#include <Updater/chkthread.h>

#include <wx/log.h>


///
/// ZRCola updater dialog
///
class wxZRColaUpdater : public wxZRColaUpdaterBase
{
public:
    wxZRColaUpdater(wxWindow* parent);
    virtual ~wxZRColaUpdater();

protected:
    void OnCheckComplete(wxThreadEvent& event);
    virtual void OnUpdate(wxCommandEvent& event);

protected:
    wxLog *m_logOrig;               ///< Original log
    wxUpdCheckThread *m_updater;    ///< Updater thread
};