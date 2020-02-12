/*
    Copyright 2015-2020 Amebis

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

#include <wxex/common.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include "zrcolagui.h"
#pragma warning(pop)
#include <Updater/chkthread.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include <wx/log.h>
#pragma warning(pop)


///
/// ZRCola updater dialog
///
class wxZRColaUpdater : public wxZRColaUpdaterBase
{
public:
    wxZRColaUpdater(wxWindow* parent);
    virtual ~wxZRColaUpdater();

protected:
    //void OnCheckComplete(wxThreadEvent& event);
    virtual void OnIdle(wxIdleEvent& event);
    virtual void OnUpdate(wxCommandEvent& event);
    virtual void OnClose(wxCommandEvent& event);

protected:
    wxLogLevel m_logLevelOrig;      ///< Original log level
    wxLog *m_logOrig;               ///< Original log
    wxUpdCheckThread *m_updater;    ///< Updater thread
    bool m_finished;                ///< Is Updater finished?
};
