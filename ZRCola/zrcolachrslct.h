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
class wxZRColaCharSelect;
class wxPersistentZRColaCharSelect;


#pragma once

#include "zrcolagui.h"
#include <zrcola/character.h>
#include <wxex/valhex.h>
#include <wxex/persist/dialog.h>
#include <wx/event.h>
#include <wx/thread.h>
#include <list>
#include <map>
#include <vector>


wxDECLARE_EVENT(wxEVT_SEARCH_COMPLETE, wxThreadEvent);


///
/// ZRCola character select dialog
///
class wxZRColaCharSelect : public wxZRColaCharSelectBase
{
public:
    wxZRColaCharSelect(wxWindow* parent);
    virtual ~wxZRColaCharSelect();

    friend class wxPersistentZRColaCharSelect;  // Allow saving/restoring window state.
    friend class SearchThread;                  // For search thread back-notifications

protected:
    virtual void OnIdle(wxIdleEvent& event);
    virtual void OnSearchText(wxCommandEvent& event);
    virtual void OnSearchMore(wxHyperlinkEvent& event);
    virtual void OnCategoriesAll(wxHyperlinkEvent& event);
    virtual void OnCategoriesNone(wxHyperlinkEvent& event);
    virtual void OnCategoriesInvert(wxHyperlinkEvent& event);
    virtual void OnCategoriesToggle(wxCommandEvent& event);
    void OnSearchComplete(wxThreadEvent& event);
    virtual void OnResultSelectCell(wxGridEvent& event);
    virtual void OnResultCellDClick(wxGridEvent& event);
    virtual void OnResultsKeyDown(wxKeyEvent& event);
    virtual void OnRecentSelectCell(wxGridEvent& event);
    virtual void OnRecentCellDClick(wxGridEvent& event);
    virtual void OnRecentKeyDown(wxKeyEvent& event);
    virtual void OnNavigateBack(wxHyperlinkEvent& event);
    virtual void OnNavigateForward(wxHyperlinkEvent& event);
    virtual void OnUnicodeText(wxCommandEvent& event);
    virtual void OnPreviewKeyDown(wxKeyEvent& event);
    virtual void OnRelatedSelectCell(wxGridEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);

    void ResetResults();
    void NavigateBy(int offset);
    void NavigateTo(wchar_t c);

public:
    wchar_t m_char;                                 ///< Currently selected character (0 when none)

protected:
    bool m_searchChanged;                           ///< Did Search field or category selection change?
    std::map<ZRCola::chrcatid_t, int> m_ccOrder;    ///< Character category order
    bool m_unicodeChanged;                          ///< Did Unicode field change?


    ///
    /// Search worker thread
    ///
    class SearchThread : public wxThread
    {
    public:
        SearchThread(wxZRColaCharSelect *parent);

    protected:
        virtual ExitCode Entry();
        static int __cdecl CompareHits(const void *a, const void *b);
        static bool __cdecl TestDestroyS(void *cookie);

    public:
        std::wstring m_search;                      ///< Search phrase
        std::set<ZRCola::chrcatid_t> m_cats;        ///< Search categories
        std::vector<std::pair<ZRCola::charrank_t, wchar_t> > m_hits; ///< Search results

    protected:
        wxZRColaCharSelect *m_parent;               ///< Thread owner
    } *m_searchThread;                              ///< Search thread


    ///
    /// Navigation state
    ///
    struct NavigationState
    {
        wchar_t m_char;
        struct {
            wxGridCellCoords m_selected;
        } m_related;
    };
    std::list<NavigationState> m_history;                   ///< Navigation history
    std::list<NavigationState>::iterator m_historyCursor;   ///< Navigation history cursor
};



///
/// Supports saving/restoring wxZRColaCharSelect state
///
class wxPersistentZRColaCharSelect : public wxPersistentDialog
{
public:
    wxPersistentZRColaCharSelect(wxZRColaCharSelect *wnd);

    virtual void Save() const;
    virtual bool Restore();
};


inline wxPersistentObject *wxCreatePersistentObject(wxZRColaCharSelect *wnd)
{
    return new wxPersistentZRColaCharSelect(wnd);
}
