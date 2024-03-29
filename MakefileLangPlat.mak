#    SPDX-License-Identifier: GPL-3.0-or-later
#    Copyright © 1991-2022 Amebis

!IFDEF HAS_VERSION

######################################################################
# 2nd Phase
# - The version is known, do the rest.
######################################################################

######################################################################
# Setup
######################################################################

SetupPackage ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) LANG=$(LANG) PLAT=$(PLAT) CFG=Release
	cd "$(MAKEDIR)"

SetupDebugPackage ::
	cd "MSI\ZRCola"
	$(MAKE) /f "Makefile" /$(MAKEFLAGS) LANG=$(LANG) PLAT=$(PLAT) CFG=Debug
	cd "$(MAKEDIR)"

!ENDIF


######################################################################
# Configuration Specific
######################################################################

CFG=Release
CFG_SUFFIX=
!INCLUDE "MakefileLangPlatCfg.mak"

CFG=Debug
CFG_SUFFIX=D
!INCLUDE "MakefileLangPlatCfg.mak"
