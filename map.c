#include "map.h"

struct map_t {
 char** keys;
 char** values; 
 int size;
 int maxSize;
 int iterator;
} *Map;