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
    while (mapGetSize(map)>0) {
        mapRemove(map, mapGetFirst(map));
    }
    
    free(map->keys);
    free(map->values);
    free(map);
}

Map mapCopy(Map map)
{
    if (map==NULL)
    {
        return NULL;
    }
    Map newMap = mapCreate();
    if(newMap == NULL)
    {
        return NULL;
    }
    for (int i=0;i<mapGetSize(map);i++)
    {
        if (!mapPut(newMap, map->keys[i], map->values[i]))
        {
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
    for(map->iterator = 0; map->iterator<mapGetSize(map); map->iterator++) {
        if (strcmp(map->keys[map->iterator], key)==0) {
            return true;
        }
    }
return false;

}


MapResult mapPut(Map map, const char* key, const char* data)
{
    assert(map != NULL);
    if (key == NULL || data == NULL) {
      return MAP_NULL_ARGUMENT;
    }
    if (mapGetSize(map) == map->maxSize) {
        if (mapExpand(map) == MAP_OUT_OF_MEMORY) { // need to implement expand
            return MAP_OUT_OF_MEMORY;
        }
    }
    if (mapContains(map, key)) {
        map->values[map->iterator] = data;
        return MAP_SUCCESS;
    }
    map->size++;
    strcpy(map->keys[mapGetSize(map)], key);
    strcpy(map->values[mapGetSize(map)], data);
    return MAP_SUCCESS;


}

char* mapGet(Map map, const char* key)
{
    if(map == NULL)
    {
        return NULL;
    }
    return (mapContains(map,key) ? map->values[map->iterator] : NULL);    
}



MapResult mapRemove(Map map, const char* key)
{
    if (key == NULL || map == NULL) {
      return MAP_NULL_ARGUMENT;
    }
    if (!mapContains(map, key)) {
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    
    map->keys[map->iterator] = map->keys[mapGetSize(map) - 1];
    map->values[map->iterator] = map->values[mapGetSize(map) - 1];
    free(map->keys[mapGetSize(map)-1]);
    free(map->values[mapGetSize(map)-1]);
    map->size--;
    return MAP_SUCCESS;
}

char* mapGetFirst(Map map)
{
    if(map == NULL || mapGetSize(map)==0)
    {
        return NULL;
    }
    return (map->keys[0]);
}

/**
*	mapGetNext: Advances the map iterator to the next key element and returns it.
*	The next key element is any key element not previously returned by the iterator.
* @param map - The map for which to advance the iterator
* @return
* 	NULL if reached the end of the map, or the iterator is at an invalid state
* 	or a NULL sent as argument
* 	The next key element on the map in case of success
*/
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
        return NULL;
    }
    while (mapGetsize(map) > 0)
    {
        mapRemove(map, map->keys[mapGetSize(map)-1]);
    }
    return MAP_SUCCESS;
    
}