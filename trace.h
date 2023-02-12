#ifndef __TRACE_H
#define __TRACE_H

typedef unsigned int address_type;

typedef struct {
  char operation;
  address_type address;
  char size;
} TraceLine;

#endif
