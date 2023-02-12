#ifndef BITS_H_
#define BITS_H_
#include "cache.h"

int get_set(Cache *cache, address_type address);
int get_tag(Cache *cache, address_type address);
int get_byte(Cache *cache, address_type address);

#endif /* BITS_H_ */
