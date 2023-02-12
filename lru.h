#ifndef __LRU_H
#define __LRU_H
#include "cache.h"

void lru_init(Cache *cache);
void lru_destroy(Cache *cache);
void lru_fetch(Set *set, unsigned int tag, LRUResult *result);

#endif
