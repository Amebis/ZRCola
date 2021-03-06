#
#    Copyright © 1991-2021 Amebis
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

Clean ::
	-if exist "$(OUTPUT_DIR)\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi" del /f /q "$(OUTPUT_DIR)\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi"

!IFDEF HAS_VERSION

######################################################################
# 2nd Phase
# - The version is known, do the rest.
######################################################################

######################################################################
# Setup
######################################################################

!IF "$(CFG)" == "Release"
Setup :: \
	"$(OUTPUT_DIR)\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi"
!ENDIF

!IF "$(CFG)" == "Debug"
SetupDebug :: \
	"$(OUTPUT_DIR)\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi"
!ENDIF


######################################################################
# Publishing
######################################################################

!IF "$(CFG)" == "Release"
PublishPre :: \
	"$(PUBLISH_DIR)\$(MSIBUILD_VERSION_STR)" \
	"$(PUBLISH_DIR)\$(MSIBUILD_VERSION_STR)\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi"

Publish :: \
	"$(PUBLISH_DIR)\_latest" \
	"$(PUBLISH_DIR)\_latest\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi" \
	"$(PUBLISH_DIR)\ZRColaInstall$(LANG_SUFFIX).exe"
!ENDIF


######################################################################
# File copy
######################################################################

"$(PUBLISH_DIR)\_latest\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi" \
"$(PUBLISH_DIR)\$(MSIBUILD_VERSION_STR)\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi" : "$(OUTPUT_DIR)\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi"
	copy /y $** $@ > NUL


######################################################################
# Building
######################################################################

"$(OUTPUT_DIR)\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi" : \
	"$(OUTPUT_DIR)\$(LANG).$(PLAT).$(CFG).2.msi" \
	"$(OUTPUT_DIR)\$(PLAT).$(CFG).inf" \
	"$(OUTPUT_DIR)\$(PLAT).$(CFG).cab"
	$(MAKE) /f "MSI\MSIBuild\CAB.mak" /$(MAKEFLAGS) MSIBUILD_ROOT="MSI\MSIBuild" MSIBUILD_TARGET_MSI=$@ MSIBUILD_SOURCE_MSI="$(OUTPUT_DIR)\$(LANG).$(PLAT).$(CFG).2.msi" MSIBUILD_INF="$(OUTPUT_DIR)\$(PLAT).$(CFG).inf" MSIBUILD_CAB="$(OUTPUT_DIR)\$(PLAT).$(CFG).cab" MSIBUILD_PRODUCT_NAME="$(MSIBUILD_PRODUCT_NAME)"

!ENDIF
