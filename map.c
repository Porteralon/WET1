#include "map.h"
#define INITIAL_SIZE 20


typedef struct Map_t {
 char** keys;
 char** values; 
 int size;
 int maxSize;
 int iterator;
};


Map mapCreate() {
    Map map = malloc(sizeof(*map));
    if (map == NULL) {
    return NULL;
    }

    map->keys = malloc(INITIAL_SIZE * sizeof(char*));
    if (map->keys == NULL) {
        free(map);
        return NULL;
    }
    map->values = malloc(INITIAL_SIZE * sizeof(char*));
    if (map->values == NULL) {
        free(map->keys);
        free(map);
        return NULL;
    }
    map->size=0;
    map->maxSize = INITIAL_SIZE;
    return map;
}

void mapDestroy(Map map)
{
    if(map == NULL)
    {
        return;
    }
    mapClear(map);
    free(map->keys);
    free(map->values);
    free(map);
}

Map mapCopy(Map map)
{
    if (map==NULL) {
        return NULL;
    }
    Map newMap = mapCreate();
    if(newMap == NULL) {
        return NULL;
    }
    MAP_FOREACH(i, map) {
        if (mapPut(newMap, i, map->values[map->iterator]) != MAP_SUCCESS) {
            mapDestroy(newMap);
            return NULL;
        }
    }
    return newMap;
}


int mapGetSize(Map map)
{
    if (map == NULL)
    {
        return -1;
    }
    return map->size;
}

bool mapContains(Map map, const char* key)
{
    if (map == NULL || key == NULL) {
        return false;
    }
    MAP_FOREACH(i, map) {
        if (!strcmp(key, i)) {
            return true;
        }
    }
    return false;

}

// remove all MAP_FOREACH

MapResult mapPut(Map map, const char* key, const char* data)
{
    if (map == NULL || key == NULL || data == NULL) { // map == NULL?
      return MAP_NULL_ARGUMENT;
    }
    if (mapGetSize(map) == map->maxSize) {
        if (mapExpand(map) == MAP_OUT_OF_MEMORY) { // need to implement expand
            return MAP_OUT_OF_MEMORY;
        }
    }
    MAP_FOREACH (i, map) {
        if (!strcmp(key, i)) {
            free(map->values[map->iterator]);
            map->values[map->iterator] = malloc(sizeof(char)*(strlen(data)+1)); // add generic funct
            if (map->values[map->iterator]) {
                return NULL; // should check what return
            }
            strcpy(map->values[map->iterator], data);
            return MAP_SUCCESS;
        }
    }
    map->size++;
    map->keys[mapGetSize(map)-1] = malloc(sizeof(char)*(strlen(key)+1));
    if (map->keys[mapGetSize(map)-1]) {
        return NULL; // should check what return
    }
    strcpy(map->keys[mapGetSize(map)-1], key);
    map->values[mapGetSize(map)-1] = malloc(sizeof(char)*(strlen(data)+1));
    if (map->values[mapGetSize(map)-1]) {
        return NULL; // should check what return
    }
    strcpy(map->values[mapGetSize(map)-1], data);
    return MAP_SUCCESS;

}

char* mapGet(Map map, const char* key)
{
    if(map == NULL)
    {
        return NULL;
    }
    for (int i = 0; i<mapGetSize(map); i++) { // Check about MAP_FOREACH
        if (strcmp(map->values[i], key) == 0) {
            return map->values[i];
        }
    }
    return NULL;
}



MapResult mapRemove(Map map, const char* key)
{
    if (key == NULL || map == NULL) {
      return MAP_NULL_ARGUMENT;
    }
    MAP_FOREACH(i, map) {
        if (strcmp(i, key) == 0) {
            free(map->keys[map->iterator]);
            free(map->values[map->iterator]);
            map->keys[map->iterator] = map->keys[mapGetSize(map) - 1];
            map->values[map->iterator] = map->values[mapGetSize(map) - 1];
            map->keys[mapGetSize(map)-1] = NULL;
            map->values[mapGetSize(map)-1] = NULL;
            map->size--;
            return MAP_SUCCESS;
        }
    }
    return MAP_ITEM_DOES_NOT_EXIST;

}

char* mapGetFirst(Map map)
{
    if(map == NULL || mapGetSize(map)==0)
    {
        return NULL;
    }
    map->iterator = 0;
    return (map->keys[map->iterator]);
}


char* mapGetNext(Map map)
{
    if(map == NULL || map->iterator>=mapGetSize(map)-1)
    {
        return NULL;
    }
    map->iterator++;
    return (map->keys[map->iterator]);
}

MapResult mapClear(Map map) {
    if (map == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    while (mapGetsize(map) > 0)
    {
        mapRemove(map, map->keys[mapGetSize(map)-1]);
    }
    return MAP_SUCCESS;
    
}