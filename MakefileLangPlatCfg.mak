#    SPDX-License-Identifier: GPL-3.0-or-later
#    Copyright © 1991-2021 Amebis

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
