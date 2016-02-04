/*
    Copyright 2016 Amebis

    This file is part of wxExtend.

    wxExtend is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    wxExtend is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with wxExtend. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <Windows.h>
#include <ShlObj.h>

#include <wx/frame.h>


#define wxABT_AUTOHIDETIMERID       1
#define wxABT_AUTOHIDETIME          300
#define wxABT_AUTOHIDETIMERINTERVAL 3000


///
/// Dockable application bar
///
class wxAppBar : public wxFrame
{
public:
    ///
    /// Application bar states
    ///
    enum wxState {
        wxSTATE_UNKNOWN = -2,
        wxSTATE_FLOAT   = -1,
        wxSTATE_LEFT    = ABE_LEFT,
        wxSTATE_TOP     = ABE_TOP,
        wxSTATE_RIGHT   = ABE_RIGHT,
        wxSTATE_BOTTOM  = ABE_BOTTOM,
    };


    ///
    /// Application bar flags
    ///
    enum wxFlags {
        wxFLAG_ALLOWFLOAT                  = 1 <<  0,  ///< Is floating application bar allowed?
        wxFLAG_ALLOWDOCKTOP                = 1 <<  1,  ///< Is docking on top of the screen allowed?
        wxFLAG_ALLOWDOCKBOTTOM             = 1 <<  2,  ///< Is docking on bottom of the screen allowed?
        wxFLAG_ALLOWDOCKLEFT               = 1 <<  3,  ///< Is docking left of the screen allowed?
        wxFLAG_ALLOWDOCKRIGHT              = 1 <<  4,  ///< Is docking right of the screen allowed?
        wxFLAG_ALWAYSONTOP                 = 1 <<  5,  ///< Is application bar always on top?
        wxFLAG_ALWAYSONTOPTASKBAR          = 1 <<  6,  ///< Does application bar follow always on top state of the system taskbar?
        wxFLAG_AUTOHIDE                    = 1 <<  7,  ///< Is application bar setup for auto-hide?
        wxFLAG_AUTOHIDETASKBAR             = 1 <<  8,  ///< Does application bar follow auto-hide state of the system taskbar?
        wxFLAG_ALLOWSIZING                 = 1 <<  9,  ///< Is application bar's sizing allowed?
        wxFLAG_HIDETASKBARTABWHENFLOATING  = 1 << 10,  ///< Should application bar's tab on the system taskbar hide when floating?
        wxFLAG_HIDETASKBARTABWHENDOCKED    = 1 << 11,  ///< Should application bar's tab on the system taskbar hide when docked?

        // Internal flags
        wxFLAG_FULLSCREENAPPOPEN           = 1 << 12,  ///< Is full-screen application open?
        wxFLAG_AUTOHIDDEN                  = 1 << 13,  ///< Is application bar auto-hidden right now?
        wxFLAG_POSITIONSET                 = 1 << 14,  ///< Is application bar's position correct?

        wxFLAG_ALLOWDOCKANY                = (wxFLAG_ALLOWDOCKTOP | wxFLAG_ALLOWDOCKBOTTOM | wxFLAG_ALLOWDOCKLEFT | wxFLAG_ALLOWDOCKRIGHT),  ///< Is docking at any edge of the screen allowed?
        wxFLAG_ALLOWDOCKHORIZONTAL         = (wxFLAG_ALLOWDOCKTOP | wxFLAG_ALLOWDOCKBOTTOM),                                               ///< Is docking on top or bottom of the screen allowed?
        wxFLAG_ALLOWDOCKVERTICAL           = (wxFLAG_ALLOWDOCKLEFT | wxFLAG_ALLOWDOCKRIGHT),                                               ///< Is docking left or right of the screen allowed?
        wxFLAG_ALLOWANY                    = (wxFLAG_ALLOWFLOAT | wxFLAG_ALLOWDOCKANY),                                                    ///< Is floating and docking at any edge of the screen allowed?
        wxFLAG_ALWAYSHIDETASKBARTAB        = (wxFLAG_HIDETASKBARTABWHENFLOATING | wxFLAG_HIDETASKBARTABWHENDOCKED),                        ///< Should application bar's tab on the system taskbar hide always?
    };

public:
    wxAppBar();
    wxAppBar(wxWindow *parent,
        wxWindowID id,
        const wxString& title,
        wxState state = wxSTATE_FLOAT,
        int flags = wxFLAG_ALLOWANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxFrameNameStr);

    bool Create(wxWindow *parent,
        wxWindowID id,
        const wxString& title,
        wxState state = wxSTATE_FLOAT,
        int flags = wxFLAG_ALLOWANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxFrameNameStr);

    virtual ~wxAppBar();

public:
    /// \name Application bar general management
    /// @{

    ///
    /// Returns current state of the application bar.
    ///
    inline wxState GetState() const;

    ///
    /// Returns whether application bar is displayed always on top.
    ///
    /// \returns
    /// - true when application bar is always on top
    /// - false otherwise
    ///
    inline bool IsAlwaysOnTop() const;

    ///
    /// Sets whether application bar is displayed always on top.
    ///
    /// \param[in] alwaysOnTop Logical variable to control whether always-on-top behaviour is to be enabled (true) or disabled (false).
    ///
    inline bool SetAlwaysOnTop(bool alwaysOnTop = true);

    ///
    /// Returns if application bar is configured for auto-hide.
    ///
    /// \returns
    /// - true if application bar is configured for auto-hide
    /// - false otherwise
    ///
    inline bool IsAutoHide() const;

    ///
    /// Sets whether application bar is displayed always on top.
    ///
    /// \param[in] autoHide Logical variable to control whether auto-hide behaviour is to be enabled (true) or disabled (false).
    ///
    inline bool SetAutoHide(bool autoHide = true);

    ///
    /// Returns if sizing of the application bar is allowed.
    ///
    /// \returns
    /// - true if sizing of the application bar is allowed
    /// - false otherwise
    ///
    inline bool GetAllowSizing() const;

    ///
    /// Sets whether resizing of the application bar is allowed.
    ///
    /// \param[in] bAllowSizing Logical variable to control whether resizing is to be enabled (true) or disabled (false).
    ///
    inline bool SetAllowSizing(bool bAllowSizing = true);

    ///
    /// Returns if application bar is auto-hidden right now.
    ///
    /// \returns
    /// - true if application bar is auto-hidden right now
    /// - false otherwise
    inline bool GetAutoHidden() const;

    //bool GetAppBarPlacement(LPAPPBARPLACEMENT lpPlacement) const;
    //bool SetAppBarPlacement(LPCAPPBARPLACEMENT lpPlacement);

    ///
    /// Minimize application bar to the edge of the desktop.
    ///
    /// \param[in] state The edge at which to dock. Must be either of: wxSTATE_LEFT, wxSTATE_TOP, wxSTATE_RIGHT, or wxSTATE_BOTTOM.
    /// \param[in] wnd When the undocked and docked window is different, this parameter denotes the undocked version.
    ///
    void MinimiseToEdge(wxState state, wxWindow* wnd = NULL);

    ///
    /// Restore application bar from the edge of the desktop.
    ///
    /// \param[in] rect The desired coordinates of the restored window. If NULL internally saved coordinates are used.
    /// \param[in] wnd When the undocked and docked window is different, this parameter denotes the undocked version.
    ///
    void MaximiseFromEdge(const RECT* rect = NULL);
    void MaximiseFromEdge(wxWindow *wnd);

    ///
    /// Shows or hides auto-hide application bar.
    ///
    /// \param[in] bShow Logical variable to control whether application bar is be shown (true) or hidden (false).
    ///
    void ShowAutoHideAppBar(bool bShow = true);

    ///
    /// Hides auto-hide application bar.
    ///
    inline void HideAutoHideAppBar() { ShowAutoHideAppBar(false); }

    /// @}

protected:
    /// \name Notification handlers
    /// @{

    ///
    /// Notification handler when the new state of the application bar is proposed.
    ///
    /// Derived classes can implement their own. However, overriden virtual method should call the parent's implementation too.
    ///
    /// \param[in] stateNew New state about to take effect.
    ///
    virtual void OnChangeState(wxState stateNew);

    ///
    /// Notification handler when the Windows' task bar state changes.
    ///
    /// Derived classes can implement their own. However, overriden virtual method should call the parent's implementation too.
    ///
    /// \param[in] state New task bar state.
    ///
    virtual void OnChangeTaskBarState(UINT_PTR state);

    ///
    /// Notification handler when desktop windows are being arranged (cascaded, tiled, ...)
    ///
    /// Derived classes can implement their own. However, overriden virtual method should call the parent's implementation too.
    ///
    /// \param[in] bBeginning true when windows arrangement starts and false when arrangement ends.
    ///
    virtual void OnWindowsArrange(bool bBeginning);

    ///
    /// Called when application bar was forced from auto-hide to normal docking.
    ///
    /// Default implementation displays a warning dialog.
    ///
    virtual void OnAutoHideDenied();

    /// @}

protected:
    virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

private:
    inline bool DockAppBar(wxState state);
    inline bool UndockAppBar();
    inline bool RegisterAutoHide(wxState state);
    inline bool UnregisterAutoHide(wxState state);
    inline bool GetDockedRect(wxState state, LPRECT rect) const;
    inline bool GetAutoHideRect(wxState state, bool bAutoHidden, LPRECT rect) const;
    inline static bool IsDocked(wxState state);
    inline static bool IsDockedVert(wxState state);
    inline static bool IsDockedHoriz(wxState state);
    inline static WXHWND GetZWnd(wxState state, int flags);
    inline static UINT_PTR GetTaskBarState();

protected:
    wxState m_state;                ///< Current state of the application bar
    wxState m_stateDesired;         ///< Desired state of the application bar while moving/resizing
    int m_flags;                    ///< Flags describing application bar's behaviour

    RECT m_rect;                    ///< Window rectangle
    SIZE m_sizeFloat;               ///< Window size when floating (we need it to restore floating size, when we undock)
    SIZE m_sizeDocked;              ///< Size of the window when docked (height when wxSTATE_TOP or wxSTATE_BOTTOM, width when wxSTATE_LEFT or wxSTATE_RIGHT)
    SIZE m_sizeMin;                 ///< Minimum window size

    UINT_PTR m_stateTaskBar;        ///< TaskBar's current state
    UINT_PTR m_timerID;             ///< Application bar's timer id.

    ITaskbarList *m_taskbarList;    ///< Windows's taskbar list interface

private:
    static const UINT WM_AB_NOTIFY; ///< Application bar notification message
};


inline wxAppBar::wxState wxAppBar::GetState() const
{
    return m_stateDesired != wxSTATE_UNKNOWN ? m_stateDesired : m_state;
}


inline bool wxAppBar::IsAlwaysOnTop() const
{
    return (m_flags & wxFLAG_ALWAYSONTOP) ? true : false;
}


inline bool wxAppBar::SetAlwaysOnTop(bool alwaysOnTop)
{
    if (alwaysOnTop)
        m_flags |= wxFLAG_ALWAYSONTOP;
    else
        m_flags &= ~wxFLAG_ALWAYSONTOP;

    if (::IsWindowVisible(m_hWnd)) {
        // Set the Z-order. SWP_NOSENDCHANGING flag prevents our OnWindowPosChanging() method to be called, since moving is not necessary.
        wxCHECK(::SetWindowPos(m_hWnd, GetZWnd(m_state, m_flags), 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOSENDCHANGING), false);
    }

    return true;
}


inline bool wxAppBar::IsAutoHide() const
{
    return (m_flags & wxFLAG_AUTOHIDE) ? true : false;
}


inline bool wxAppBar::SetAutoHide(bool autoHide)
{
    bool autoHidePrev = IsAutoHide();

    if (autoHide)
        m_flags |= wxFLAG_AUTOHIDE;
    else
        m_flags &= ~wxFLAG_AUTOHIDE;

    if (autoHidePrev != autoHide && IsDocked(m_state) && ::IsWindowVisible(m_hWnd)) {
        if (autoHide) {
            // Application bar should be auto-hidden. Try to setup it so.

            // We undock to free the space before registering auto-hide, otherwise system will leave application bar's space reserved.
            UndockAppBar();

            // Register auto-hide application bar.
            if (!RegisterAutoHide(m_state)) {
                // Auto-hide failed; some other application bar must be hidden on this edge already. Dock back instead.
                DockAppBar(m_state);
                return false;
            }
        } else {
            // Unregister the auto-hidden application bar and dock.
            UnregisterAutoHide(m_state);
            DockAppBar(m_state);
        }
    }

    return true;
}


inline bool wxAppBar::GetAllowSizing() const
{
    return (m_flags & wxFLAG_ALLOWSIZING) ? true : false;
}


inline bool wxAppBar::SetAllowSizing(bool bAllowSizing)
{
    if (bAllowSizing)
        m_flags |= wxFLAG_ALLOWSIZING;
    else
        m_flags &= ~wxFLAG_ALLOWSIZING;

    return true;
}


inline bool wxAppBar::GetAutoHidden() const
{
    return (m_flags & wxFLAG_AUTOHIDDEN) ? true : false;
}


inline bool wxAppBar::DockAppBar(wxState state)
{
    wxASSERT(IsDocked(state));

    APPBARDATA abd = {
        sizeof(abd),
        m_hWnd,
        0,
        state
    };

    // Calculate docked window rect and dock the window there.
    GetDockedRect(state, &(abd.rc));
    wxCHECK(::SHAppBarMessage(ABM_SETPOS, &abd), false);
    m_flags |= wxFLAG_POSITIONSET;
    m_rect = abd.rc;
    wxCHECK(::SetWindowPos(m_hWnd, GetZWnd(state, m_flags), 0, 0, 0, 0, SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_FRAMECHANGED), false);

    return true;
}


inline bool wxAppBar::UndockAppBar()
{
    APPBARDATA abd = {
        sizeof(abd),
        m_hWnd,
        0,
        wxSTATE_LEFT
    };

    // Free application bar's space to undock.
    wxASSERT(!abd.rc.left && !abd.rc.top && !abd.rc.right && !abd.rc.bottom);
    wxCHECK(::SHAppBarMessage(ABM_SETPOS, &abd), false);

    return true;
}


inline bool wxAppBar::RegisterAutoHide(wxState state)
{
    wxASSERT(IsDocked(state));

    APPBARDATA abd = {
        sizeof(abd),
        m_hWnd,
        0,
        state,
        {},
        (LPARAM)true
    };

    // Register application bar as auto-hide.
    if (::SHAppBarMessage(ABM_SETAUTOHIDEBAR, &abd)) {
        // Auto-hide succeeded.
        m_flags |=  wxFLAG_AUTOHIDE;
        m_flags &= ~wxFLAG_AUTOHIDDEN;

        // Calculate auto-hidden window rect and move the window there.
        GetAutoHideRect(state, false, &(abd.rc));
        m_flags |= wxFLAG_POSITIONSET;
        m_rect = abd.rc;
        wxCHECK(::SetWindowPos(m_hWnd, GetZWnd(state, m_flags), 0, 0, 0, 0, SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_FRAMECHANGED), false);

        // Reset auto-hide timer.
        m_timerID = ::SetTimer(m_hWnd, wxABT_AUTOHIDETIMERID, wxABT_AUTOHIDETIMERINTERVAL, NULL);

        return true;
    } else {
        // Auto-hide failed; some other application bar must be hidden on this edge already.
        m_flags &= ~(wxFLAG_AUTOHIDE | wxFLAG_AUTOHIDDEN);

        // Post a message to user.
        OnAutoHideDenied();

        return false;
    }
}


inline bool wxAppBar::UnregisterAutoHide(wxState state)
{
    wxASSERT(IsDocked(state));

    APPBARDATA abd = {
        sizeof(abd),
        m_hWnd,
        0,
        state,
        {},
        (LPARAM)false
    };

    // Unregister application bar as auto-hide.
    wxCHECK(::SHAppBarMessage(ABM_SETAUTOHIDEBAR, &abd), false);
    m_flags &= ~wxFLAG_AUTOHIDDEN;

    // Cancel the timer.
    if (m_timerID) {
        ::KillTimer(m_hWnd, m_timerID);
        m_timerID = 0;
    }

    return true;
}


inline bool wxAppBar::GetDockedRect(wxState state, LPRECT rect) const
{
    wxASSERT(rect);
    wxASSERT(IsDocked(state));

    APPBARDATA abd = {
        sizeof(abd),
        m_hWnd,
        0,
        state,
        {
            // Set dimensions to full screen.
            0,
            0,
            ::GetSystemMetrics(SM_CXSCREEN),
            ::GetSystemMetrics(SM_CYSCREEN)
        }
    };

    wxCHECK(::SHAppBarMessage(ABM_QUERYPOS, &abd), false);

    // Correct our dimensions accordingly.
    switch (state) {
        case wxSTATE_LEFT:
            rect->left   = abd.rc.left;
            rect->top    = abd.rc.top;
            rect->right  = abd.rc.left + m_sizeDocked.cx;
            rect->bottom = abd.rc.bottom;
            break;

        case wxSTATE_TOP:
            rect->left   = abd.rc.left;
            rect->top    = abd.rc.top;
            rect->right  = abd.rc.right;
            rect->bottom = abd.rc.top + m_sizeDocked.cy;
            break;

        case wxSTATE_RIGHT:
            rect->left   = abd.rc.right - m_sizeDocked.cx;
            rect->top    = abd.rc.top;
            rect->right  = abd.rc.right;
            rect->bottom = abd.rc.bottom;
            break;

        case wxSTATE_BOTTOM:
            rect->left   = abd.rc.left;
            rect->top    = abd.rc.bottom - m_sizeDocked.cy;
            rect->right  = abd.rc.right;
            rect->bottom = abd.rc.bottom;
            break;

        default:
            // Unknown state.
            wxFAIL;
            return false;
    }

    return true;
}


inline bool wxAppBar::GetAutoHideRect(wxState state, bool bAutoHidden, LPRECT rect) const
{
    wxASSERT(rect);
    wxASSERT(IsDocked(state));

    // Keep a part of the application bar visible at all times
    const int iBorder = ::GetSystemMetrics(IsDockedVert(state) ? SM_CXBORDER : SM_CYBORDER) * 2;
    const RECT rcScreen = {
        0,
        0,
        ::GetSystemMetrics(SM_CXSCREEN),
        ::GetSystemMetrics(SM_CYSCREEN)
    };

    // Correct our dimensions accordingly.
    switch (state) {
        case wxSTATE_LEFT:
            rect->top    = rcScreen.top;
            rect->bottom = rcScreen.bottom;
            rect->right  = rcScreen.left + (bAutoHidden ? iBorder : m_sizeDocked.cx);
            rect->left   = rect->right - m_sizeDocked.cx;
            break;

        case wxSTATE_TOP:
            rect->left   = rcScreen.left;
            rect->right  = rcScreen.right;
            rect->bottom = rcScreen.top + (bAutoHidden ? iBorder : m_sizeDocked.cy);
            rect->top    = rect->bottom - m_sizeDocked.cy;
            break;

        case wxSTATE_RIGHT:
            rect->left   = rcScreen.right - (bAutoHidden ? iBorder : m_sizeDocked.cx);
            rect->top    = rcScreen.top;
            rect->right  = rect->left + m_sizeDocked.cx;
            rect->bottom = rcScreen.bottom;
            break;

        case wxSTATE_BOTTOM:
            rect->left   = rcScreen.left;
            rect->top    = rcScreen.bottom - (bAutoHidden ? iBorder : m_sizeDocked.cy);
            rect->right  = rcScreen.right;
            rect->bottom = rect->top + m_sizeDocked.cy;
            break;

        default:
            // Unknown state.
            wxFAIL;
            return false;
    }

    return true;
}


inline bool wxAppBar::IsDocked(wxState state)
{
    return wxSTATE_LEFT <= state && state <= wxSTATE_BOTTOM;
}


inline bool wxAppBar::IsDockedVert(wxState state)
{
    return state == wxSTATE_LEFT || state == wxSTATE_RIGHT;
}


inline bool wxAppBar::IsDockedHoriz(wxState state)
{
    return state == wxSTATE_TOP || state == wxSTATE_BOTTOM;
}


inline WXHWND wxAppBar::GetZWnd(wxState state, int flags)
{
    if (flags & wxFLAG_FULLSCREENAPPOPEN) {
        // If full screen app is open, clear the always-on-top flag.
        return HWND_NOTOPMOST;
    }

    //_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
    //if (::IsWindow(pThreadState->m_hTrackingWindow)) {
    //    // We're tracking another window/menu, position behind it.
    //    return pThreadState->m_hTrackingWindow;
    // }

    if (state == wxSTATE_FLOAT) {
        // When floating, decide according to the flags.
        return (flags & wxFLAG_ALWAYSONTOP) ? HWND_TOPMOST : HWND_NOTOPMOST;
    }

    if (IsDocked(state)) {
        if (flags & wxFLAG_AUTOHIDE) {
            // Auto-hidden docked application bar is always on top.
            return HWND_TOPMOST;
        }
        
        if (flags & wxFLAG_ALWAYSONTOP) {
            // Regular docked always-on-top application bar.
            WXHWND hWndTaskbar = ::FindWindow(_T("Shell_TrayWnd"), _T(""));
            if (hWndTaskbar) {
                // We found the taskbar window. Position directly after it. Otherwise our window overlaps Vista's Start button orb.
                return hWndTaskbar;
            } else {
                // Only always-on-top windows are allowed to resize properly outside the desktop rect.
                // If window is docked and not always on top, resizing a window to smaller size is obstructed by desktop rect limit.
                return HWND_TOPMOST;
            }
        }


        // Regular docked non-always-on-top application bar.
        return HWND_NOTOPMOST;
    } else {
        // Unknown state. Do nothing.
        wxFAIL;
        return HWND_TOP;
    }
}


inline UINT_PTR wxAppBar::GetTaskBarState()
{
    APPBARDATA abd = { sizeof(abd) };

    return ::SHAppBarMessage(ABM_GETSTATE, &abd);
}
