/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2021 Amebis
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
