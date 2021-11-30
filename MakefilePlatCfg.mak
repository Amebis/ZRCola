#    SPDX-License-Identifier: GPL-3.0-or-later
#    Copyright © 1991-2021 Amebis

Clean ::
	msbuild.exe $(MSBUILDFLAGS) "ZRCola.sln"          /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT)
!IF "$(PLAT)" == "Win32"
	msbuild.exe $(MSBUILDFLAGS) "ZRColaUtils.sln"     /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT)
!ENDIF
	msbuild.exe $(MSBUILDFLAGS) "MSI\MSICA\MSICA.sln" /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT)
	msbuild.exe $(MSBUILDFLAGS) "Updater\Updater.sln" /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT)
	-if exist "$(OUTPUT_DIR)\$(PLAT).$(CFG).ddf" del /f /q "$(OUTPUT_DIR)\$(PLAT).$(CFG).ddf"
	-if exist "$(OUTPUT_DIR)\$(PLAT).$(CFG).cab" del /f /q "$(OUTPUT_DIR)\$(PLAT).$(CFG).cab"
	-if exist "$(OUTPUT_DIR)\$(PLAT).$(CFG).inf" del /f /q "$(OUTPUT_DIR)\$(PLAT).$(CFG).inf"
	-if exist "$(OUTPUT_DIR)\$(PLAT).$(CFG).rpt" del /f /q "$(OUTPUT_DIR)\$(PLAT).$(CFG).rpt"


!IFDEF HAS_VERSION

######################################################################
# 2nd Phase
# - The version is known, do the rest.
######################################################################

######################################################################
# Building
######################################################################

"$(OUTPUT_DIR)\$(PLAT).$(CFG).ddf" : \
#	"$(OUTPUT_DIR)\de_DE.$(PLAT).$(CFG).2.msi" \
	"$(OUTPUT_DIR)\en_US.$(PLAT).$(CFG).2.msi" \
	"$(OUTPUT_DIR)\ru_RU.$(PLAT).$(CFG).2.msi" \
	"$(OUTPUT_DIR)\sl_SI.$(PLAT).$(CFG).2.msi"
	-if exist "$(@:"=).tmp" del /f /q "$(@:"=).tmp"
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:MakeDDF //Nologo "$(@:"=).tmp" $** /O:"$(OUTPUT_DIR)\$(PLAT).$(CFG)" /C:LZX
	move /y "$(@:"=).tmp" $@ > NUL

"$(OUTPUT_DIR)\$(PLAT).$(CFG).cab" \
"$(OUTPUT_DIR)\$(PLAT).$(CFG).inf" \
"$(OUTPUT_DIR)\$(PLAT).$(CFG).rpt" : "$(OUTPUT_DIR)\$(PLAT).$(CFG).ddf"
	makecab.exe /F $**

!ENDIF