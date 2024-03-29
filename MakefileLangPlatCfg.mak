#    SPDX-License-Identifier: GPL-3.0-or-later
#    Copyright © 1991-2022 Amebis

Clean ::
	-if exist "output\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi" del /f /q "output\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi"

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
	"output\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi"
!ENDIF

!IF "$(CFG)" == "Debug"
SetupDebug :: \
	"output\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi"
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
"$(PUBLISH_DIR)\$(MSIBUILD_VERSION_STR)\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi" : "output\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi"
	copy /y $** $@ > NUL


######################################################################
# Building
######################################################################

"output\ZRCola$(LANG_SUFFIX)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi" : \
	"output\$(LANG).$(PLAT).$(CFG).2.msi" \
	"output\$(PLAT).$(CFG).inf" \
	"output\$(PLAT).$(CFG).cab"
	-if exist $@ del /f /q $@
	copy /y "output\$(LANG).$(PLAT).$(CFG).2.msi" "$(@:"=).tmp" > NUL
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:SetCAB //Nologo "$(@:"=).tmp" "output\$(PLAT).$(CFG).inf" /E
	"$(WINDOWSSDKVERBINPATH)x86\msiinfo.exe" "$(@:"=).tmp" /nologo /U 4
!IFDEF MANIFESTCERTIFICATETHUMBPRINT
	signtool.exe sign /sha1 "$(MANIFESTCERTIFICATETHUMBPRINT)" /fd sha256 /tr "$(MANIFESTTIMESTAMPRFC3161URL)" /td sha256 /d "$(MSIBUILD_PRODUCT_NAME)" /q "$(@:"=).tmp"
!ENDIF
	attrib.exe +r "$(@:"=).tmp"
	move /y "$(@:"=).tmp" $@ > NUL

!ENDIF
