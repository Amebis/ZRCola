#include "zrcola.h"

//
// Update log file
//
#define UPDATER_LOG_FILE            "ZRCola-Update.log"


//
// Update server(s)
// Zero delimited list of update servers
//
#define UPDATER_HTTP_SERVER         "zrcola.zrc-sazu.si\0zrcola-2.amebis.si\0"

//
// Update server port
//
#define UPDATER_HTTP_PORT           80

//
// Update server catalog location
//
#define UPDATER_HTTP_PATH           "/UpdateRepo/Catalog-1.xml"


//
// The product vendor and application name for configuration keeping.
//
#define UPDATER_CFG_VENDOR          "Amebis"
#define UPDATER_CFG_APPLICATION     "ZRCola"


//
// Product version as a single DWORD
// Note: Used for version comparison within C/C++ code.
//
#define UPDATER_PRODUCT_VERSION     ZRCOLA_VERSION

//
// Human readable product version and build year for UI
//
#define UPDATER_PRODUCT_VERSION_STR ZRCOLA_VERSION_STR
