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

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////////
// wxAppBar
//////////////////////////////////////////////////////////////////////////

const UINT wxAppBar::WM_AB_NOTIFY = ::RegisterWindowMessage(_T("AppBarNotify"));


wxAppBar::wxAppBar() :
    m_timerID(0),
    m_taskbarList(NULL),
    wxFrame()
{
}


wxAppBar::wxAppBar(wxWindow *parent, wxWindowID id, const wxString& title, wxState state, int flags, const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
    m_timerID(0),
    m_taskbarList(NULL),
    wxFrame()
{
    Create(parent, id, title, state, flags, pos, size, style, name);
}


bool wxAppBar::Create(wxWindow *parent, wxWindowID id, const wxString& title, wxState state, int flags, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
{
    // Save initial window rectangle.
    m_rect.left   = pos.x;
    m_rect.top    = pos.y;
    m_rect.right  = pos.x + size.x;
    m_rect.bottom = pos.y + size.y;

    // Save initial floating window size.
    m_sizeFloat.cx = size.x;
    m_sizeFloat.cy = size.y;

    // Save initial docked window width/height.
    m_sizeDocked.cx = size.x;
    m_sizeDocked.cy = size.y;

    // Save initial state and flags.
    m_state        = state;
    m_stateDesired = wxSTATE_UNKNOWN;
    m_flags        = flags;

    // Find the taskbar list's interface.
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList, (LPVOID*)&m_taskbarList)), false);
    wxCHECK(SUCCEEDED(m_taskbarList->HrInit()), false);

    // If any taskbar list tab's hiding is in affect, set application bar's style as tool window to avoid being displayed on the taskbar initially.
    if (m_flags & wxFLAG_ALWAYSHIDETASKBARTAB)
        style |= wxFRAME_TOOL_WINDOW;

    // Create frame.
    wxCHECK(wxFrame::Create(parent, id, title, pos, size, style, name), false);

    // Register our application bar.
    APPBARDATA abd = { sizeof(abd), m_hWnd, WM_AB_NOTIFY, m_state, };
    wxCHECK(::SHAppBarMessage(ABM_NEW, &abd), false);

    // Get the state of the Windows taskbar.
    m_stateTaskBar = GetTaskBarState();

    if (m_flags & wxFLAG_AUTOHIDETASKBAR) {
        // Mimic the auto-hide state of taskbar.
        SetAutoHide((m_stateTaskBar & ABS_AUTOHIDE) ? true : false);
    }

    if (m_flags & wxFLAG_ALWAYSONTOPTASKBAR) {
        // Mimic the always-on-top state of taskbar.
        SetAlwaysOnTop((m_stateTaskBar & ABS_ALWAYSONTOP) ? true : false);
    }

    return true;
}


wxAppBar::~wxAppBar()
{
    wxASSERT(m_timerID == 0);

    // Remove the application bar.
    APPBARDATA abd = {
        sizeof(abd),
        m_hWnd
    };
    wxVERIFY(::SHAppBarMessage(ABM_REMOVE, &abd));

    if (m_taskbarList)
        m_taskbarList->Release();
}


//bool wxAppBar::GetAppBarPlacement(LPAPPBARPLACEMENT lpPlacement) const
//{
//    wxASSERT(lpPlacement);
//
//    if (lpPlacement->dwSize >= sizeof(APPBARPLACEMENT1)) {
//        lpPlacement->dwState = (DWORD)m_state;
//        lpPlacement->dwFlags = m_flags;
//        if (m_state == wxSTATE_FLOAT) {
//            // Current application bar's rect is floating rect.
//            lpPlacement->rcFloat = m_rect;
//        } else if (IsDocked(m_state)) {
//            // Application bar is docked. Calc floating rect from m_sizeFloat.
//            lpPlacement->rcFloat.left   = 0;
//            lpPlacement->rcFloat.top    = 0;
//            lpPlacement->rcFloat.right  = m_sizeFloat.cx;
//            lpPlacement->rcFloat.bottom = m_sizeFloat.cy;
//        } else {
//            // Application bar's state is unknown.
//            return false;
//        }
//        lpPlacement->szDocked = m_sizeDocked;
//    }
//
//    lpPlacement->dwSize = sizeof(*lpPlacement);
//
//    return true;
//}
//
//
//bool wxAppBar::SetAppBarPlacement(LPCAPPBARPLACEMENT lpPlacement)
//{
//    wxASSERT(lpPlacement);
//
//    if (lpPlacement->dwSize >= sizeof(APPBARPLACEMENT1)) {
//        if (lpPlacement->dwState != wxSTATE_FLOAT && !IsDocked(lpPlacement->dwState)) {
//            // The state is unknown. Abort, before we mix up internal state.
//            return false;
//        }
//
//        // Clean previous docking/auto-hide settings if required.
//        if (IsDocked(m_state) &&
//            (lpPlacement->dwState == wxSTATE_FLOAT || lpPlacement->dwState != m_state && IsDocked(lpPlacement->dwState)))
//        {
//            if (IsAutoHide())
//                UnregisterAutoHide(m_state);
//            else if (lpPlacement->dwState == wxSTATE_FLOAT)
//                UndockAppBar();
//        }
//
//        // Update application bar's flags, size and position.
//        m_flags |= wxFLAG_POSITIONSET;
//        m_sizeDocked = lpPlacement->szDocked;
//        if (lpPlacement->dwState == wxSTATE_FLOAT) {
//            m_rect = lpPlacement->rcFloat;
//        } else if (IsDocked(lpPlacement->dwState)) {
//            m_sizeFloat.cx = lpPlacement->rcFloat.right  - lpPlacement->rcFloat.left;
//            m_sizeFloat.cy = lpPlacement->rcFloat.bottom - lpPlacement->rcFloat.top;
//            GetDockedRect(lpPlacement->dwState, &m_rect);
//        } else {
//            // Application bar's state is unknown.
//            wxFAIL;
//            return false;
//        }
//
//        // Notify about the change of state.
//        OnChangeState(lpPlacement->dwState);
//        m_state = lpPlacement->dwState;
//
//        SetAutoHide(lpPlacement->dwFlags & wxFLAG_AUTOHIDE);
//        SetAlwaysOnTop(lpPlacement->dwFlags & wxFLAG_ALWAYSONTOP);
//    }
//
//    return true;
//}


void wxAppBar::MinimiseToEdge(wxState state, wxWindow *wnd)
{
    wxASSERT(IsDocked(state));

    if (!wnd) {
        // No other window was specified. Minimize ourself.
        wnd = this;
    }

    WXHWND hWnd = wnd->GetHWND();

    // If our window is hidden, there's nothing we can do.
    if (hWnd == m_hWnd && !::IsWindowVisible(m_hWnd))
        return;

    if (m_state == wxSTATE_FLOAT) {
        // Remember the last floating size.
        m_sizeFloat.cx = m_rect.right  - m_rect.left;
        m_sizeFloat.cy = m_rect.bottom - m_rect.top;
    }

    RECT rectTo;
    if (IsAutoHide())
        GetAutoHideRect(state, false, &rectTo);
    else
        GetDockedRect(state, &rectTo);

    if (::wxGetDoWndAnimation()) {
        RECT rectFrom;

        // Calculate source and destination rectangles.
        if (hWnd != m_hWnd)
            ::GetWindowRect(hWnd, &rectFrom);
        else
            rectFrom = m_rect;

        // Do the animation.
        wxVERIFY(::DrawAnimatedRects(hWnd, IDANI_CAPTION, &rectFrom, &rectTo));
    }

    // Set the window rect.
    m_flags |= wxFLAG_POSITIONSET;
    m_rect = rectTo;

    // Notify about the change of state.
    OnChangeState(state);
    m_state = state;

    if (hWnd != m_hWnd) {
        // Hide the source window.
        wnd->Hide();
        // ::SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) &~ WS_VISIBLE);

        // Show the application bar.
        Show();
    } else {
        if (IsAutoHide()) {
            // Register auto-hide application bar.
            RegisterAutoHide(state);
        } else {
            // Auto-hide failed or wasn't desired at all.
            DockAppBar(state);
        }
    }

    ::SetActiveWindow(m_hWnd);
}


void wxAppBar::MaximiseFromEdge(const RECT* rect)
{
    wxASSERT(::IsWindowVisible(m_hWnd));

    RECT rc;

    if (!rect) {
        // Calculate the destination rect.
        rc.left   = (::GetSystemMetrics(SM_CXSCREEN) - m_sizeFloat.cx) / 2;
        rc.top    = (::GetSystemMetrics(SM_CYSCREEN) - m_sizeFloat.cy) / 2;
        rc.right  = rc.left + m_sizeFloat.cx;
        rc.bottom = rc.top  + m_sizeFloat.cy;

        rect = &rc;
    } else {
        m_sizeFloat.cx = rect->right  - rect->left;
        m_sizeFloat.cy = rect->bottom - rect->top;
    }

    if (::wxGetDoWndAnimation()) {
        // Do the animation.
        wxVERIFY(::DrawAnimatedRects(m_hWnd, IDANI_CAPTION, &m_rect, rect));
    }

    // Clean previous docking/auto-hide settings if required.
    if (IsDocked(m_state)) {
        if (IsAutoHide())
            UnregisterAutoHide(m_state);
        else
            UndockAppBar();
    }

    // Set the window rect.
    m_flags |= wxFLAG_POSITIONSET;
    m_rect = *rect;

    // Notify about the change of state.
    OnChangeState(wxSTATE_FLOAT);
    m_state = wxSTATE_FLOAT;

    ::SetWindowPos(m_hWnd, GetZWnd(wxSTATE_FLOAT, m_flags), 0, 0, 0, 0, SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_FRAMECHANGED);
}


void wxAppBar::MaximiseFromEdge(wxWindow *wnd)
{
    wxASSERT(wnd);
    wxASSERT(::IsWindowVisible(m_hWnd));

    WXHWND hWnd = wnd->GetHWND();
    RECT rectTo;

    ::GetWindowRect(hWnd, &rectTo);
    m_sizeFloat.cx = rectTo.right  - rectTo.left;
    m_sizeFloat.cy = rectTo.bottom - rectTo.top;

    if (::wxGetDoWndAnimation()) {
        // Do the animation.
        wxVERIFY(::DrawAnimatedRects(hWnd, IDANI_CAPTION, &m_rect, &rectTo));
    }

    Hide();

    // Show the destination window.
    wnd->Show();
    // ::SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | WS_VISIBLE);
    wxVERIFY(::SetForegroundWindow(hWnd));
    ::SetActiveWindow(hWnd);
    // wxVERIFY(::RedrawWindow(hWnd, NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_FRAME | RDW_INVALIDATE | RDW_ERASE));
}


void wxAppBar::ShowAutoHideAppBar(bool bShow)
{
    RECT rcEnd;
    bool
        bFullDragOn,
        bHidden = GetAutoHidden(),
        bVisible = ::IsWindowVisible(m_hWnd) ? true : false;

    if (!IsAutoHide() || bHidden != bShow) {
        // We're not setup for auto-hide or already in the desired state.
        return;
    }

    // Get our end window location.
    GetAutoHideRect(m_state, !bHidden, &rcEnd);

    if (bVisible) {
        RECT rcStart;

        // Only slide the window if the user has FullDrag turned on.
        if (!::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &bFullDragOn, 0))
            bFullDragOn = false;

        // Get our start window location.
        GetAutoHideRect(m_state, bHidden, &rcStart);

        if (bFullDragOn && (rcStart.left != rcEnd.left || rcStart.top != rcEnd.top || rcStart.right != rcEnd.right || rcStart.bottom != rcEnd.bottom)) {
            // Get our starting and ending time.
            DWORD dwTimeStart = ::GetTickCount();
            DWORD dwTimeElapsed;
            RECT rcDelta;

            // Calculate difference between ending and starting rectangle.
            rcDelta.left   = rcEnd.left   - rcStart.left;
            rcDelta.top    = rcEnd.top    - rcStart.top;
            rcDelta.right  = rcEnd.right  - rcStart.right;
            rcDelta.bottom = rcEnd.bottom - rcStart.bottom;

            m_flags |= wxFLAG_POSITIONSET;

            while ((dwTimeElapsed = ::GetTickCount() - dwTimeStart) < wxABT_AUTOHIDETIME) {
                // Do the linear interpolation.
                m_rect.left   = rcStart.left   + ::MulDiv(rcDelta.left,   dwTimeElapsed, wxABT_AUTOHIDETIME);
                m_rect.top    = rcStart.top    + ::MulDiv(rcDelta.top,    dwTimeElapsed, wxABT_AUTOHIDETIME);
                m_rect.right  = rcStart.right  + ::MulDiv(rcDelta.right,  dwTimeElapsed, wxABT_AUTOHIDETIME);
                m_rect.bottom = rcStart.bottom + ::MulDiv(rcDelta.bottom, dwTimeElapsed, wxABT_AUTOHIDETIME);

                // Show the window at its changed position
                ::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
                ::UpdateWindow(m_hWnd);
            }
        }
    }

    // Make sure that the window is at its final position
    m_flags |= wxFLAG_POSITIONSET;
    m_rect = rcEnd;

    if (bShow)
        m_flags &= ~wxFLAG_AUTOHIDDEN;
    else
        m_flags |= wxFLAG_AUTOHIDDEN;

    if (bVisible) {
        ::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
        ::UpdateWindow(m_hWnd);
        // Reset auto-hide timer.
        m_timerID = ::SetTimer(m_hWnd, wxABT_AUTOHIDETIMERID, wxABT_AUTOHIDETIMERINTERVAL, NULL);
    }
}


void wxAppBar::OnChangeState(wxState stateNew)
{
    if (stateNew == wxSTATE_FLOAT) {
        if (m_flags & wxFLAG_HIDETASKBARTABWHENFLOATING) {
            // Hide our application bar's entry on the Windows's taskbar.
            wxModifyStyleEx(m_hWnd, 0, WS_EX_TOOLWINDOW);
            if (m_taskbarList)
                wxVERIFY(SUCCEEDED(m_taskbarList->DeleteTab(m_hWnd)));
        } else if (m_flags & wxFLAG_HIDETASKBARTABWHENDOCKED) {
            // Show our application bar's entry on the Windows's taskbar.
            wxModifyStyleEx(m_hWnd, WS_EX_TOOLWINDOW, 0);
            if (m_taskbarList)
                wxVERIFY(SUCCEEDED(m_taskbarList->AddTab(m_hWnd)));
        }
    } else if (IsDocked(stateNew)) {
        if (m_flags & wxFLAG_HIDETASKBARTABWHENDOCKED) {
            // Hide our application bar's entry on the Windows's taskbar.
            wxModifyStyleEx(m_hWnd, 0, WS_EX_TOOLWINDOW);
            if (m_taskbarList)
                wxVERIFY(SUCCEEDED(m_taskbarList->DeleteTab(m_hWnd)));
        } else if (m_flags & wxFLAG_HIDETASKBARTABWHENFLOATING) {
            // Show our application bar's entry on the Windows's taskbar.
            wxModifyStyleEx(m_hWnd, WS_EX_TOOLWINDOW, 0);
            if (m_taskbarList)
                wxVERIFY(SUCCEEDED(m_taskbarList->AddTab(m_hWnd)));
        }
    } else {
        // Unknown state.
        wxFAIL;
    }
}


void wxAppBar::OnChangeTaskBarState(UINT_PTR state)
{
    if (state != m_stateTaskBar) {
        // No change, no fun.
        return;
    }

    if ((m_flags & wxFLAG_ALWAYSONTOPTASKBAR) &&
        (m_stateTaskBar & ABS_ALWAYSONTOP) != (state & ABS_ALWAYSONTOP)) {
        // Always-on-top state of the taskbar changed and we're supposed to mimic it.
        SetAlwaysOnTop((state & ABS_ALWAYSONTOP) ? true : false);
    }

    if ((m_flags & wxFLAG_AUTOHIDETASKBAR) &&
        (m_stateTaskBar & ABS_AUTOHIDE) != (state & ABS_AUTOHIDE)) {
        // Auto-hide state of the taskbar changed and we're supposed to mimic it.
        SetAutoHide((state & ABS_AUTOHIDE) ? true : false);
    }

    // Remember the state.
    m_stateTaskBar = state;
}


void wxAppBar::OnWindowsArrange(bool WXUNUSED(bBeginning))
{
}


void wxAppBar::OnAutoHideDenied()
{
    wxMessageBox(wxT("There is already an auto hidden bar on this edge.\nOnly one auto hidden bar is allowed on each edge.\nAuto-hide feature is now off."), wxT("Warning"), wxOK | wxICON_WARNING, this);
}


WXLRESULT wxAppBar::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    switch (message) {
    case WM_SHOWWINDOW:
        if (wParam) {
            // Do the false change of state notification, to allow initialization.
            OnChangeState(m_state);

            if (IsDocked(m_state)) {
                if (IsAutoHide()) {
                    // Register auto-hide application bar.
                    RegisterAutoHide(m_state);
                } else {
                    // Auto-hide failed or wasn't desired at all.
                    DockAppBar(m_state);
                }
            }

            // We're all set. Show the window now.
            return wxFrame::MSWWindowProc(message, wParam, lParam);
        } else {
            // Hide the window first, to avoid flicker.
            WXLRESULT lResult = wxFrame::MSWWindowProc(message, wParam, lParam);

            // Clean previous docking/auto-hide settings if required.
            if (IsDocked(m_state)) {
                if (IsAutoHide())
                    UnregisterAutoHide(m_state);
                else
                    UndockAppBar();
            }

            return lResult;
        }

    case WM_WINDOWPOSCHANGING: {
        wxASSERT(lParam);
        LPWINDOWPOS lpwndpos = (LPWINDOWPOS)lParam;

        // Sync to our window rect.
        if (!(m_flags & wxFLAG_POSITIONSET) && !(lpwndpos->flags & SWP_NOMOVE)) {
            // Get the window position first time the SetWindowPos() is called.
            m_rect.right  = lpwndpos->x + m_rect.right  - m_rect.left;
            m_rect.bottom = lpwndpos->y + m_rect.bottom - m_rect.top;
            m_rect.left   = lpwndpos->x;
            m_rect.top    = lpwndpos->y;
            m_flags |= wxFLAG_POSITIONSET;
        } else {
            lpwndpos->x = m_rect.left;
            lpwndpos->y = m_rect.top;
        }
        lpwndpos->cx = m_rect.right - m_rect.left;
        lpwndpos->cy = m_rect.bottom - m_rect.top;
        lpwndpos->flags &= ~(SWP_NOMOVE | SWP_NOSIZE);
        return wxFrame::MSWWindowProc(message, wParam, lParam);
    }

    case WM_WINDOWPOSCHANGED: {
        WXLRESULT lResult = wxFrame::MSWWindowProc(message, wParam, lParam);

        if (IsDocked(m_state)) {
            APPBARDATA abd = {
                sizeof(abd),
                m_hWnd
            };

            // When our window changes position, tell the Shell so that any
            // auto-hidden application bar on our edge stays on top of our window making it
            // always accessible to the user.
            wxVERIFY(::SHAppBarMessage(ABM_WINDOWPOSCHANGED, &abd));
        }

        return lResult;
    }

    case WM_ENTERSIZEMOVE:
        m_stateDesired = m_state;

        return 0;

    case WM_EXITSIZEMOVE:
        // Clean previous docking/auto-hide settings if required.
        if (IsDocked(m_state) &&
            (m_stateDesired == wxSTATE_FLOAT || m_stateDesired != m_state && IsDocked(m_stateDesired)))
        {
            if (IsAutoHide())
                UnregisterAutoHide(m_state);
            else if (m_stateDesired == wxSTATE_FLOAT)
                UndockAppBar();
        }

        // Setup new docking/auto-hide settings.
        if (IsDocked(m_stateDesired)) {
            if (IsAutoHide()) {
                // Application bar should be auto-hide. Try to setup it as so.
                // Register auto-hide application bar.
                RegisterAutoHide(m_stateDesired);
            } else {
                // Auto-hide failed or wasn't desired at all.
                DockAppBar(m_stateDesired);
            }
        }

        // Notify about the change of state.
        OnChangeState(m_stateDesired);
        m_state = m_stateDesired;
        m_stateDesired = wxSTATE_UNKNOWN;

        return 0;

    case WM_MOVING: {
        WXLRESULT lResult = wxFrame::MSWWindowProc(message, wParam, lParam);

        wxASSERT(lParam);
        LPRECT lpRect = (LPRECT)lParam;

        // Phase 1. - Determine the desired state (which screen edge or floating) according to the mouse position.

        wxState uStateDesiredPrev = m_stateDesired;
        DWORD dwPoint = ::GetMessagePos();
        wxPoint ptMouse(GET_X_LPARAM(dwPoint), GET_Y_LPARAM(dwPoint));

        m_stateDesired = wxSTATE_UNKNOWN;

        if ((m_flags & wxFLAG_ALLOWFLOAT) && (::GetKeyState(VK_CONTROL) < 0 || !(m_flags & wxFLAG_ALLOWDOCKANY))) {
            // The application bar's style allows floating and user is holding down the Ctrl key, or docking is disabled anyway, so we'll force float.
            m_stateDesired = wxSTATE_FLOAT;
        } else {
            RECT rcWorkArea = {};
            INT iDist, iDistMin = INT_MAX;

            // Get the rectangle that bounds the size of the screen
            // minus any docked (but not-autohidden) AppBars.
            wxVERIFY(::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0));

            // Test all four edges, to find the closest one.
            if ((m_flags & wxFLAG_ALLOWDOCKTOP) && (iDist = ptMouse.y - rcWorkArea.top) < iDistMin) {
                m_stateDesired = wxSTATE_TOP;
                iDistMin = iDist;
            }
            if ((m_flags & wxFLAG_ALLOWDOCKBOTTOM) && (iDist = rcWorkArea.bottom - ptMouse.y) < iDistMin) {
                m_stateDesired = wxSTATE_BOTTOM;
                iDistMin = iDist;
            }
            if ((m_flags & wxFLAG_ALLOWDOCKLEFT) && (iDist = ptMouse.x - rcWorkArea.left) < iDistMin) {
                m_stateDesired = wxSTATE_LEFT;
                iDistMin = iDist;
            }
            if ((m_flags & wxFLAG_ALLOWDOCKRIGHT) && (iDist = rcWorkArea.right - ptMouse.x) < iDistMin) {
                m_stateDesired = wxSTATE_RIGHT;
                iDistMin = iDist;
            }
            if ((m_flags & wxFLAG_ALLOWFLOAT) && (iDist = wxMax(::GetSystemMetrics(SM_CXVSCROLL), ::GetSystemMetrics(SM_CYHSCROLL))) < iDistMin) {
                m_stateDesired = wxSTATE_FLOAT;
                iDistMin = iDist;
            }
        }

        // We must had come up with some useful state.
        wxASSERT(m_stateDesired != wxSTATE_UNKNOWN);

        // Phase 2. - Calculate the desired rectangle, according to the desired state.

        if (m_stateDesired == wxSTATE_FLOAT) {
            if (IsDocked(uStateDesiredPrev)) {
                // We just switched from docked to floating state. Align the window according to the mouse pointer.
                lpRect->left   = ptMouse.x - m_sizeFloat.cx / 2;
                lpRect->top    = ptMouse.y;

                // We also resize back to floating window size.
                lpRect->right  = lpRect->left + m_sizeFloat.cx;
                lpRect->bottom = lpRect->top + m_sizeFloat.cy;
            }
        } else if (IsDocked(m_stateDesired)) {
            if (IsAutoHide()) {
                // Calculate auto-hide window rect.
                GetAutoHideRect(m_stateDesired, false, lpRect);
            } else {
                // Calculate docked window rect.
                GetDockedRect(m_stateDesired, lpRect);
            }
        } else {
            // Window is not floating. It's not docked either. Then what?
            wxFAIL;
        }

        // Set the window rect.
        m_flags |= wxFLAG_POSITIONSET;
        m_rect = *lpRect;

        if (m_stateDesired != uStateDesiredPrev) {
            // Notify about the proposed change of state, but don't change the state yet.
            OnChangeState(m_stateDesired);
        }

        return lResult;
    }

    case WM_SIZING: {
        wxASSERT(lParam);

        LPRECT lpRect = (LPRECT)lParam;

        m_flags |= wxFLAG_POSITIONSET;
        m_rect = *lpRect;

        if (m_stateDesired == wxSTATE_FLOAT) {
            // Remember the floating window size.
            m_sizeFloat.cx = lpRect->right  - lpRect->left;
            m_sizeFloat.cy = lpRect->bottom - lpRect->top;
        } else if (IsDocked(m_stateDesired)) {
            // Remember the docked window size.
            if (IsDockedVert(m_stateDesired))
                m_sizeDocked.cx = lpRect->right - lpRect->left;
            else
                m_sizeDocked.cy = lpRect->bottom - lpRect->top;
        } else {
            // Unknown state.
            wxFAIL;
        }

        return wxFrame::MSWWindowProc(message, wParam, lParam);
    }

    case WM_GETMINMAXINFO: {
        WXLRESULT lResult = wxFrame::MSWWindowProc(message, wParam, lParam);

        wxASSERT(lParam);
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;

        // Convert client size to window size.
        RECT rc = {};
        wxVERIFY(::GetClientRect(m_hWnd, &rc));
        if (rc.right - rc.left && rc.bottom - rc.top) {
            lpMMI->ptMinTrackSize.x = m_sizeMin.cx + (m_rect.right  - m_rect.left) - (rc.right  - rc.left);
            lpMMI->ptMinTrackSize.y = m_sizeMin.cy + (m_rect.bottom - m_rect.top ) - (rc.bottom - rc.top );
        }

        return lResult;
    }

    case WM_NCMOUSEMOVE:
        // If we are a docked, auto-hidden application bar, shown us
        // when the user moves over our non-client area
        ShowAutoHideAppBar(true);

        return wxFrame::MSWWindowProc(message, wParam, lParam);

    case WM_NCHITTEST: {
        // Find out what the base class thinks is the hit test code.
        WXLRESULT lResult = wxFrame::MSWWindowProc(message, wParam, lParam);

        if (HTSIZEFIRST <= lResult && lResult <= HTSIZELAST) {
            if (GetAllowSizing()) {
                if (IsDocked(m_state)) {
                    // When the application bar is docked, the user can resize only one edge.
                    // This next section determines which edge the user can resize.
                    // To allow resizing, the application bar window must have the WS_THICKFRAME style.

                    RECT rcClient = {};
                    wxPoint point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

                    // Resizing IS allowed for the edge that the application bar is docked on.
                    // Get the location of the appbar's client area in screen coordinates.
                    wxVERIFY(::GetClientRect(m_hWnd, &rcClient));
                    wxVERIFY(::ClientToScreen(m_hWnd, (LPPOINT)&rcClient));
                    wxVERIFY(::ClientToScreen(m_hWnd, ((LPPOINT)&rcClient) + 1));

                    // Assume that we can't resize
                    lResult = HTBORDER;

                    switch (m_state) {
                        case wxSTATE_LEFT:   if (point.x > rcClient.right)  lResult = HTRIGHT;  break;
                        case wxSTATE_TOP:    if (point.y > rcClient.bottom) lResult = HTBOTTOM; break;
                        case wxSTATE_RIGHT:  if (point.x < rcClient.left)   lResult = HTLEFT;   break;
                        case wxSTATE_BOTTOM: if (point.y < rcClient.top)    lResult = HTTOP;    break;
                    }
                }
            } else {
                // The sizing is locked. Pretend that the mouse is not on a resize border.
                lResult = HTBORDER;
            }
        }

        return lResult;
    }

    case WM_ACTIVATE: {
        WXLRESULT lResult = wxFrame::MSWWindowProc(message, wParam, lParam);

        if (IsDocked(m_state)) {
            APPBARDATA abd = {
                sizeof(abd),
                m_hWnd
            };

            // When our window changes activation state, tell the Shell so that any
            // auto-hidden application bar on our edge stays on top of our window making it
            // always accessible to the user.
            wxVERIFY(::SHAppBarMessage(ABM_ACTIVATE, &abd));

            if (LOWORD(wParam) == WA_INACTIVE && IsAutoHide()) {
                // Hide the application bar if we are docked and auto-hidden.
                ShowAutoHideAppBar(false);
            }
        }

        return lResult;
    }

    case WM_TIMER:
        if (wParam == wxABT_AUTOHIDETIMERID) {
            if (!GetAutoHidden() &&
                IsDocked(m_state) &&
                GetActiveWindow() != m_hWnd)
            {
                // Get the position of the mouse and the application bar's position
                // Everything must be in screen coordinates.
                DWORD dwPoint = ::GetMessagePos();
                wxPoint pt(GET_X_LPARAM(dwPoint), GET_Y_LPARAM(dwPoint));
                wxRect rc(m_rect.left, m_rect.top, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top);

                // Add a little margin around the application bar
                rc.Inflate(2 * ::GetSystemMetrics(SM_CXDOUBLECLK), 2 * ::GetSystemMetrics(SM_CYDOUBLECLK));
                if (!rc.Contains(pt)) {
                    // If the mouse is NOT over or near the application bar, hide it.
                    ShowAutoHideAppBar(false);
                }
            }

            return 0;
        } else
            return wxFrame::MSWWindowProc(message, wParam, lParam);

    default:
        if (message == WM_AB_NOTIFY) {
            switch (wParam) {
                case ABN_FULLSCREENAPP:
                    if (lParam)
                        m_flags |= wxFLAG_FULLSCREENAPPOPEN;
                    else
                        m_flags &= ~wxFLAG_FULLSCREENAPPOPEN;
                    OnChangeTaskBarState(GetTaskBarState());

                    if (::IsWindowVisible(m_hWnd)) {
                        // Set the Z-order. SWP_NOSENDCHANGING flag prevents our OnWindowPosChanging() method to be called, since moving is not desired.
                        wxVERIFY(::SetWindowPos(m_hWnd, GetZWnd(m_state, m_flags), 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOSENDCHANGING));
                    }
                    break;

                case ABN_POSCHANGED:
                    if (IsDocked(m_state) && !IsAutoHide())
                        DockAppBar(m_state);
                    break;

                case ABN_STATECHANGE:
                    // Taskbar's state changed.
                    OnChangeTaskBarState(GetTaskBarState());
                    break;

                case ABN_WINDOWARRANGE:
                    OnWindowsArrange(lParam ? true : false);
                    break;

                default:
                    // Unknown message.
                    wxFAIL;
                    break;
            }

            return 0;
        } else
            return wxFrame::MSWWindowProc(message, wParam, lParam);
    }
}
