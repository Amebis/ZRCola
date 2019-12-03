/*
    Copyright 2015-2019 Amebis

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


///
/// Main function
///
extern "C" void WinMainCRTStartup()
{
    // Load "KERNEL32.DLL".
    HMODULE hKernel32 = LoadLibrary(TEXT("KERNEL32.DLL"));
    if (!hKernel32)
        ExitProcess(1);

    // Get IsWow64Process() address.
    BOOL (WINAPI *_IsWow64Process)(__in HANDLE hProcess, __out PBOOL Wow64Process) = (BOOL(WINAPI*)(__in HANDLE, __out PBOOL))::GetProcAddress(hKernel32, "IsWow64Process");

    BOOL bIs64Bit;
#ifndef _WIN64
    // Determine if this is a 32-bit process under Windows-over-Windows64.
    if (_IsWow64Process) {
        // See what IsWow64Process() says.
        if (!_IsWow64Process(::GetCurrentProcess(), &bIs64Bit)) {
            // IsWow64Process() returned an error. Assume not 64-bit Windows.
            bIs64Bit = FALSE;
        }
    } else {
        // This platform does not have IsWow64Process(). Therefore, this is definitely not 64-bit Windows.
        bIs64Bit = FALSE;
    }
#else
    // This is a running 64-bit process. The Windows must be 64 bit then.
    bIs64Bit = TRUE;
#endif

    FreeLibrary(hKernel32);

    // Get temporary folder path.
    static const LPTSTR pszTempFolderDefault = TEXT("");
    LPTSTR pszTempFolder;
    DWORD dwLength;
    if ((dwLength = GetEnvironmentVariable(TEXT("TEMP"), NULL, 0)) != 0 &&
        (pszTempFolder = (LPTSTR)LocalAlloc(LMEM_FIXED, (dwLength + 1)*sizeof(TCHAR))) != NULL) // +1 is for trailing backslash when missing!
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
        (DWORD_PTR)(bIs64Bit ? TEXT("64") : TEXT("32")),
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
