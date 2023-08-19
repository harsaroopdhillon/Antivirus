#pragma once

#define DATABASE "database.db"
#define SCANFILEMAXLIMIT  1000 * 1024  // ; there are 1024 MB in 1 GB so calculate this to get total disk size and only take up less than 30%
#define YARARULES L"YaraRules.yar"

#include <Windows.h>


#define CONFIG_REREAD_INTERVAL (5 * 1000)
#define CONFIG_APPNAME         (L"AntivirusService")

