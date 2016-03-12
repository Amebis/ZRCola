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
!ELSE
PLAT=Win32
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
	devenv.com "ZRCola.sln" /clean "Release|Win32"
	devenv.com "ZRCola.sln" /clean "Debug|Win32"
	devenv.com "ZRCola.sln" /clean "Release|x64"
	devenv.com "ZRCola.sln" /clean "Debug|x64"
	devenv.com "MSI\MSICA\MSICA.sln" /clean "Release|Win32"
	devenv.com "MSI\MSICA\MSICA.sln" /clean "Debug|Win32"
	devenv.com "MSI\MSICA\MSICA.sln" /clean "Release|x64"
	devenv.com "MSI\MSICA\MSICA.sln" /clean "Debug|x64"
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
!INCLUDE "MSI\include\MSIBuildCfg.mak"


######################################################################
# Main targets
######################################################################

All :: \
	Setup

Setup : \
	"$(OUTPUT_DIR)\Setup" \
	"$(OUTPUT_DIR)\Setup\ZRColaEn32.msi" \
	"$(OUTPUT_DIR)\Setup\ZRColaEn64.msi" \
	"$(OUTPUT_DIR)\Setup\ZRColaSl32.msi" \
	"$(OUTPUT_DIR)\Setup\ZRColaSl64.msi"

SetupDebug : \
	"$(OUTPUT_DIR)\Setup" \
	"$(OUTPUT_DIR)\Setup\ZRColaEn32D.msi" \
	"$(OUTPUT_DIR)\Setup\ZRColaEn64D.msi" \
	"$(OUTPUT_DIR)\Setup\ZRColaSl32D.msi" \
	"$(OUTPUT_DIR)\Setup\ZRColaSl64D.msi"

Register :: \
	RegisterShortcuts

Unregister :: \
	UnregisterShortcuts

RegisterShortcuts : \
	"$(ALLUSERSPROFILE)\Microsoft\Windows\Start Menu\Programs\ZRCola" \
	"$(ALLUSERSPROFILE)\Microsoft\Windows\Start Menu\Programs\ZRCola\ZRCola.lnk"

UnregisterShortcuts :
	-if exist "$(ALLUSERSPROFILE)\Microsoft\Windows\Start Menu\Programs\ZRCola" rd /s /q "$(ALLUSERSPROFILE)\Microsoft\Windows\Start Menu\Programs\ZRCola"


######################################################################
# Folder creation
######################################################################

"$(OUTPUT_DIR)" \
"$(OUTPUT_DIR)\Setup" \
"$(ALLUSERSPROFILE)\Microsoft\Windows\Start Menu\Programs\ZRCola" :
	if not exist $@ md $@

"$(OUTPUT_DIR)\Setup" : "$(OUTPUT_DIR)"


######################################################################
# File copy
######################################################################

"$(OUTPUT_DIR)\Setup\ZRColaEn32.msi" \
$(REDIST_EN_WIN32) : "$(OUTPUT_DIR)\ZRColaEn32.3.msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\ZRColaEn32D.msi" \
$(REDIST_EN_WIN32) : "$(OUTPUT_DIR)\ZRColaEn32D.3.msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\ZRColaEn64.msi" \
$(REDIST_EN_X64) : "$(OUTPUT_DIR)\ZRColaEn64.3.msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\ZRColaEn64D.msi" \
$(REDIST_EN_X64) : "$(OUTPUT_DIR)\ZRColaEn64D.3.msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\ZRColaSl32.msi" \
$(REDIST_SL_WIN32) : "$(OUTPUT_DIR)\ZRColaSl32.3.msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\ZRColaSl32D.msi" \
$(REDIST_SL_WIN32) : "$(OUTPUT_DIR)\ZRColaSl32D.3.msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\ZRColaSl64.msi" \
$(REDIST_SL_X64) : "$(OUTPUT_DIR)\ZRColaSl64.3.msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\ZRColaSl64D.msi" \
$(REDIST_SL_X64) : "$(OUTPUT_DIR)\ZRColaSl64D.3.msi"
	copy /y $** $@ > NUL


######################################################################
# Shortcut creation
######################################################################

"$(ALLUSERSPROFILE)\Microsoft\Windows\Start Menu\Programs\ZRCola\ZRCola.lnk" : "$(OUTPUT_DIR)\$(PLAT).Debug\ZRCola.exe"
	cscript.exe "bin\MkLnk.wsf" //Nologo $@ $**


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

"$(OUTPUT_DIR)\ZRColaEn32D.3.msi" \
"$(OUTPUT_DIR)\ZRColaSl32D.3.msi" ::
	devenv.com "MSI\MSICA\MSICA.sln" /build "Debug|Win32"

"$(OUTPUT_DIR)\ZRColaEn64.3.msi" \
"$(OUTPUT_DIR)\ZRColaSl64.3.msi" ::
	devenv.com "MSI\MSICA\MSICA.sln" /build "Release|x64"

"$(OUTPUT_DIR)\ZRColaEn64D.3.msi" \
"$(OUTPUT_DIR)\ZRColaSl64D.3.msi" ::
	devenv.com "MSI\MSICA\MSICA.sln" /build "Debug|x64"

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
