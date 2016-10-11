#include "version.h"

//
// Update server(s)
// Zero delimited list of update servers
//
#define UPDATER_HTTP_SERVER         "www.amebis.si\0"

//
// Update server port
//
#define UPDATER_HTTP_PORT           80

//
// Update server catalog location
//
#define UPDATER_HTTP_PATH           "/prenos/ZRCola/catalog-0000.xml"


//
// The product vendor and application name for configuration keeping.
//
#define UPDATER_CFG_VENDOR          "Amebis"
#define UPDATER_CFG_APPLICATION     "ZRCola"


//
// Product version as a single DWORD
// Note: Used for version comparison within C/C++ code.
//
#define UPDATER_PRODUCT_VERSION     PRODUCT_VERSION

//
// Human readable product version and build year for UI
//
#define UPDATER_PRODUCT_VERSION_STR PRODUCT_VERSION_STR
