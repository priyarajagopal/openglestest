#pragma once

#include <string>
#include <vector>
#include <stdint.h>

namespace renderlib {

typedef uint32_t ElementId;
typedef std::vector<ElementId> ElementIdList;

enum ElementType { UNKNOWN, SPACES };

}