#include "map.h"
#define INITIAL_SIZE 20
#define DATA_ITEM_NOT_EXIST -1

typedef struct Element_t {
    char* keys;
    char* values;
} *Element;

typedef struct Map_t {
    Element data;
    int size;
    int maxSize;
    int iterator;
};

static int dataFind(Map map, const char* toFind)
{
    for (int i = 0; i<mapGetSize(map); i++){
        if(strcmp(map->data[i].keys, toFind)==0) {
            return i;
        }
    }
    return DATA_ITEM_NOT_EXIST;
}

static char* stringCopy(const char* toCopy)
{
    char* out = malloc(sizeof(char)*strlen(toCopy)+1);
    if(!out)
    {
        return NULL;
    }
    return (strcpy(out,toCopy));
}

Map mapCreate() {
    Map map = malloc(sizeof(*map));
    if (map == NULL) {
    return NULL;
    }

    map->data = malloc(INITIAL_SIZE * sizeof(*map->data));
    if (map->data == NULL) {
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
    free(map->data);
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
    for (int i = 0; i<mapGetSize(map); i++) {
        if (mapPut(newMap, map->data[i].keys, map->data[i].values) != MAP_SUCCESS) {
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
    int key_to_find = dataFind(map, key);
        if (key_to_find != DATA_ITEM_NOT_EXIST) {
            map->iterator = key_to_find;
            return true;
        }
    return false;

}

// remove all MAP_FOREACH

MapResult mapPut(Map map, const char* key, const char* data)
{
    if (map == NULL || key == NULL || data == NULL) { // map == NULL?
      return MAP_NULL_ARGUMENT;
    }
    if (mapContains(map,key)) {
        free(map->data[map->iterator].values);
        map->data[map->iterator].values = stringCopy(data);
        if (!map->data[map->iterator].values) {
            return MAP_OUT_OF_MEMORY; 
        }
        return MAP_SUCCESS;
    }
    if (mapGetSize(map) == map->maxSize) {
        if (mapExpand(map) == MAP_OUT_OF_MEMORY) { // need to implement expand
            return MAP_OUT_OF_MEMORY;
        }
    }
    map->size++;
    Element newData = malloc(sizeof(struct Element_t));
    if (!newData){
        return MAP_OUT_OF_MEMORY;
    }
    newData->keys = stringCopy(key);
    if (!newData->keys) {
        
        return MAP_OUT_OF_MEMORY;
    }
    newData->values = stringCopy(data);
    if (!newData->values) {
        return MAP_OUT_OF_MEMORY; 
    }
    map->data[mapGetSize(map) - 1] = *newData;
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
    int toRemove = dataFind(map, key);
        if (toRemove != DATA_ITEM_NOT_EXIST) {
            free(map->data[toRemove].keys);
            free(map->data[toRemove].values);
            free(map->data[toRemove]);
            map->data[toRemove] = map->data[mapGetSize(map) - 1];
            map->size--;
            return MAP_SUCCESS;
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
        mapRemove(map, mapGetFirst(map));
    }
    return MAP_SUCCESS;
    
}