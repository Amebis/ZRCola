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
class wxZRColaCharSelect;
class wxPersistentZRColaCharSelect;


#pragma once

#include <wxex/common.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include "zrcolagui.h"
#pragma warning(pop)
#include <zrcola/character.h>
#include <wxex/persist/toplevel.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include <wx/validate.h>
#include <wx/event.h>
#include <wx/thread.h>
#pragma warning(pop)
#include <list>
#include <map>
#include <string>
#include <vector>


///
/// Validator for Unicode character
///
class wxZRColaUTF16CharValidator : public wxValidator
{
public:
    ///
    /// Construct the validator with a value to store data
    ///
    wxZRColaUTF16CharValidator(wchar_t *val = NULL);

    ///
    /// Copies this validator
    ///
    virtual wxObject* Clone() const;

    ///
    /// Validates the value
    ///
    virtual bool Validate(wxWindow *parent);

    ///
    /// Transfers the value to the window
    ///
    virtual bool TransferToWindow();

    ///
    /// Transfers the value from the window
    ///
    virtual bool TransferFromWindow();

    ///
    /// Parses FQDN value
    ///
    static bool Parse(const wxString &val_in, size_t i_start, size_t i_end, wxTextCtrl *ctrl, wxWindow *parent, wchar_t *val_out = NULL);

protected:
    wchar_t *m_val; ///< Pointer to variable to receive control's parsed value

private:
    wxDECLARE_DYNAMIC_CLASS(wxZRColaUTF16CharValidator);
    wxDECLARE_NO_ASSIGN_CLASS(wxZRColaUTF16CharValidator);
};


///
/// Validator for Unicode dump
///
class wxZRColaUnicodeDumpValidator : public wxValidator
{
public:
    ///
    /// Construct the validator with a value to store data
    ///
    wxZRColaUnicodeDumpValidator(wxString *val = NULL);

    ///
    /// Copies this validator
    ///
    virtual wxObject* Clone() const;

    ///
    /// Validates the value
    ///
    virtual bool Validate(wxWindow *parent);

    ///
    /// Transfers the value to the window
    ///
    virtual bool TransferToWindow();

    ///
    /// Transfers the value from the window
    ///
    virtual bool TransferFromWindow();

    ///
    /// Parses Unicode dump value
    ///
    static bool Parse(const wxString &val_in, size_t i_start, size_t i_end, wxTextCtrl *ctrl, wxWindow *parent, wxString *val_out = NULL);

protected:
    wxString *m_val; ///< Pointer to variable to receive control's parsed value

private:
    wxDECLARE_DYNAMIC_CLASS(wxZRColaUnicodeDumpValidator);
    wxDECLARE_NO_ASSIGN_CLASS(wxZRColaUnicodeDumpValidator);
};


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
    virtual void OnInsertUpdateUI(wxUpdateUIEvent& event);
    virtual void OnInsert(wxCommandEvent& event);

    void ResetResults();
    void NavigateBy(int offset);
    void NavigateTo(const wxString &c);

public:
    wxString m_char;                                ///< Currently selected character (empty when none)

protected:
    LCID m_locale;                                  ///< Locale for tag lookup
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
        std::vector<std::pair<ZRCola::charrank_t, std::wstring> > m_hits; ///< Search results

    protected:
        wxZRColaCharSelect *m_parent;               ///< Thread owner
    } *m_searchThread;                              ///< Search thread


    ///
    /// Navigation state
    ///
    struct NavigationState
    {
        std::wstring m_char;
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
class wxPersistentZRColaCharSelect : public wxPersistentTLWEx
{
public:
    wxPersistentZRColaCharSelect(wxZRColaCharSelect *wnd);

    virtual void Save() const;
    virtual bool Restore();

protected:
    static const int s_guiLevel;    ///< Persisted placements/sizing are considered incompatible between different GUI levels
};


inline wxPersistentObject *wxCreatePersistentObject(wxZRColaCharSelect *wnd)
{
    return new wxPersistentZRColaCharSelect(wnd);
}
