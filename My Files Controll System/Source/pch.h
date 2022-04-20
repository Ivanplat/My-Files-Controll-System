#pragma once

#include "Modules/IBaseModule.h"
#include "Modules/Client/public/ClientModule.h"
#include "Modules/CommandParser/public/CommandParserModule.h"
#include "Modules/FileControll/public/FileControllModule.h"
#include "Modules/FileControll/public/XMLModule.h"
#include "Modules/System/public/SystemModule.h"
#include "Core/GarbageCollector.h"
#include "Core/Logger.h"


#include <iostream>
#include <filesystem>
#include <thread>
#include <functional>
#include <map>
#include <vector>
#include <memory>
#include <cassert>
#include <fstream>
#include <sstream>


static GarbageCollector* GC = GarbageCollector::Instance();