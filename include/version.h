/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2021 Amebis
*/

#pragma once

//
// Product version as a single DWORD
// Note: Used for version comparison within C/C++ code.
//
#define PRODUCT_VERSION          0x02050400

//
// Product version by components
// Note: Resource Compiler has limited preprocessing capability,
// thus we need to specify major, minor and other version components
// separately.
//
#define PRODUCT_VERSION_MAJ      2
#define PRODUCT_VERSION_MIN      5
#define PRODUCT_VERSION_REV      4
#define PRODUCT_VERSION_BUILD    0

//
// Human readable product version and build year for UI
//
#define PRODUCT_VERSION_STR      "2.5.4"
#define PRODUCT_BUILD_YEAR_STR   "2021"

//
// Numerical version presentation for ProductVersion propery in
// MSI packages (syntax: N.N[.N[.N]])
//
#define PRODUCT_VERSION_INST     "2.5.4"

//
// The product code for ProductCode property in MSI packages
// Replace with new on every version change, regardless how minor it is.
//
#define PRODUCT_VERSION_GUID     "{EBF841F9-2BFB-444C-AC5E-874586350EFD}"

//
// The product vendor and application name for configuration keeping.
//
#define PRODUCT_CFG_VENDOR       "Amebis"
#define PRODUCT_CFG_APPLICATION  "ZRCola"
