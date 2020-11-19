#
#    Copyright 1991-2020 Amebis
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
PUBLISH_DIR=\\amebis.doma\Splet\WWW\IIS-ext\prenos.amebis.si\ZRCola

!IF "$(PROCESSOR_ARCHITECTURE)" == "AMD64"
PLAT=x64
REG_FLAGS=/f /reg:64
REG_FLAGS32=/f /reg:32
PROGRAM_FILES_32=C:\Program Files (x86)
!ELSEIF "$(PROCESSOR_ARCHITECTURE)" == "ARM64"
PLAT=ARM64
REG_FLAGS=/f /reg:64
REG_FLAGS32=/f /reg:32
PROGRAM_FILES_32=C:\Program Files (x86)
!ELSE
PLAT=Win32
REG_FLAGS=/f
PROGRAM_FILES_32=C:\Program Files
!ENDIF
MSBUILDFLAGS=/v:m /m


All ::

Clean :: \
	CleanSetup

CleanSetup ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean
	cd "$(MAKEDIR)\MSI\MSIBuild\Version"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean
	cd "$(MAKEDIR)"
	-if exist "$(OUTPUT_DIR)\catalog.xml" del /f /q "$(OUTPUT_DIR)\catalog.xml"

!IFNDEF HAS_VERSION

######################################################################
# 1st Phase
# - Version info parsing
######################################################################

All \
Setup \
SetupDebug \
Register \
Unregister \
Localization \
PublishPre \
Publish ::
	cd "MSI\MSIBuild\Version"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Version
	cd "$(MAKEDIR)"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) HAS_VERSION=1 $@

!ELSE

######################################################################
# 2nd Phase
# - The version is known, do the rest.
######################################################################

!INCLUDE "MSI\MSIBuild\Version\Version.mak"
!INCLUDE "include\MSIBuildCfg.mak"

PUBLISH_PACKAGE_URL=http://prenos.amebis.si/ZRCola/$(MSIBUILD_VERSION_STR)

######################################################################
# Main targets
######################################################################

All :: \
	Setup


######################################################################
# Setup
######################################################################

Setup :: \
	Localization \
	SetupCompile \
	SetupPackage \

SetupDebug :: \
	Localization \
	SetupDebugCompile \
	SetupDebugPackage \


######################################################################
# Publishing
######################################################################

Publish :: \
	PublishPre \
	"$(PUBLISH_DIR)" \
	"$(PUBLISH_DIR)\catalog-0001.xml"


######################################################################
# Registration
######################################################################

Register :: \
	Localization \
	RegisterCompile \
	RegisterSettings \
	InstallFonts \
	RegisterShortcuts

Unregister :: \
	UnregisterShortcuts \
	UninstallFonts \
	UnregisterSettings

RegisterCompile ::
	msbuild.exe $(MSBUILDFLAGS) "ZRCola.sln" /t:Build /p:Platform=$(PLAT) /p:Configuration=Debug

RegisterSettings ::
	reg.exe add "HKLM\Software\Amebis\ZRCola" /v "Language"                   /t REG_SZ /d "en_US"                           $(REG_FLAGS) > NUL
	reg.exe add "HKLM\Software\Amebis\ZRCola" /v "LocalizationRepositoryPath" /t REG_SZ /d "$(MAKEDIR)\$(OUTPUT_DIR)\locale" $(REG_FLAGS) > NUL
	reg.exe add "HKLM\Software\Amebis\ZRCola" /v "DatabasePath"               /t REG_SZ /d "$(MAKEDIR)\$(OUTPUT_DIR)\data"   $(REG_FLAGS) > NUL
!IF "$(PROCESSOR_ARCHITECTURE)" == "AMD64" || "$(PROCESSOR_ARCHITECTURE)" == "ARM64"
	reg.exe add "HKLM\Software\Amebis\ZRCola" /v "Language"                   /t REG_SZ /d "en_US"                           $(REG_FLAGS32) > NUL
	reg.exe add "HKLM\Software\Amebis\ZRCola" /v "LocalizationRepositoryPath" /t REG_SZ /d "$(MAKEDIR)\$(OUTPUT_DIR)\locale" $(REG_FLAGS32) > NUL
	reg.exe add "HKLM\Software\Amebis\ZRCola" /v "DatabasePath"               /t REG_SZ /d "$(MAKEDIR)\$(OUTPUT_DIR)\data"   $(REG_FLAGS32) > NUL
!ENDIF

UnregisterSettings ::
	-reg.exe delete "HKLM\Software\Amebis\ZRCola" /v "Language"                   $(REG_FLAGS) > NUL
	-reg.exe delete "HKLM\Software\Amebis\ZRCola" /v "LocalizationRepositoryPath" $(REG_FLAGS) > NUL
	-reg.exe delete "HKLM\Software\Amebis\ZRCola" /v "DatabasePath"               $(REG_FLAGS) > NUL
!IF "$(PROCESSOR_ARCHITECTURE)" == "AMD64" || "$(PROCESSOR_ARCHITECTURE)" == "ARM64"
	-reg.exe delete "HKLM\Software\Amebis\ZRCola" /v "Language"                   $(REG_FLAGS32) > NUL
	-reg.exe delete "HKLM\Software\Amebis\ZRCola" /v "LocalizationRepositoryPath" $(REG_FLAGS32) > NUL
	-reg.exe delete "HKLM\Software\Amebis\ZRCola" /v "DatabasePath"               $(REG_FLAGS32) > NUL
!ENDIF

InstallFonts :: \
	"$(WINDIR)\Fonts\ZRCola_Re.otf" \
	"$(WINDIR)\Fonts\ZRCola_It.otf" \
	"$(WINDIR)\Fonts\ZRCola_Bd.otf" \
	"$(WINDIR)\Fonts\ZRCola_BI.otf"
	reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola (TrueType)"             /t REG_SZ /d "ZRCola_Re.otf" $(REG_FLAGS) > NUL
	reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola Italic (TrueType)"      /t REG_SZ /d "ZRCola_It.otf" $(REG_FLAGS) > NUL
	reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola Bold (TrueType)"        /t REG_SZ /d "ZRCola_Bd.otf" $(REG_FLAGS) > NUL
	reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola Bold Italic (TrueType)" /t REG_SZ /d "ZRCola_BI.otf" $(REG_FLAGS) > NUL
	reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\FontSubstitutes" /v "00 ZRCola"                     /t REG_SZ /d "ZRCola"        $(REG_FLAGS) > NUL

UninstallFonts ::
	-reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola (TrueType)"             $(REG_FLAGS) > NUL
	-reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola Italic (TrueType)"      $(REG_FLAGS) > NUL
	-reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola Bold (TrueType)"        $(REG_FLAGS) > NUL
	-reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola Bold Italic (TrueType)" $(REG_FLAGS) > NUL
	-reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\FontSubstitutes" /v "00 ZRCola"                     $(REG_FLAGS) > NUL
	-if exist "$(WINDIR)\Fonts\ZRCola_Re.otf" del /f /q "$(WINDIR)\Fonts\ZRCola_Re.otf"
	-if exist "$(WINDIR)\Fonts\ZRCola_It.otf" del /f /q "$(WINDIR)\Fonts\ZRCola_It.otf"
	-if exist "$(WINDIR)\Fonts\ZRCola_Bd.otf" del /f /q "$(WINDIR)\Fonts\ZRCola_Bd.otf"
	-if exist "$(WINDIR)\Fonts\ZRCola_BI.otf" del /f /q "$(WINDIR)\Fonts\ZRCola_BI.otf"

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
"$(OUTPUT_DIR)\locale" \
"$(PUBLISH_DIR)" \
"$(PUBLISH_DIR)\_latest" \
"$(PUBLISH_DIR)\$(MSIBUILD_VERSION_STR)" \
"$(PROGRAMDATA)\Microsoft\Windows\Start Menu\Programs\ZRCola" :
	if not exist $@ md $@

"$(OUTPUT_DIR)\Keys" \
"$(OUTPUT_DIR)\locale" : "$(OUTPUT_DIR)"

"$(PUBLISH_DIR)\_latest" \
"$(PUBLISH_DIR)\$(MSIBUILD_VERSION_STR)" : "$(PUBLISH_DIR)"


######################################################################
# File copy
######################################################################

"$(WINDIR)\Fonts\ZRCola_Re.otf" : "$(OUTPUT_DIR)\ZRCola_Re.otf"
	copy /y $** $@ > NUL

"$(WINDIR)\Fonts\ZRCola_It.otf" : "$(OUTPUT_DIR)\ZRCola_It.otf"
	copy /y $** $@ > NUL

"$(WINDIR)\Fonts\ZRCola_Bd.otf" : "$(OUTPUT_DIR)\ZRCola_Bd.otf"
	copy /y $** $@ > NUL

"$(WINDIR)\Fonts\ZRCola_BI.otf" : "$(OUTPUT_DIR)\ZRCola_BI.otf"
	copy /y $** $@ > NUL


######################################################################
# Shortcut creation
######################################################################

"$(PROGRAMDATA)\Microsoft\Windows\Start Menu\Programs\ZRCola\ZRCola.lnk" : "$(OUTPUT_DIR)\$(PLAT).Debug\ZRCola.exe"
	cscript.exe "bin\MkLnk.wsf" //Nologo $@ "$(MAKEDIR)\$(OUTPUT_DIR)\$(PLAT).Debug\ZRCola.exe"


######################################################################
# Building
######################################################################

"$(PUBLISH_DIR)\catalog-0001.xml" : "$(OUTPUT_DIR)\catalog.xml"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\catalog.xml" : \
	"$(OUTPUT_DIR)\ZRColaEn-x86.msi" \
	"$(OUTPUT_DIR)\ZRColaEn-amd64.msi" \
	"$(OUTPUT_DIR)\ZRColaEn-arm64.msi" \
	"$(OUTPUT_DIR)\ZRColaRu-x86.msi" \
	"$(OUTPUT_DIR)\ZRColaRu-amd64.msi" \
	"$(OUTPUT_DIR)\ZRColaRu-arm64.msi" \
	"$(OUTPUT_DIR)\ZRColaSl-x86.msi" \
	"$(OUTPUT_DIR)\ZRColaSl-amd64.msi" \
	"$(OUTPUT_DIR)\ZRColaSl-arm64.msi"
	copy /y "$(PUBLISH_DIR)\catalog-0001.xml" "$(@:"=).tmp" > NUL
	"$(OUTPUT_DIR)\$(PLAT).Release\UpdPublish.exe" "$(@:"=).tmp" "$(@:"=).tmp" win-x86   en_US "$(PUBLISH_PACKAGE_URL)/ZRColaEn-x86.msi"   -f "$(OUTPUT_DIR)\ZRColaEn-x86.msi"
	"$(OUTPUT_DIR)\$(PLAT).Release\UpdPublish.exe" "$(@:"=).tmp" "$(@:"=).tmp" win-amd64 en_US "$(PUBLISH_PACKAGE_URL)/ZRColaEn-amd64.msi" -f "$(OUTPUT_DIR)\ZRColaEn-amd64.msi"
	"$(OUTPUT_DIR)\$(PLAT).Release\UpdPublish.exe" "$(@:"=).tmp" "$(@:"=).tmp" win-arm64 en_US "$(PUBLISH_PACKAGE_URL)/ZRColaEn-arm64.msi" -f "$(OUTPUT_DIR)\ZRColaEn-arm64.msi"
	"$(OUTPUT_DIR)\$(PLAT).Release\UpdPublish.exe" "$(@:"=).tmp" "$(@:"=).tmp" win-x86   ru_RU "$(PUBLISH_PACKAGE_URL)/ZRColaRu-x86.msi"   -f "$(OUTPUT_DIR)\ZRColaRu-x86.msi"
	"$(OUTPUT_DIR)\$(PLAT).Release\UpdPublish.exe" "$(@:"=).tmp" "$(@:"=).tmp" win-amd64 ru_RU "$(PUBLISH_PACKAGE_URL)/ZRColaRu-amd64.msi" -f "$(OUTPUT_DIR)\ZRColaRu-amd64.msi"
	"$(OUTPUT_DIR)\$(PLAT).Release\UpdPublish.exe" "$(@:"=).tmp" "$(@:"=).tmp" win-arm64 ru_RU "$(PUBLISH_PACKAGE_URL)/ZRColaRu-arm64.msi" -f "$(OUTPUT_DIR)\ZRColaRu-arm64.msi"
	"$(OUTPUT_DIR)\$(PLAT).Release\UpdPublish.exe" "$(@:"=).tmp" "$(@:"=).tmp" win-x86   sl_SI "$(PUBLISH_PACKAGE_URL)/ZRColaSl-x86.msi"   -f "$(OUTPUT_DIR)\ZRColaSl-x86.msi"
	"$(OUTPUT_DIR)\$(PLAT).Release\UpdPublish.exe" "$(@:"=).tmp" "$(@:"=).tmp" win-amd64 sl_SI "$(PUBLISH_PACKAGE_URL)/ZRColaSl-amd64.msi" -f "$(OUTPUT_DIR)\ZRColaSl-amd64.msi"
	"$(OUTPUT_DIR)\$(PLAT).Release\UpdPublish.exe" "$(@:"=).tmp" "$(@:"=).tmp" win-arm64 sl_SI "$(PUBLISH_PACKAGE_URL)/ZRColaSl-arm64.msi" -f "$(OUTPUT_DIR)\ZRColaSl-arm64.msi"
	"$(OUTPUT_DIR)\$(PLAT).Release\UpdSignXML.exe" "$(@:"=).tmp" $@
	-if exist "$(@:"=).tmp" del /f /q "$(@:"=).tmp"

!ENDIF

######################################################################
# Platform Specific
######################################################################

PLAT=Win32
PLAT_SUFFIX=-x86
!INCLUDE "MakefilePlat.mak"

PLAT=x64
PLAT_SUFFIX=-amd64
!INCLUDE "MakefilePlat.mak"

PLAT=ARM64
PLAT_SUFFIX=-arm64
!INCLUDE "MakefilePlat.mak"


######################################################################
# Language Specific
######################################################################

LANG=en_US
LANG_BASE=en
LANG_SUFFIX=En
!INCLUDE "MakefileLang.mak"

LANG=ru_RU
LANG_BASE=ru
LANG_SUFFIX=Ru
!INCLUDE "MakefileLang.mak"

LANG=sl_SI
LANG_BASE=sl
LANG_SUFFIX=Sl
!INCLUDE "MakefileLang.mak"
