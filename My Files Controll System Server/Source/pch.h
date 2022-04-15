#pragma once

#include "Core/GarbageCollector.h"

#include <iostream>
#include <filesystem>
#include <vector>
#include <set>
#include <fstream>
#include <memory>

static auto GC = GarbageCollector::Instance();