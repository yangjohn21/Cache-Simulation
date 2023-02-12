#include "bits.h"
#include "cache.h"

int get_set(Cache *cache, address_type address) {
  // TODO:
  //  Extract the set bits from a 32-bit address.
  //  
  int set_bit_num = cache->set_bits;
  address = address << (32 - set_bit_num - cache->block_bits);
  address = address >> (32 - set_bit_num);
  return address;
}

int get_tag(Cache *cache, address_type address) {
  // TODO:
  // Extract the tag bits from a 32-bit address.
  //
  address = address >> (cache->set_bits + cache->block_bits);
  return address;
}

int get_byte(Cache *cache, address_type address) {
  // TODO
  // Extract the block offset (byte index) bits from a 32-bit address.
  //  
  int not_offset = 32 - cache->block_bits;
  address = address << not_offset;
  address = address >> not_offset;
  return address;
}
