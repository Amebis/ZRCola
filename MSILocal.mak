#
#    Copyright 1991-2017 Amebis
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

!IF "$(PROCESSOR_ARCHITECTURE)" == "AMD64"
PROGRAM_FILES_32=C:\Program Files (x86)
!ELSE
PROGRAM_FILES_32=C:\Program Files
!ENDIF
DEVENV10=$(PROGRAM_FILES_32)\Microsoft Visual Studio 10.0\Common7\IDE\devenv.com


All ::

Clean ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean LANG=$(LANG) PLAT=Win32 CFG=Release
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean LANG=$(LANG) PLAT=Win32 CFG=Debug
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean LANG=$(LANG) PLAT=x64   CFG=Release
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) Clean LANG=$(LANG) PLAT=x64   CFG=Debug
	cd "$(MAKEDIR)"
	-if exist "$(OUTPUT_DIR)\ZRCola$(LANG_OUT)32.msi"  del /f /q "$(OUTPUT_DIR)\ZRCola$(LANG_OUT)32.msi"
	-if exist "$(OUTPUT_DIR)\ZRCola$(LANG_OUT)32D.msi" del /f /q "$(OUTPUT_DIR)\ZRCola$(LANG_OUT)32D.msi"
	-if exist "$(OUTPUT_DIR)\ZRCola$(LANG_OUT)64.msi"  del /f /q "$(OUTPUT_DIR)\ZRCola$(LANG_OUT)64.msi"
	-if exist "$(OUTPUT_DIR)\ZRCola$(LANG_OUT)64D.msi" del /f /q "$(OUTPUT_DIR)\ZRCola$(LANG_OUT)64D.msi"

!IFDEF HAS_VERSION

######################################################################
# 2nd Phase
# - The version is known, do the rest.
######################################################################

######################################################################
# File copy
######################################################################

"$(PUBLISH_PACKAGE_DIR)\ZRCola$(LANG_OUT)32.msi" : "$(OUTPUT_DIR)\ZRCola$(LANG_OUT)32.msi"
	copy /y $** $@ > NUL

"$(PUBLISH_PACKAGE_DIR)\ZRCola$(LANG_OUT)64.msi" : "$(OUTPUT_DIR)\ZRCola$(LANG_OUT)64.msi"
	copy /y $** $@ > NUL

"$(PUBLISH_DIR)\ZRColaInstall$(LANG_OUT).exe" : "$(OUTPUT_DIR)\Win32.Release\ZRColaInstall$(LANG_OUT).exe"
	copy /y $** $@ > NUL


######################################################################
# Building
######################################################################

"$(OUTPUT_DIR)\ZRCola.$(LANG).32.2.msi" :: Localization

"$(OUTPUT_DIR)\ZRCola.$(LANG).32D.2.msi" :: Localization

"$(OUTPUT_DIR)\ZRCola.$(LANG).64.2.msi" :: Localization

"$(OUTPUT_DIR)\ZRCola.$(LANG).64D.2.msi" :: Localization

"$(OUTPUT_DIR)\ZRCola.$(LANG).32.2.msi" ::
	"$(DEVENV10)" "ZRCola.sln"          /build "Release|Win32"
	"$(DEVENV10)" "MSI\MSICA\MSICA.sln" /build "Release|Win32"
	"$(DEVENV10)" "Updater\Updater.sln" /build "Release|Win32"

"$(OUTPUT_DIR)\ZRCola.$(LANG).32D.2.msi" ::
	"$(DEVENV10)" "ZRCola.sln"          /build "Debug|Win32"
	"$(DEVENV10)" "MSI\MSICA\MSICA.sln" /build "Debug|Win32"
	"$(DEVENV10)" "Updater\Updater.sln" /build "Debug|Win32"

"$(OUTPUT_DIR)\ZRCola.$(LANG).64.2.msi" ::
	"$(DEVENV10)" "ZRCola.sln"          /build "Release|x64"
	"$(DEVENV10)" "MSI\MSICA\MSICA.sln" /build "Release|x64"
	"$(DEVENV10)" "Updater\Updater.sln" /build "Release|x64"

"$(OUTPUT_DIR)\ZRCola.$(LANG).64D.2.msi" ::
	"$(DEVENV10)" "ZRCola.sln"          /build "Debug|x64"
	"$(DEVENV10)" "MSI\MSICA\MSICA.sln" /build "Debug|x64"
	"$(DEVENV10)" "Updater\Updater.sln" /build "Debug|x64"

"$(OUTPUT_DIR)\Win32.Release\ZRColaInstall$(LANG_OUT).exe" ::
	"$(DEVENV10)" "ZRColaUtils.sln" /build "Release|Win32"

"$(OUTPUT_DIR)\Win32.Debug\ZRColaInstall$(LANG_OUT).exe" ::
	"$(DEVENV10)" "ZRColaUtils.sln" /build "Debug|Win32"

"$(OUTPUT_DIR)\ZRCola.$(LANG).32.2.msi" ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) LANG=$(LANG) PLAT=Win32 CFG=Release
	cd "$(MAKEDIR)"

"$(OUTPUT_DIR)\ZRCola.$(LANG).32D.2.msi" ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) LANG=$(LANG) PLAT=Win32 CFG=Debug
	cd "$(MAKEDIR)"

"$(OUTPUT_DIR)\ZRCola.$(LANG).64.2.msi" ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) LANG=$(LANG) PLAT=x64 CFG=Release
	cd "$(MAKEDIR)"

"$(OUTPUT_DIR)\ZRCola.$(LANG).64D.2.msi" ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) LANG=$(LANG) PLAT=x64 CFG=Debug
	cd "$(MAKEDIR)"

"$(OUTPUT_DIR)\ZRCola$(LANG_OUT)32.msi" : \
	"$(OUTPUT_DIR)\ZRCola.$(LANG).32.2.msi" \
	"$(OUTPUT_DIR)\ZRCola32.inf" \
	"$(OUTPUT_DIR)\ZRCola32.cab"
	$(MAKE) /f "MSI\MSIBuild\CAB.mak" /$(MAKEFLAGS) MSIBUILD_ROOT="MSI\MSIBuild" MSIBUILD_TARGET_MSI=$@ MSIBUILD_SOURCE_MSI="$(OUTPUT_DIR)\ZRCola.$(LANG).32.2.msi" MSIBUILD_INF="$(OUTPUT_DIR)\ZRCola32.inf" MSIBUILD_CAB="$(OUTPUT_DIR)\ZRCola32.cab" MSIBUILD_PRODUCT_NAME="$(MSIBUILD_PRODUCT_NAME)"

"$(OUTPUT_DIR)\ZRCola$(LANG_OUT)32D.msi" : \
	"$(OUTPUT_DIR)\ZRCola.$(LANG).32D.2.msi" \
	"$(OUTPUT_DIR)\ZRCola32D.inf" \
	"$(OUTPUT_DIR)\ZRCola32D.cab"
	$(MAKE) /f "MSI\MSIBuild\CAB.mak" /$(MAKEFLAGS) MSIBUILD_ROOT="MSI\MSIBuild" MSIBUILD_TARGET_MSI=$@ MSIBUILD_SOURCE_MSI="$(OUTPUT_DIR)\ZRCola.$(LANG).32D.2.msi" MSIBUILD_INF="$(OUTPUT_DIR)\ZRCola32D.inf" MSIBUILD_CAB="$(OUTPUT_DIR)\ZRCola32D.cab" MSIBUILD_PRODUCT_NAME="$(MSIBUILD_PRODUCT_NAME)"

"$(OUTPUT_DIR)\ZRCola$(LANG_OUT)64.msi" : \
	"$(OUTPUT_DIR)\ZRCola.$(LANG).64.2.msi" \
	"$(OUTPUT_DIR)\ZRCola64.inf" \
	"$(OUTPUT_DIR)\ZRCola64.cab"
	$(MAKE) /f "MSI\MSIBuild\CAB.mak" /$(MAKEFLAGS) MSIBUILD_ROOT="MSI\MSIBuild" MSIBUILD_TARGET_MSI=$@ MSIBUILD_SOURCE_MSI="$(OUTPUT_DIR)\ZRCola.$(LANG).64.2.msi" MSIBUILD_INF="$(OUTPUT_DIR)\ZRCola64.inf" MSIBUILD_CAB="$(OUTPUT_DIR)\ZRCola64.cab" MSIBUILD_PRODUCT_NAME="$(MSIBUILD_PRODUCT_NAME)"

"$(OUTPUT_DIR)\ZRCola$(LANG_OUT)64D.msi" : \
	"$(OUTPUT_DIR)\ZRCola.$(LANG).64D.2.msi" \
	"$(OUTPUT_DIR)\ZRCola64D.inf" \
	"$(OUTPUT_DIR)\ZRCola64D.cab"
	$(MAKE) /f "MSI\MSIBuild\CAB.mak" /$(MAKEFLAGS) MSIBUILD_ROOT="MSI\MSIBuild" MSIBUILD_TARGET_MSI=$@ MSIBUILD_SOURCE_MSI="$(OUTPUT_DIR)\ZRCola.$(LANG).64D.2.msi" MSIBUILD_INF="$(OUTPUT_DIR)\ZRCola64D.inf" MSIBUILD_CAB="$(OUTPUT_DIR)\ZRCola64D.cab" MSIBUILD_PRODUCT_NAME="$(MSIBUILD_PRODUCT_NAME)"

!ENDIF
