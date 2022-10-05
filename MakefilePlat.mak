#    SPDX-License-Identifier: GPL-3.0-or-later
#    Copyright © 1991-2022 Amebis

######################################################################
# Setup
######################################################################

SetupCompile :: GenRSAKeypair
	msbuild.exe $(MSBUILDFLAGS) "ZRCola.sln"          /t:Build /p:Configuration=Release /p:Platform=$(PLAT)
	msbuild.exe $(MSBUILDFLAGS) "MSI\MSICA\MSICA.sln" /t:Build /p:Configuration=Release /p:Platform=$(PLAT)
	msbuild.exe $(MSBUILDFLAGS) "Updater\Updater.sln" /t:Build /p:Configuration=Release /p:Platform=$(PLAT)

SetupDebugCompile :: GenRSAKeypair
	msbuild.exe $(MSBUILDFLAGS) "ZRCola.sln"          /t:Build /p:Configuration=Debug   /p:Platform=$(PLAT)
	msbuild.exe $(MSBUILDFLAGS) "MSI\MSICA\MSICA.sln" /t:Build /p:Configuration=Debug   /p:Platform=$(PLAT)
	msbuild.exe $(MSBUILDFLAGS) "Updater\Updater.sln" /t:Build /p:Configuration=Debug   /p:Platform=$(PLAT)

GenRSAKeypair ::
	cd "Updater"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) GenRSAKeypair
	cd "$(MAKEDIR)"

######################################################################
# Publishing
######################################################################

"output\PDB.zip" : output\$(PLAT).Release\*.pdb


######################################################################
# Configuration Specific
######################################################################

CFG=Release
CFG_SUFFIX=
!INCLUDE "MakefilePlatCfg.mak"

CFG=Debug
CFG_SUFFIX=D
!INCLUDE "MakefilePlatCfg.mak"
