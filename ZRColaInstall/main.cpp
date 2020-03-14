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

#include "pch.h"


///
/// Main function
///
int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    TCHAR szArchProc[MAX_PATH];
    if (!GetEnvironmentVariable(TEXT("PROCESSOR_ARCHITECTURE"), szArchProc, _countof(szArchProc)))
        szArchProc[0] = 0;
    szArchProc[_countof(szArchProc) - 1] = 0;

    TCHAR szArchWin[MAX_PATH];
    if (!GetEnvironmentVariable(TEXT("PROCESSOR_ARCHITEW6432"), szArchWin, _countof(szArchWin)))
        szArchWin[0] = 0;
    szArchWin[_countof(szArchWin) - 1] = 0;

    // If PROCESSOR_ARCHITEW6432 is defined, this is a Windows-on-Windows
    // guest of some kind. szArchWin is the host's architecture and szArchProc
    // is the current process' architecture.

    // If PROCESSOR_ARCHITEW6432 is not defined, then PROCESSOR_ARCHITECTURE
    // is both the process and OS architecture.
    if (szArchWin[0] == 0)
        memcpy_s(szArchWin, sizeof(szArchWin), szArchProc, sizeof(szArchProc));

    // If PROCESSOR_ARCHITECTURE is not defined, then either we're running on
    // Windows 9x or something is messing with the environment. Make the
    // Windows 9x assumption.
    if (szArchProc[0] == 0) {
        memcpy_s(szArchProc, sizeof(szArchProc), TEXT("x86"), sizeof(TEXT("x86")));
        memcpy_s(szArchWin , sizeof(szArchWin ), TEXT("x86"), sizeof(TEXT("x86")));
    }

    LPCTSTR pszPlatSuffix;
    if (_tcsicmp(szArchWin, TEXT("amd64")) == 0)
        pszPlatSuffix = TEXT("-amd64");
    else if (_tcsicmp(szArchWin, TEXT("arm64")) == 0)
        pszPlatSuffix = TEXT("-arm64");
    else
        pszPlatSuffix = TEXT("-x86");

    // Get temporary folder path.
    static const LPTSTR pszTempFolderDefault = TEXT("");
    LPTSTR pszTempFolder;
    DWORD dwLength;
    if ((dwLength = GetEnvironmentVariable(TEXT("TEMP"), NULL, 0)) != 0 &&
        (pszTempFolder = (LPTSTR)LocalAlloc(LMEM_FIXED, (dwLength + 2)*sizeof(TCHAR))) != NULL) // +1 for trailing backslash when missing, +1 for zero terminator
    {
        dwLength = ::GetEnvironmentVariable(TEXT("TEMP"), pszTempFolder, dwLength);

        if (pszTempFolder[dwLength - 1] != TEXT('\\')) {
            // Append trailing backslash.
            pszTempFolder[dwLength    ] = TEXT('\\');
            pszTempFolder[dwLength + 1] = 0;
        }
    } else
        pszTempFolder = pszTempFolderDefault;

    // Format msiexec's command line.
    LPTSTR pszParams;
    DWORD_PTR aArgs[] = {
        (DWORD_PTR)TEXT(ZRCOLA_INSTALL_LANG),
        (DWORD_PTR)pszPlatSuffix,
        (DWORD_PTR)pszTempFolder,
    };
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_FROM_STRING,
        TEXT("/i \"https://prenos.amebis.si/ZRCola/_latest/ZRCola%1%2.msi\" /l* \"%3ZRCola%1%2.log\""),
        0,
        0,
        (LPTSTR)&pszParams,
        1,
        (va_list*)aArgs);

    // Launch installation.
    int iResult = (int)ShellExecute(NULL, NULL, TEXT("msiexec.exe"), pszParams, NULL, SW_SHOWNORMAL) > 32;

    // Clean up.
    LocalFree(pszParams);
    if (pszTempFolder != pszTempFolderDefault) LocalFree(pszTempFolder);

    ExitProcess(iResult ? 0 : 2);
}
