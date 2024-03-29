#    SPDX-License-Identifier: GPL-3.0-or-later
#    Copyright © 1991-2022 Amebis

Clean ::
	-if exist "output\locale\$(LANG)\wxstd.mo"        del /f /q "output\locale\$(LANG)\wxstd.mo"
	-if exist "output\locale\$(LANG)\ZRCola-zrcdb.mo" del /f /q "output\locale\$(LANG)\ZRCola-zrcdb.mo"

!IFDEF HAS_VERSION

######################################################################
# 2nd Phase
# - The version is known, do the rest.
######################################################################

######################################################################
# Localization
######################################################################

Localization :: \
	"output\locale\$(LANG)" \
	"output\locale\$(LANG)\wxstd.mo" \
	"output\locale\$(LANG)\ZRCola-zrcdb.mo"


######################################################################
# Folder creation
######################################################################

"output\locale\$(LANG)" :
	if not exist $@ md $@

"output\locale\$(LANG)" : "output\locale"


######################################################################
# File copy
######################################################################

"$(PUBLISH_DIR)\ZRColaInstall$(LANG_SUFFIX).exe" : "output\Win32.Release\ZRColaInstall$(LANG_SUFFIX).exe"
	copy /y $** $@ > NUL


######################################################################
# Building
######################################################################

"output\locale\$(LANG)\wxstd.mo" : \
!IF EXISTS("$(WXWIN)\locale\$(LANG).po")
	"$(WXWIN)\locale\$(LANG).po"
!ELSEIF EXISTS("$(WXWIN)\locale\$(LANG_BASE).po")
	"$(WXWIN)\locale\$(LANG_BASE).po"
!ELSE
	"$(WXWIN)\locale\wxstd.pot"
!ENDIF
	msgfmt.exe --output-file=$@ --alignment=1 --endianness=little $**

"output\locale\$(LANG)\ZRCola-zrcdb.mo" : \
!IF EXISTS("output\locale\$(LANG)\ZRCola-zrcdb.po")
	"output\locale\$(LANG)\ZRCola-zrcdb.po"
!ELSE
	"output\locale\ZRCola-zrcdb.pot"
!ENDIF
	msgfmt.exe --output-file=$@ --alignment=1 --endianness=little $**

"output\Win32.Release\ZRColaInstall$(LANG_SUFFIX).exe" ::
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
