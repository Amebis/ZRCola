#    SPDX-License-Identifier: GPL-3.0-or-later
#    Copyright © 1991-2021 Amebis

Clean ::
	-if exist "$(OUTPUT_DIR)\locale\$(LANG)\wxstd.mo"        del /f /q "$(OUTPUT_DIR)\locale\$(LANG)\wxstd.mo"
	-if exist "$(OUTPUT_DIR)\locale\$(LANG)\ZRCola-zrcdb.mo" del /f /q "$(OUTPUT_DIR)\locale\$(LANG)\ZRCola-zrcdb.mo"

!IFDEF HAS_VERSION

######################################################################
# 2nd Phase
# - The version is known, do the rest.
######################################################################

######################################################################
# Localization
######################################################################

Localization :: \
	"$(OUTPUT_DIR)\locale\$(LANG)" \
	"$(OUTPUT_DIR)\locale\$(LANG)\wxstd.mo" \
	"$(OUTPUT_DIR)\locale\$(LANG)\ZRCola-zrcdb.mo"


######################################################################
# Folder creation
######################################################################

"$(OUTPUT_DIR)\locale\$(LANG)" :
	if not exist $@ md $@

"$(OUTPUT_DIR)\locale\$(LANG)" : "$(OUTPUT_DIR)\locale"


######################################################################
# File copy
######################################################################

"$(PUBLISH_DIR)\ZRColaInstall$(LANG_SUFFIX).exe" : "$(OUTPUT_DIR)\Win32.Release\ZRColaInstall$(LANG_SUFFIX).exe"
	copy /y $** $@ > NUL


######################################################################
# Building
######################################################################

"$(OUTPUT_DIR)\locale\$(LANG)\wxstd.mo" : \
!IF EXISTS("$(WXWIN)\locale\$(LANG).po")
	"$(WXWIN)\locale\$(LANG).po"
!ELSEIF EXISTS("$(WXWIN)\locale\$(LANG_BASE).po")
	"$(WXWIN)\locale\$(LANG_BASE).po"
!ELSE
	"$(WXWIN)\locale\wxstd.pot"
!ENDIF
	msgfmt.exe --output-file=$@ --alignment=1 --endianness=little $**

"$(OUTPUT_DIR)\locale\$(LANG)\ZRCola-zrcdb.mo" : \
!IF EXISTS("$(OUTPUT_DIR)\locale\$(LANG)\ZRCola-zrcdb.po")
	"$(OUTPUT_DIR)\locale\$(LANG)\ZRCola-zrcdb.po"
!ELSE
	"$(OUTPUT_DIR)\locale\ZRCola-zrcdb.pot"
!ENDIF
	msgfmt.exe --output-file=$@ --alignment=1 --endianness=little $**

"$(OUTPUT_DIR)\Win32.Release\ZRColaInstall$(LANG_SUFFIX).exe" ::
	msbuild.exe $(MSBUILDFLAGS) "ZRColaInstall\ZRColaInstall.vcxproj" /t:Build /p:Configuration=Release;Language=$(LANG_SUFFIX) /p:Platform=Win32

!ENDIF


######################################################################
# Platform Specific
######################################################################

PLAT=Win32
PLAT_SUFFIX=-x86
!INCLUDE "MakefileLangPlat.mak"

PLAT=x64
PLAT_SUFFIX=-amd64
!INCLUDE "MakefileLangPlat.mak"

PLAT=ARM64
PLAT_SUFFIX=-arm64
!INCLUDE "MakefileLangPlat.mak"
