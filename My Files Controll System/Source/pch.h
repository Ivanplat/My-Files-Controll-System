#pragma once

#include "Core/GarbageCollector.h"


#include <iostream>
#include <filesystem>
#include <thread>
#include <functional>
#include <map>
#include <set>
#include <vector>
#include <memory>
#include <cassert>
#include <fstream>


static GarbageCollector* GC = GarbageCollector::Instance();