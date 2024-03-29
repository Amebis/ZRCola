#    SPDX-License-Identifier: GPL-3.0-or-later
#    Copyright © 1991-2022 Amebis

Clean ::
	msbuild.exe $(MSBUILDFLAGS) "ZRCola.sln"          /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT)
!IF "$(PLAT)" == "Win32"
	msbuild.exe $(MSBUILDFLAGS) "ZRColaUtils.sln"     /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT)
!ENDIF
	msbuild.exe $(MSBUILDFLAGS) "MSI\MSICA\MSICA.sln" /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT)
	msbuild.exe $(MSBUILDFLAGS) "Updater\Updater.sln" /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT)
	-if exist "output\$(PLAT).$(CFG).ddf" del /f /q "output\$(PLAT).$(CFG).ddf"
	-if exist "output\$(PLAT).$(CFG).cab" del /f /q "output\$(PLAT).$(CFG).cab"
	-if exist "output\$(PLAT).$(CFG).inf" del /f /q "output\$(PLAT).$(CFG).inf"
	-if exist "output\$(PLAT).$(CFG).rpt" del /f /q "output\$(PLAT).$(CFG).rpt"


!IFDEF HAS_VERSION

######################################################################
# 2nd Phase
# - The version is known, do the rest.
######################################################################

######################################################################
# Building
######################################################################

"output\$(PLAT).$(CFG).ddf" : \
#	"output\de_DE.$(PLAT).$(CFG).2.msi" \
	"output\en_US.$(PLAT).$(CFG).2.msi" \
	"output\ru_RU.$(PLAT).$(CFG).2.msi" \
	"output\sl_SI.$(PLAT).$(CFG).2.msi"
	-if exist "$(@:"=).tmp" del /f /q "$(@:"=).tmp"
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:MakeDDF //Nologo "$(@:"=).tmp" $** /O:"output\$(PLAT).$(CFG)" /C:LZX
	move /y "$(@:"=).tmp" $@ > NUL

"output\$(PLAT).$(CFG).cab" \
"output\$(PLAT).$(CFG).inf" \
"output\$(PLAT).$(CFG).rpt" : "output\$(PLAT).$(CFG).ddf"
	makecab.exe /F $**

!ENDIF