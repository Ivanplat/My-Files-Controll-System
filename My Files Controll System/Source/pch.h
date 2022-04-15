#pragma once

#include "Core/GarbageCollector.h"


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