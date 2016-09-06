#
#    Copyright 1991-2016 Amebis
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

######################################################################
# MSI General Package Information

MSIBUILD_PRODUCT_NAME=ZRCola
!IF "$(LANG)" == "Sl"
MSIBUILD_LANG_PO=sl_SI
MSIBUILD_LANGID=1060
MSIBUILD_CODEPAGE=1250
!ELSEIF "$(LANG)" == "Ru"
MSIBUILD_LANG_PO=ru_RU
MSIBUILD_LANGID=1049
MSIBUILD_CODEPAGE=1251
!ELSE
LANG=En
MSIBUILD_LANG_PO=en_US
MSIBUILD_LANGID=1033
MSIBUILD_CODEPAGE=1252
!ENDIF


######################################################################
# The base name of the output MSI file

MSIBUILD_TARGET=ZRCola$(LANG)
!IF "$(PLAT)" == "x64"
MSIBUILD_TARGET=$(MSIBUILD_TARGET)64
!ELSE
PLAT=Win32
MSIBUILD_TARGET=$(MSIBUILD_TARGET)32
!ENDIF
!IF "$(CFG)" == "Debug"
MSIBUILD_TARGET=$(MSIBUILD_TARGET)D
!ELSE
CFG=Release
!ENDIF


######################################################################
# Path to version file
# (relative from MSIBuild\Version folder)

MSIBUILD_VERSION_FILE=..\..\..\include\zrcola.h


######################################################################
# Working/output folder for building MSI file
# (relative from MSIBuild folder)

MSIBUILD_OUTPUT_DIR=..\..\output


######################################################################
# Path to MSIBuild folder
# (relative from ZRCola folder)

MSIBUILD_ROOT=..\MSIBuild


######################################################################
# GUID used to determine MSI upgrade logic

!IF "$(PLAT)" == "x64"
MSIBUILD_UPGRADE_GUID={12AC2F89-DDE4-4C3C-BDB8-FF3C329C4098}
!ELSE
MSIBUILD_UPGRADE_GUID={A7562A6B-ADF8-4775-BBDD-BC125AE58C03}
!ENDIF


######################################################################
# Minimum MSI version required to install this package

MSIBUILD_MSI_VERSION_MIN=200


######################################################################
# Length of ID and help fields in MSI tables (in characters)

MSIBUILD_LENGTH_ID=128
MSIBUILD_LENGTH_HELP=256


######################################################################
# Should MSIBuild compress files into CAB itself?

MSIBUILD_COMPRESS=1


######################################################################
# Prevent installation of 32-bit MSI on 64-bit Windows

MSIBUILD_HAS_X64=1
MSIBUILD_NO_WOW64=1


######################################################################
# Component and registry settings (platform dependant)

!IF "$(PLAT)" == "x64"
MSIBUILD_COMPONENT_ATTRIB_FILE=256
MSIBUILD_COMPONENT_ATTRIB_REGISTRY=260
MSIBUILD_REG32_RELOCATION=\Wow6432Node
!ELSE
MSIBUILD_COMPONENT_ATTRIB_FILE=0
MSIBUILD_COMPONENT_ATTRIB_REGISTRY=4
MSIBUILD_REG32_RELOCATION=
!ENDIF


######################################################################
# List of modules to compile and include in link

MSIBUILD_MODULES=\
	"$(MSIBUILD_ROOT)\Core\$(LANG).$(PLAT).$(CFG).msm" \
	"$(MSIBUILD_ROOT)\Version\$(LANG).$(PLAT).$(CFG).msm" \
	"$(MSIBUILD_ROOT)\UI\$(LANG).$(PLAT).$(CFG).msm" \
	"..\MSICA\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\Updater\Updater\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\Updater\UpdCheck\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\lib\MSICALib\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\lib\libZRCola\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\lib\libZRColaUI\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\lib\wxExtend\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\ZRCola\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"Main\$(LANG).$(PLAT).$(CFG).msm"


######################################################################
# List of precompiled MSM modules to include in link

MSIBUILD_MODULES_PRECOMPILED=\
!IF "$(CFG)" == "Debug"
!IF "$(PLAT)" == "x64"
	"..\MSM\wxMSW_wxbase30ud_vc100_x64.msm" \
	"..\MSM\wxMSW_wxbase30ud_net_vc100_x64.msm" \
	"..\MSM\wxMSW_wxbase30ud_xml_vc100_x64.msm" \
	"..\MSM\wxMSW_wxmsw30ud_adv_vc100_x64.msm" \
	"..\MSM\wxMSW_wxmsw30ud_aui_vc100_x64.msm" \
	"..\MSM\wxMSW_wxmsw30ud_core_vc100_x64.msm" \
	"..\MSM\Microsoft_VC100_DebugCRT_x64.msm"
!ELSE
	"..\MSM\wxMSW_wxbase30ud_vc100.msm" \
	"..\MSM\wxMSW_wxbase30ud_net_vc100.msm" \
	"..\MSM\wxMSW_wxbase30ud_xml_vc100.msm" \
	"..\MSM\wxMSW_wxmsw30ud_adv_vc100.msm" \
	"..\MSM\wxMSW_wxmsw30ud_aui_vc100.msm" \
	"..\MSM\wxMSW_wxmsw30ud_core_vc100.msm" \
	"..\MSM\Microsoft_VC100_DebugCRT_x86.msm"
!ENDIF
!ELSE
!IF "$(PLAT)" == "x64"
	"..\MSM\wxMSW_wxbase30u_vc100_x64.msm" \
	"..\MSM\wxMSW_wxbase30u_net_vc100_x64.msm" \
	"..\MSM\wxMSW_wxbase30u_xml_vc100_x64.msm" \
	"..\MSM\wxMSW_wxmsw30u_adv_vc100_x64.msm" \
	"..\MSM\wxMSW_wxmsw30u_aui_vc100_x64.msm" \
	"..\MSM\wxMSW_wxmsw30u_core_vc100_x64.msm" \
	"..\MSM\Microsoft_VC100_CRT_x64.msm"
!ELSE
	"..\MSM\wxMSW_wxbase30u_vc100.msm" \
	"..\MSM\wxMSW_wxbase30u_net_vc100.msm" \
	"..\MSM\wxMSW_wxbase30u_xml_vc100.msm" \
	"..\MSM\wxMSW_wxmsw30u_adv_vc100.msm" \
	"..\MSM\wxMSW_wxmsw30u_aui_vc100.msm" \
	"..\MSM\wxMSW_wxmsw30u_core_vc100.msm" \
	"..\MSM\Microsoft_VC100_CRT_x86.msm"
!ENDIF
!ENDIF


######################################################################
# wxExtend Module

WXEXTEND_BIN_DIR=ZRCOLABINDIR
WXEXTEND_LOC_DIR=ZRCOLALOCDIR


######################################################################
# Updater Module

UPDATER_BIN_DIR=ZRCOLABINDIR
UPDATER_FEATURE=
UPDATER_BIN_DIR=ZRCOLABINDIR
UPDATER_LOC_DIR=ZRCOLALOCDIR
