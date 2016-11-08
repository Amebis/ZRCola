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
PUBLISH_DIR=\\amebis.doma\Splet\WWW\Apache\www.amebis.si-prenos\ZRCola

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
	-if exist "$(OUTPUT_DIR)\locale\de_DE\wxstd.mo"        del /f /q "$(OUTPUT_DIR)\locale\de_DE\wxstd.mo"
	-if exist "$(OUTPUT_DIR)\locale\ru_RU\wxstd.mo"        del /f /q "$(OUTPUT_DIR)\locale\ru_RU\wxstd.mo"
	-if exist "$(OUTPUT_DIR)\locale\sl_SI\wxstd.mo"        del /f /q "$(OUTPUT_DIR)\locale\sl_SI\wxstd.mo"
	-if exist "$(OUTPUT_DIR)\locale\de_DE\ZRCola-zrcdb.mo" del /f /q "$(OUTPUT_DIR)\locale\de_DE\ZRCola-zrcdb.mo"
	-if exist "$(OUTPUT_DIR)\locale\ru_RU\ZRCola-zrcdb.mo" del /f /q "$(OUTPUT_DIR)\locale\ru_RU\ZRCola-zrcdb.mo"
	-if exist "$(OUTPUT_DIR)\locale\sl_SI\ZRCola-zrcdb.mo" del /f /q "$(OUTPUT_DIR)\locale\sl_SI\ZRCola-zrcdb.mo"
	-if exist "$(OUTPUT_DIR)\ZRCola32.ddf"                 del /f /q "$(OUTPUT_DIR)\ZRCola32.ddf"
	-if exist "$(OUTPUT_DIR)\ZRCola32D.ddf"                del /f /q "$(OUTPUT_DIR)\ZRCola32D.ddf"
	-if exist "$(OUTPUT_DIR)\ZRCola64.ddf"                 del /f /q "$(OUTPUT_DIR)\ZRCola64.ddf"
	-if exist "$(OUTPUT_DIR)\ZRCola64D.ddf"                del /f /q "$(OUTPUT_DIR)\ZRCola64D.ddf"
	-if exist "$(OUTPUT_DIR)\ZRCola32.cab"                 del /f /q "$(OUTPUT_DIR)\ZRCola32.cab"
	-if exist "$(OUTPUT_DIR)\ZRCola32.inf"                 del /f /q "$(OUTPUT_DIR)\ZRCola32.inf"
	-if exist "$(OUTPUT_DIR)\ZRCola32.rpt"                 del /f /q "$(OUTPUT_DIR)\ZRCola32.rpt"
	-if exist "$(OUTPUT_DIR)\ZRCola32D.cab"                del /f /q "$(OUTPUT_DIR)\ZRCola32D.cab"
	-if exist "$(OUTPUT_DIR)\ZRCola32D.inf"                del /f /q "$(OUTPUT_DIR)\ZRCola32D.inf"
	-if exist "$(OUTPUT_DIR)\ZRCola32D.rpt"                del /f /q "$(OUTPUT_DIR)\ZRCola32D.rpt"
	-if exist "$(OUTPUT_DIR)\ZRCola64.cab"                 del /f /q "$(OUTPUT_DIR)\ZRCola64.cab"
	-if exist "$(OUTPUT_DIR)\ZRCola64.inf"                 del /f /q "$(OUTPUT_DIR)\ZRCola64.inf"
	-if exist "$(OUTPUT_DIR)\ZRCola64.rpt"                 del /f /q "$(OUTPUT_DIR)\ZRCola64.rpt"
	-if exist "$(OUTPUT_DIR)\ZRCola64D.cab"                del /f /q "$(OUTPUT_DIR)\ZRCola64D.cab"
	-if exist "$(OUTPUT_DIR)\ZRCola64D.inf"                del /f /q "$(OUTPUT_DIR)\ZRCola64D.inf"
	-if exist "$(OUTPUT_DIR)\ZRCola64D.rpt"                del /f /q "$(OUTPUT_DIR)\ZRCola64D.rpt"
	-if exist "$(OUTPUT_DIR)\catalog.xml"                  del /f /q "$(OUTPUT_DIR)\catalog.xml"

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
Publish :: "MSI\MSIBuild\Version\Version.mak"
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

PUBLISH_PACKAGE_DIR=$(PUBLISH_DIR)\$(MSIBUILD_VERSION_STR)
PUBLISH_PACKAGE_URL=http://www.amebis.si/prenos/ZRCola/$(MSIBUILD_VERSION_STR)

######################################################################
# Main targets
######################################################################

All :: \
	Setup

Setup :: \
#	"$(OUTPUT_DIR)\ZRColaDe32.msi" \
#	"$(OUTPUT_DIR)\ZRColaDe64.msi" \
	"$(OUTPUT_DIR)\ZRColaEn32.msi" \
	"$(OUTPUT_DIR)\ZRColaEn64.msi" \
	"$(OUTPUT_DIR)\ZRColaRu32.msi" \
	"$(OUTPUT_DIR)\ZRColaRu64.msi" \
	"$(OUTPUT_DIR)\ZRColaSl32.msi" \
	"$(OUTPUT_DIR)\ZRColaSl64.msi"

SetupDebug :: \
#	"$(OUTPUT_DIR)\ZRColaDe32D.msi" \
#	"$(OUTPUT_DIR)\ZRColaDe64D.msi" \
	"$(OUTPUT_DIR)\ZRColaEn32D.msi" \
	"$(OUTPUT_DIR)\ZRColaEn64D.msi" \
	"$(OUTPUT_DIR)\ZRColaRu32D.msi" \
	"$(OUTPUT_DIR)\ZRColaRu64D.msi" \
	"$(OUTPUT_DIR)\ZRColaSl32D.msi" \
	"$(OUTPUT_DIR)\ZRColaSl64D.msi"


######################################################################
# Publishing
######################################################################

PublishPre :: \
	"$(PUBLISH_PACKAGE_DIR)" \
#	"$(PUBLISH_PACKAGE_DIR)\ZRColaDe32.msi" \
#	"$(PUBLISH_PACKAGE_DIR)\ZRColaDe64.msi" \
	"$(PUBLISH_PACKAGE_DIR)\ZRColaEn32.msi" \
	"$(PUBLISH_PACKAGE_DIR)\ZRColaEn64.msi" \
	"$(PUBLISH_PACKAGE_DIR)\ZRColaRu32.msi" \
	"$(PUBLISH_PACKAGE_DIR)\ZRColaRu64.msi" \
	"$(PUBLISH_PACKAGE_DIR)\ZRColaSl32.msi" \
	"$(PUBLISH_PACKAGE_DIR)\ZRColaSl64.msi" \
	"$(PUBLISH_DIR)" \
#	"$(PUBLISH_DIR)\ZRColaInstallDe.exe" \
	"$(PUBLISH_DIR)\ZRColaInstallEn.exe" \
	"$(PUBLISH_DIR)\ZRColaInstallRu.exe" \
	"$(PUBLISH_DIR)\ZRColaInstallSl.exe"

Publish :: \
	PublishPre \
	"$(PUBLISH_DIR)\catalog-0000.xml"


######################################################################
# Registration
######################################################################

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
	"$(WINDIR)\Fonts\ZRCola_Re.otf" \
	"$(WINDIR)\Fonts\ZRCola_It.otf" \
	"$(WINDIR)\Fonts\ZRCola_Bd.otf" \
	"$(WINDIR)\Fonts\ZRCola_BI.otf"
	reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola (TrueType)"             /t REG_SZ /d "ZRCola_Re.otf" $(REG_FLAGS) > NUL
	reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola Italic (TrueType)"      /t REG_SZ /d "ZRCola_It.otf" $(REG_FLAGS) > NUL
	reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola Bold (TrueType)"        /t REG_SZ /d "ZRCola_Bd.otf" $(REG_FLAGS) > NUL
	reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola Bold Italic (TrueType)" /t REG_SZ /d "ZRCola_BI.otf" $(REG_FLAGS) > NUL
	reg.exe add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\FontSubstitutes" /v "00 ZRCola"                     /t REG_SZ /d "ZRCola"        $(REG_FLAGS) > NUL

UninstallFonts::
	-reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola (TrueType)"             $(REG_FLAGS) > NUL
	-reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola Italic (TrueType)"      $(REG_FLAGS) > NUL
	-reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola Bold (TrueType)"        $(REG_FLAGS) > NUL
	-reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"           /v "ZRCola Bold Italic (TrueType)" $(REG_FLAGS) > NUL
	-reg.exe delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\FontSubstitutes" /v "00 ZRCola"                     $(REG_FLAGS) > NUL
	-if exist "$(WINDIR)\Fonts\ZRCola_Re.otf" rd /s /q "$(WINDIR)\Fonts\ZRCola_Re.otf"
	-if exist "$(WINDIR)\Fonts\ZRCola_It.otf" rd /s /q "$(WINDIR)\Fonts\ZRCola_It.otf"
	-if exist "$(WINDIR)\Fonts\ZRCola_Bd.otf" rd /s /q "$(WINDIR)\Fonts\ZRCola_Bd.otf"
	-if exist "$(WINDIR)\Fonts\ZRCola_BI.otf" rd /s /q "$(WINDIR)\Fonts\ZRCola_BI.otf"

RegisterShortcuts :: \
	"$(PROGRAMDATA)\Microsoft\Windows\Start Menu\Programs\ZRCola" \
	"$(PROGRAMDATA)\Microsoft\Windows\Start Menu\Programs\ZRCola\ZRCola.lnk"

UnregisterShortcuts ::
	-if exist "$(PROGRAMDATA)\Microsoft\Windows\Start Menu\Programs\ZRCola" rd /s /q "$(PROGRAMDATA)\Microsoft\Windows\Start Menu\Programs\ZRCola"

######################################################################
# Localization
######################################################################

Localization :: \
	"$(OUTPUT_DIR)\locale\de_DE" \
	"$(OUTPUT_DIR)\locale\de_DE\wxstd.mo" \
	"$(OUTPUT_DIR)\locale\de_DE\ZRCola-zrcdb.mo" \
	"$(OUTPUT_DIR)\locale\ru_RU" \
	"$(OUTPUT_DIR)\locale\ru_RU\wxstd.mo" \
	"$(OUTPUT_DIR)\locale\ru_RU\ZRCola-zrcdb.mo" \
	"$(OUTPUT_DIR)\locale\sl_SI" \
	"$(OUTPUT_DIR)\locale\sl_SI\wxstd.mo" \
	"$(OUTPUT_DIR)\locale\sl_SI\ZRCola-zrcdb.mo"


######################################################################
# Folder creation
######################################################################

"$(OUTPUT_DIR)" \
"$(OUTPUT_DIR)\Keys" \
"$(OUTPUT_DIR)\locale" \
"$(OUTPUT_DIR)\locale\de_DE" \
"$(OUTPUT_DIR)\locale\ru_RU" \
"$(OUTPUT_DIR)\locale\sl_SI" \
"$(PUBLISH_DIR)" \
"$(PUBLISH_PACKAGE_DIR)" \
"$(PROGRAMDATA)\Microsoft\Windows\Start Menu\Programs\ZRCola" :
	if not exist $@ md $@

"$(OUTPUT_DIR)\Keys" \
"$(OUTPUT_DIR)\locale" : "$(OUTPUT_DIR)"

"$(OUTPUT_DIR)\locale\de_DE" \
"$(OUTPUT_DIR)\locale\ru_RU" \
"$(OUTPUT_DIR)\locale\sl_SI" : "$(OUTPUT_DIR)\locale"

"$(PUBLISH_PACKAGE_DIR)" : "$(PUBLISH_DIR)"


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

"$(OUTPUT_DIR)\locale\de_DE\wxstd.mo" : "$(WXWIN)\locale\de.po"
	msgfmt.exe --output-file=$@ --alignment=1 --endianness=little $**

"$(OUTPUT_DIR)\locale\ru_RU\wxstd.mo" : "$(WXWIN)\locale\ru.po"
	msgfmt.exe --output-file=$@ --alignment=1 --endianness=little $**

"$(OUTPUT_DIR)\locale\sl_SI\wxstd.mo" : "$(WXWIN)\locale\sl.po"
	msgfmt.exe --output-file=$@ --alignment=1 --endianness=little $**

"$(OUTPUT_DIR)\locale\de_DE\ZRCola-zrcdb.mo" : "$(OUTPUT_DIR)\locale\de_DE\ZRCola-zrcdb.po"
	msgfmt.exe --output-file=$@ --alignment=1 --endianness=little $**

"$(OUTPUT_DIR)\locale\ru_RU\ZRCola-zrcdb.mo" : "$(OUTPUT_DIR)\locale\ru_RU\ZRCola-zrcdb.po"
	msgfmt.exe --output-file=$@ --alignment=1 --endianness=little $**

"$(OUTPUT_DIR)\locale\sl_SI\ZRCola-zrcdb.mo" : "$(OUTPUT_DIR)\locale\sl_SI\ZRCola-zrcdb.po"
	msgfmt.exe --output-file=$@ --alignment=1 --endianness=little $**

"$(OUTPUT_DIR)\Win32.Release\libZRCola10u_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Release\libZRColaUI10u_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Release\stdex10u_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Release\wxExtend10u_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Release\ZRCola.exe" :: Localization

"$(OUTPUT_DIR)\Win32.Debug\libZRCola10ud_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Debug\libZRColaUI10ud_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Debug\stdex10ud_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Debug\wxExtend10ud_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Debug\ZRCola.exe" :: Localization

"$(OUTPUT_DIR)\x64.Release\libZRCola10u_vc100.dll" \
"$(OUTPUT_DIR)\x64.Release\libZRColaUI10u_vc100.dll" \
"$(OUTPUT_DIR)\x64.Release\stdex10u_vc100.dll" \
"$(OUTPUT_DIR)\x64.Release\wxExtend10u_vc100.dll" \
"$(OUTPUT_DIR)\x64.Release\ZRCola.exe" :: Localization

"$(OUTPUT_DIR)\x64.Debug\libZRCola10ud_vc100.dll" \
"$(OUTPUT_DIR)\x64.Debug\libZRColaUI10ud_vc100.dll" \
"$(OUTPUT_DIR)\x64.Debug\stdex10ud_vc100.dll" \
"$(OUTPUT_DIR)\x64.Debug\wxExtend10ud_vc100.dll" \
"$(OUTPUT_DIR)\x64.Debug\ZRCola.exe" :: Localization

"$(OUTPUT_DIR)\Win32.Release\libZRCola10u_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Release\libZRColaUI10u_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Release\stdex10u_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Release\wxExtend10u_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Release\ZRCola.exe" ::
	devenv.com "ZRCola.sln" /build "Release|Win32"

"$(OUTPUT_DIR)\Win32.Debug\libZRCola10ud_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Debug\libZRColaUI10ud_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Debug\stdex10ud_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Debug\wxExtend10ud_vc100.dll" \
"$(OUTPUT_DIR)\Win32.Debug\ZRCola.exe" ::
	devenv.com "ZRCola.sln" /build "Debug|Win32"

"$(OUTPUT_DIR)\x64.Release\libZRCola10u_vc100.dll" \
"$(OUTPUT_DIR)\x64.Release\libZRColaUI10u_vc100.dll" \
"$(OUTPUT_DIR)\x64.Release\stdex10u_vc100.dll" \
"$(OUTPUT_DIR)\x64.Release\wxExtend10u_vc100.dll" \
"$(OUTPUT_DIR)\x64.Release\ZRCola.exe" ::
	devenv.com "ZRCola.sln" /build "Release|x64"

"$(OUTPUT_DIR)\x64.Debug\libZRCola10ud_vc100.dll" \
"$(OUTPUT_DIR)\x64.Debug\libZRColaUI10ud_vc100.dll" \
"$(OUTPUT_DIR)\x64.Debug\stdex10ud_vc100.dll" \
"$(OUTPUT_DIR)\x64.Debug\wxExtend10ud_vc100.dll" \
"$(OUTPUT_DIR)\x64.Debug\ZRCola.exe" ::
	devenv.com "ZRCola.sln" /build "Debug|x64"

"$(OUTPUT_DIR)\ZRCola32.ddf" : \
#	"$(OUTPUT_DIR)\ZRCola.de_DE.32.2.msi" \
	"$(OUTPUT_DIR)\ZRCola.en_US.32.2.msi" \
	"$(OUTPUT_DIR)\ZRCola.ru_RU.32.2.msi" \
	"$(OUTPUT_DIR)\ZRCola.sl_SI.32.2.msi"
	-if exist $@ del /f /q $@
	-if exist "$(@:"=).tmp" del /f /q "$(@:"=).tmp"
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:MakeDDF //Nologo "$(@:"=).tmp" $** /O:"$(OUTPUT_DIR)\ZRCola32" /C:LZX
	move /y "$(@:"=).tmp" $@ > NUL

"$(OUTPUT_DIR)\ZRCola32D.ddf" : \
#	"$(OUTPUT_DIR)\ZRCola.de_DE.32D.2.msi" \
	"$(OUTPUT_DIR)\ZRCola.en_US.32D.2.msi" \
	"$(OUTPUT_DIR)\ZRCola.ru_RU.32D.2.msi" \
	"$(OUTPUT_DIR)\ZRCola.sl_SI.32D.2.msi"
	-if exist $@ del /f /q $@
	-if exist "$(@:"=).tmp" del /f /q "$(@:"=).tmp"
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:MakeDDF //Nologo "$(@:"=).tmp" $** /O:"$(OUTPUT_DIR)\ZRCola32D" /C:LZX
	move /y "$(@:"=).tmp" $@ > NUL

"$(OUTPUT_DIR)\ZRCola64.ddf" : \
#	"$(OUTPUT_DIR)\ZRCola.de_DE.64.2.msi" \
	"$(OUTPUT_DIR)\ZRCola.en_US.64.2.msi" \
	"$(OUTPUT_DIR)\ZRCola.ru_RU.64.2.msi" \
	"$(OUTPUT_DIR)\ZRCola.sl_SI.64.2.msi"
	-if exist $@ del /f /q $@
	-if exist "$(@:"=).tmp" del /f /q "$(@:"=).tmp"
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:MakeDDF //Nologo "$(@:"=).tmp" $** /O:"$(OUTPUT_DIR)\ZRCola64" /C:LZX
	move /y "$(@:"=).tmp" $@ > NUL

"$(OUTPUT_DIR)\ZRCola64D.ddf" : \
#	"$(OUTPUT_DIR)\ZRCola.de_DE.64D.2.msi" \
	"$(OUTPUT_DIR)\ZRCola.en_US.64D.2.msi" \
	"$(OUTPUT_DIR)\ZRCola.ru_RU.64D.2.msi" \
	"$(OUTPUT_DIR)\ZRCola.sl_SI.64D.2.msi"
	-if exist $@ del /f /q $@
	-if exist "$(@:"=).tmp" del /f /q "$(@:"=).tmp"
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:MakeDDF //Nologo "$(@:"=).tmp" $** /O:"$(OUTPUT_DIR)\ZRCola64D" /C:LZX
	move /y "$(@:"=).tmp" $@ > NUL

"$(OUTPUT_DIR)\ZRCola32.cab" \
"$(OUTPUT_DIR)\ZRCola32.inf" \
"$(OUTPUT_DIR)\ZRCola32.rpt" : "$(OUTPUT_DIR)\ZRCola32.ddf"
	makecab.exe /F $**

"$(OUTPUT_DIR)\ZRCola32D.cab" \
"$(OUTPUT_DIR)\ZRCola32D.inf" \
"$(OUTPUT_DIR)\ZRCola32D.rpt" : "$(OUTPUT_DIR)\ZRCola32D.ddf"
	makecab.exe /F $**

"$(OUTPUT_DIR)\ZRCola64.cab" \
"$(OUTPUT_DIR)\ZRCola64.inf" \
"$(OUTPUT_DIR)\ZRCola64.rpt" : "$(OUTPUT_DIR)\ZRCola64.ddf"
	makecab.exe /F $**

"$(OUTPUT_DIR)\ZRCola64D.cab" \
"$(OUTPUT_DIR)\ZRCola64D.inf" \
"$(OUTPUT_DIR)\ZRCola64D.rpt" : "$(OUTPUT_DIR)\ZRCola64D.ddf"
	makecab.exe /F $**

"$(PUBLISH_DIR)\catalog-0000.xml" : "$(OUTPUT_DIR)\catalog.xml"
	if exist $@  del /f /q $@
	output\$(PLAT).Release\UpdSignXML.exe $** $@

"$(OUTPUT_DIR)\catalog.xml" : \
#	"$(OUTPUT_DIR)\ZRColaDe32.msi" \
#	"$(OUTPUT_DIR)\ZRColaDe64.msi" \
	"$(OUTPUT_DIR)\ZRColaEn32.msi" \
	"$(OUTPUT_DIR)\ZRColaEn64.msi" \
	"$(OUTPUT_DIR)\ZRColaRu32.msi" \
	"$(OUTPUT_DIR)\ZRColaRu64.msi" \
	"$(OUTPUT_DIR)\ZRColaSl32.msi" \
	"$(OUTPUT_DIR)\ZRColaSl64.msi"
	-if exist $@ del /f /q $@
	-if exist "$(@:"=).tmp" del /f /q "$(@:"=).tmp"
	copy /y "$(PUBLISH_DIR)\catalog-0000.xml" "$(@:"=).tmp" > NUL
#	output\$(PLAT).Release\UpdPublish.exe "$(@:"=).tmp" "$(@:"=).tmp" win-x86   de_DE "$(PUBLISH_PACKAGE_URL)/ZRColaDe32.msi" -f "$(OUTPUT_DIR)\ZRColaDe32.msi"
#	output\$(PLAT).Release\UpdPublish.exe "$(@:"=).tmp" "$(@:"=).tmp" win-amd64 de_DE "$(PUBLISH_PACKAGE_URL)/ZRColaDe64.msi" -f "$(OUTPUT_DIR)\ZRColaDe64.msi"
	output\$(PLAT).Release\UpdPublish.exe "$(@:"=).tmp" "$(@:"=).tmp" win-x86   en_US "$(PUBLISH_PACKAGE_URL)/ZRColaEn32.msi" -f "$(OUTPUT_DIR)\ZRColaEn32.msi"
	output\$(PLAT).Release\UpdPublish.exe "$(@:"=).tmp" "$(@:"=).tmp" win-amd64 en_US "$(PUBLISH_PACKAGE_URL)/ZRColaEn64.msi" -f "$(OUTPUT_DIR)\ZRColaEn64.msi"
	output\$(PLAT).Release\UpdPublish.exe "$(@:"=).tmp" "$(@:"=).tmp" win-x86   ru_RU "$(PUBLISH_PACKAGE_URL)/ZRColaRu32.msi" -f "$(OUTPUT_DIR)\ZRColaRu32.msi"
	output\$(PLAT).Release\UpdPublish.exe "$(@:"=).tmp" "$(@:"=).tmp" win-amd64 ru_RU "$(PUBLISH_PACKAGE_URL)/ZRColaRu64.msi" -f "$(OUTPUT_DIR)\ZRColaRu64.msi"
	output\$(PLAT).Release\UpdPublish.exe "$(@:"=).tmp" "$(@:"=).tmp" win-x86   sl_SI "$(PUBLISH_PACKAGE_URL)/ZRColaSl32.msi" -f "$(OUTPUT_DIR)\ZRColaSl32.msi"
	output\$(PLAT).Release\UpdPublish.exe "$(@:"=).tmp" "$(@:"=).tmp" win-amd64 sl_SI "$(PUBLISH_PACKAGE_URL)/ZRColaSl64.msi" -f "$(OUTPUT_DIR)\ZRColaSl64.msi"
	move /y "$(@:"=).tmp" $@ > NUL

!ENDIF


#LANG=de_DE
#LANG_OUT=De
#!INCLUDE "MSILocal.mak"

LANG=en_US
LANG_OUT=En
!INCLUDE "MSILocal.mak"

LANG=ru_RU
LANG_OUT=Ru
!INCLUDE "MSILocal.mak"

LANG=sl_SI
LANG_OUT=Sl
!INCLUDE "MSILocal.mak"
