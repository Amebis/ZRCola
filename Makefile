#
#    Copyright 1991-2016 Amebis
#
#    This file is part of ZRCola.
#
#    ZRCola is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    ZRCola is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with ZRCola. If not, see <http://www.gnu.org/licenses/>.
#

OUTPUT_DIR=output

!IF "$(PROCESSOR_ARCHITECTURE)" == "AMD64"
PLAT=x64
REG_FLAGS=/f /reg:64
REG_FLAGS32=/f /reg:32
!ELSE
PLAT=Win32
REG_FLAGS=/f
!ENDIF


All ::

Clean ::
	cd "MSI\MSIBuild\Version"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean
	cd "$(MAKEDIR)"
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean LANG=En PLAT=Win32 CFG=Release
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean LANG=En PLAT=Win32 CFG=Debug
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean LANG=En PLAT=x64   CFG=Release
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean LANG=En PLAT=x64   CFG=Debug
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean LANG=Sl PLAT=Win32 CFG=Release
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean LANG=Sl PLAT=Win32 CFG=Debug
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean LANG=Sl PLAT=x64   CFG=Release
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean LANG=Sl PLAT=x64   CFG=Debug
	cd "$(MAKEDIR)"
	devenv.com "ZRCola.sln"          /clean "Release|Win32"
	devenv.com "ZRCola.sln"          /clean "Debug|Win32"
	devenv.com "ZRCola.sln"          /clean "Release|x64"
	devenv.com "ZRCola.sln"          /clean "Debug|x64"
	devenv.com "ZRColaUtils.sln"     /clean "Release|Win32"
	devenv.com "ZRColaUtils.sln"     /clean "Debug|Win32"
	devenv.com "MSI\MSICA\MSICA.sln" /clean "Release|Win32"
	devenv.com "MSI\MSICA\MSICA.sln" /clean "Debug|Win32"
	devenv.com "MSI\MSICA\MSICA.sln" /clean "Release|x64"
	devenv.com "MSI\MSICA\MSICA.sln" /clean "Debug|x64"
	devenv.com "Updater\Updater.sln" /clean "Release|Win32"
	devenv.com "Updater\Updater.sln" /clean "Debug|Win32"
	devenv.com "Updater\Updater.sln" /clean "Release|x64"
	devenv.com "Updater\Updater.sln" /clean "Debug|x64"
	-if exist "$(OUTPUT_DIR)\Setup\ZRColaEn32.msi"  del /f /q "$(OUTPUT_DIR)\Setup\ZRColaEn32.msi"
	-if exist "$(OUTPUT_DIR)\Setup\ZRColaEn32D.msi" del /f /q "$(OUTPUT_DIR)\Setup\ZRColaEn32D.msi"
	-if exist "$(OUTPUT_DIR)\Setup\ZRColaEn64.msi"  del /f /q "$(OUTPUT_DIR)\Setup\ZRColaEn64.msi"
	-if exist "$(OUTPUT_DIR)\Setup\ZRColaEn64D.msi" del /f /q "$(OUTPUT_DIR)\Setup\ZRColaEn64D.msi"
	-if exist "$(OUTPUT_DIR)\Setup\ZRColaSl32.msi"  del /f /q "$(OUTPUT_DIR)\Setup\ZRColaSl32.msi"
	-if exist "$(OUTPUT_DIR)\Setup\ZRColaSl32D.msi" del /f /q "$(OUTPUT_DIR)\Setup\ZRColaSl32D.msi"
	-if exist "$(OUTPUT_DIR)\Setup\ZRColaSl64.msi"  del /f /q "$(OUTPUT_DIR)\Setup\ZRColaSl64.msi"
	-if exist "$(OUTPUT_DIR)\Setup\ZRColaSl64D.msi" del /f /q "$(OUTPUT_DIR)\Setup\ZRColaSl64D.msi"

!IFNDEF HAS_VERSION

######################################################################
# 1st Phase
# - Version info parsing
######################################################################

All \
Setup \
SetupDebug \
Register \
Unregister :: "MSI\MSIBuild\Version\Version.mak"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) HAS_VERSION=1 $@

"MSI\MSIBuild\Version\Version.mak" ::
	cd "MSI\MSIBuild\Version"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Version
	cd "$(MAKEDIR)"

!ELSE

######################################################################
# 2nd Phase
# - The version is known, do the rest.
######################################################################

!INCLUDE "MSI\MSIBuild\Version\Version.mak"
!INCLUDE "include\MSIBuildCfg.mak"


######################################################################
# Main targets
######################################################################

All :: \
	Setup

Setup :: \
	"$(OUTPUT_DIR)\Setup" \
	"$(OUTPUT_DIR)\Setup\ZRColaEn32.msi" \
	"$(OUTPUT_DIR)\Setup\ZRColaEn64.msi" \
	"$(OUTPUT_DIR)\Setup\ZRColaSl32.msi" \
	"$(OUTPUT_DIR)\Setup\ZRColaSl64.msi"

SetupDebug :: \
	"$(OUTPUT_DIR)\Setup" \
	"$(OUTPUT_DIR)\Setup\ZRColaEn32D.msi" \
	"$(OUTPUT_DIR)\Setup\ZRColaEn64D.msi" \
	"$(OUTPUT_DIR)\Setup\ZRColaSl32D.msi" \
	"$(OUTPUT_DIR)\Setup\ZRColaSl64D.msi"

Register :: \
	RegisterSettings \
	InstallFonts \
	RegisterShortcuts

Unregister :: \
	UnregisterShortcuts \
	UninstallFonts \
	UnregisterSettings

RegisterSettings ::
	reg.exe add "HKLM\Software\Amebis\ZRCola" /v "LocalizationRepositoryPath" /t REG_SZ /d "$(MAKEDIR)\$(OUTPUT_DIR)\locale" $(REG_FLAGS) > NUL
	reg.exe add "HKLM\Software\Amebis\ZRCola" /v "DatabasePath"               /t REG_SZ /d "$(MAKEDIR)\$(OUTPUT_DIR)\data"   $(REG_FLAGS) > NUL
!IF "$(PROCESSOR_ARCHITECTURE)" == "AMD64"
	reg.exe add "HKLM\Software\Amebis\ZRCola" /v "LocalizationRepositoryPath" /t REG_SZ /d "$(MAKEDIR)\$(OUTPUT_DIR)\locale" $(REG_FLAGS32) > NUL
	reg.exe add "HKLM\Software\Amebis\ZRCola" /v "DatabasePath"               /t REG_SZ /d "$(MAKEDIR)\$(OUTPUT_DIR)\data"   $(REG_FLAGS32) > NUL
!ENDIF

UnregisterSettings ::
	-reg.exe delete "HKLM\Software\Amebis\ZRCola" /v "LocalizationRepositoryPath" $(REG_FLAGS) > NUL
	-reg.exe delete "HKLM\Software\Amebis\ZRCola" /v "DatabasePath"               $(REG_FLAGS) > NUL
!IF "$(PROCESSOR_ARCHITECTURE)" == "AMD64"
	-reg.exe delete "HKLM\Software\Amebis\ZRCola" /v "LocalizationRepositoryPath" $(REG_FLAGS32) > NUL
	-reg.exe delete "HKLM\Software\Amebis\ZRCola" /v "DatabasePath"               $(REG_FLAGS32) > NUL
!ENDIF

InstallFonts :: \
	"$(WINDIR)\Fonts\00_ZRCola_Re.ttf" \
	"$(WINDIR)\Fonts\00_ZRCola_It.ttf" \
	"$(WINDIR)\Fonts\00_ZRCola_Bd.ttf" \
	"$(WINDIR)\Fonts\00_ZRCola_BI.ttf"
	reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts" /v "00 ZRCola (TrueType)"             /t REG_SZ /d "00_ZRCola_Re.ttf" $(REG_FLAGS) > NUL
	reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts" /v "00 ZRCola Italic (TrueType)"      /t REG_SZ /d "00_ZRCola_It.ttf" $(REG_FLAGS) > NUL
	reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts" /v "00 ZRCola Bold (TrueType)"        /t REG_SZ /d "00_ZRCola_Bd.ttf" $(REG_FLAGS) > NUL
	reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts" /v "00 ZRCola Bold Italic (TrueType)" /t REG_SZ /d "00_ZRCola_BI.ttf" $(REG_FLAGS) > NUL

UninstallFonts::
	-reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts" /v "00 ZRCola (TrueType)"             $(REG_FLAGS) > NUL
	-reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts" /v "00 ZRCola Italic (TrueType)"      $(REG_FLAGS) > NUL
	-reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts" /v "00 ZRCola Bold (TrueType)"        $(REG_FLAGS) > NUL
	-reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts" /v "00 ZRCola Bold Italic (TrueType)" $(REG_FLAGS) > NUL
	-if exist "$(WINDIR)\Fonts\00_ZRCola_Re.ttf" rd /s /q "$(WINDIR)\Fonts\00_ZRCola_Re.ttf"
	-if exist "$(WINDIR)\Fonts\00_ZRCola_It.ttf" rd /s /q "$(WINDIR)\Fonts\00_ZRCola_It.ttf"
	-if exist "$(WINDIR)\Fonts\00_ZRCola_Bd.ttf" rd /s /q "$(WINDIR)\Fonts\00_ZRCola_Bd.ttf"
	-if exist "$(WINDIR)\Fonts\00_ZRCola_BI.ttf" rd /s /q "$(WINDIR)\Fonts\00_ZRCola_BI.ttf"

RegisterShortcuts :: \
	"$(PROGRAMDATA)\Microsoft\Windows\Start Menu\Programs\ZRCola" \
	"$(PROGRAMDATA)\Microsoft\Windows\Start Menu\Programs\ZRCola\ZRCola.lnk"

UnregisterShortcuts ::
	-if exist "$(PROGRAMDATA)\Microsoft\Windows\Start Menu\Programs\ZRCola" rd /s /q "$(PROGRAMDATA)\Microsoft\Windows\Start Menu\Programs\ZRCola"


######################################################################
# Folder creation
######################################################################

"$(OUTPUT_DIR)" \
"$(OUTPUT_DIR)\Keys" \
"$(OUTPUT_DIR)\Setup" \
"$(PROGRAMDATA)\Microsoft\Windows\Start Menu\Programs\ZRCola" :
	if not exist $@ md $@

"$(OUTPUT_DIR)\Keys" \
"$(OUTPUT_DIR)\Setup" : "$(OUTPUT_DIR)"


######################################################################
# File copy
######################################################################

"$(OUTPUT_DIR)\Setup\ZRColaEn32.msi" \
$(REDIST_EN_WIN32) : "$(OUTPUT_DIR)\ZRColaEn32.3.msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\ZRColaEn32D.msi" : "$(OUTPUT_DIR)\ZRColaEn32D.3.msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\ZRColaEn64.msi" \
$(REDIST_EN_X64) : "$(OUTPUT_DIR)\ZRColaEn64.3.msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\ZRColaEn64D.msi" : "$(OUTPUT_DIR)\ZRColaEn64D.3.msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\ZRColaSl32.msi" \
$(REDIST_SL_WIN32) : "$(OUTPUT_DIR)\ZRColaSl32.3.msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\ZRColaSl32D.msi" : "$(OUTPUT_DIR)\ZRColaSl32D.3.msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\ZRColaSl64.msi" \
$(REDIST_SL_X64) : "$(OUTPUT_DIR)\ZRColaSl64.3.msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\ZRColaSl64D.msi" : "$(OUTPUT_DIR)\ZRColaSl64D.3.msi"
	copy /y $** $@ > NUL

"$(WINDIR)\Fonts\00_ZRCola_Re.ttf" : "$(OUTPUT_DIR)\00_ZRCola_Re.ttf"
	copy /y $** $@ > NUL

"$(WINDIR)\Fonts\00_ZRCola_It.ttf" : "$(OUTPUT_DIR)\00_ZRCola_It.ttf"
	copy /y $** $@ > NUL

"$(WINDIR)\Fonts\00_ZRCola_Bd.ttf" : "$(OUTPUT_DIR)\00_ZRCola_Bd.ttf"
	copy /y $** $@ > NUL

"$(WINDIR)\Fonts\00_ZRCola_BI.ttf" : "$(OUTPUT_DIR)\00_ZRCola_BI.ttf"
	copy /y $** $@ > NUL


######################################################################
# Shortcut creation
######################################################################

"$(PROGRAMDATA)\Microsoft\Windows\Start Menu\Programs\ZRCola\ZRCola.lnk" : "$(OUTPUT_DIR)\$(PLAT).Debug\ZRCola.exe"
	cscript.exe "bin\MkLnk.wsf" //Nologo $@ "$(MAKEDIR)\$(OUTPUT_DIR)\$(PLAT).Debug\ZRCola.exe"


######################################################################
# Building
######################################################################

"$(OUTPUT_DIR)\Win32.Release\libZRCola10u_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Release\libZRColaUI10u_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Release\stdex10u_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Release\wxExtend10u_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Release\ZRCola.exe" \
"$(OUTPUT_DIR)\ZRColaEn32.3.msi" \
"$(OUTPUT_DIR)\ZRColaSl32.3.msi" ::
	devenv.com "ZRCola.sln" /build "Release|Win32"

"$(OUTPUT_DIR)\Win32.Debug\libZRCola10ud_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Debug\libZRColaUI10ud_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Debug\stdex10ud_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Debug\wxExtend10ud_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Debug\ZRCola.exe" \
"$(OUTPUT_DIR)\ZRColaEn32D.3.msi" \
"$(OUTPUT_DIR)\ZRColaSl32D.3.msi" ::
	devenv.com "ZRCola.sln" /build "Debug|Win32"

"$(OUTPUT_DIR)\x64.Release\libZRCola10u_vc100.dll" \
"$(OUTPUT_DIR)\x64.Release\libZRColaUI10u_vc100.dll" \
"$(OUTPUT_DIR)\x64.Release\stdex10u_vc100.dll" \
"$(OUTPUT_DIR)\x64.Release\wxExtend10u_vc100.dll" \
"$(OUTPUT_DIR)\x64.Release\ZRCola.exe" \
"$(OUTPUT_DIR)\ZRColaEn64.3.msi" \
"$(OUTPUT_DIR)\ZRColaSl64.3.msi" ::
	devenv.com "ZRCola.sln" /build "Release|x64"

"$(OUTPUT_DIR)\x64.Debug\libZRCola10ud_vc100.dll" \
"$(OUTPUT_DIR)\x64.Debug\libZRColaUI10ud_vc100.dll" \
"$(OUTPUT_DIR)\x64.Debug\stdex10ud_vc100.dll" \
"$(OUTPUT_DIR)\x64.Debug\wxExtend10ud_vc100.dll" \
"$(OUTPUT_DIR)\x64.Debug\ZRCola.exe" \
"$(OUTPUT_DIR)\ZRColaEn64D.3.msi" \
"$(OUTPUT_DIR)\ZRColaSl64D.3.msi" ::
	devenv.com "ZRCola.sln" /build "Debug|x64"

"$(OUTPUT_DIR)\ZRColaEn32.3.msi" \
"$(OUTPUT_DIR)\ZRColaSl32.3.msi" ::
	devenv.com "MSI\MSICA\MSICA.sln" /build "Release|Win32"
	devenv.com "Updater\Updater.sln" /build "Release|Win32"

"$(OUTPUT_DIR)\ZRColaEn32D.3.msi" \
"$(OUTPUT_DIR)\ZRColaSl32D.3.msi" ::
	devenv.com "MSI\MSICA\MSICA.sln" /build "Debug|Win32"
	devenv.com "Updater\Updater.sln" /build "Debug|Win32"

"$(OUTPUT_DIR)\ZRColaEn64.3.msi" \
"$(OUTPUT_DIR)\ZRColaSl64.3.msi" ::
	devenv.com "MSI\MSICA\MSICA.sln" /build "Release|x64"
	devenv.com "Updater\Updater.sln" /build "Release|x64"

"$(OUTPUT_DIR)\ZRColaEn64D.3.msi" \
"$(OUTPUT_DIR)\ZRColaSl64D.3.msi" ::
	devenv.com "MSI\MSICA\MSICA.sln" /build "Debug|x64"
	devenv.com "Updater\Updater.sln" /build "Debug|x64"

"$(OUTPUT_DIR)\ZRColaEn32.3.msi" ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) LANG=En PLAT=Win32 CFG=Release
	cd "$(MAKEDIR)"

"$(OUTPUT_DIR)\ZRColaEn32D.3.msi" ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) LANG=En PLAT=Win32 CFG=Debug
	cd "$(MAKEDIR)"

"$(OUTPUT_DIR)\ZRColaEn64.3.msi" ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) LANG=En PLAT=x64 CFG=Release
	cd "$(MAKEDIR)"

"$(OUTPUT_DIR)\ZRColaEn64D.3.msi" ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) LANG=En PLAT=x64 CFG=Debug
	cd "$(MAKEDIR)"

"$(OUTPUT_DIR)\ZRColaSl32.3.msi" ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) LANG=Sl PLAT=Win32 CFG=Release
	cd "$(MAKEDIR)"

"$(OUTPUT_DIR)\ZRColaSl32D.3.msi" ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) LANG=Sl PLAT=Win32 CFG=Debug
	cd "$(MAKEDIR)"

"$(OUTPUT_DIR)\ZRColaSl64.3.msi" ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) LANG=Sl PLAT=x64 CFG=Release
	cd "$(MAKEDIR)"

"$(OUTPUT_DIR)\ZRColaSl64D.3.msi" ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) LANG=Sl PLAT=x64 CFG=Debug
	cd "$(MAKEDIR)"

!ENDIF
