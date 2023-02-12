#include "lru.h"
#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

void lru_init_queue(Set *set)
{
  LRUNode *s = NULL;
  LRUNode **pp = &s; // place to chain in the next node
  for (int i = 0; i < set->line_count; i++)
  {
    Line *line = &set->lines[i];
    LRUNode *node = (LRUNode *)(malloc(sizeof(LRUNode)));
    node->line = line;
    node->next = NULL;
    (*pp) = node;
    pp = &((*pp)->next);
  }
  set->lru_queue = s;
}

void lru_init(Cache *cache)
{
  Set *sets = cache->sets;
  for (int i = 0; i < cache->set_count; i++)
  {
    lru_init_queue(&sets[i]);
  }
}

void lru_destroy(Cache *cache)
{
  Set *sets = cache->sets;
  for (int i = 0; i < cache->set_count; i++)
  {
    LRUNode *p = sets[i].lru_queue;
    LRUNode *n = p;
    while (p != NULL)
    {
      p = p->next;
      free(n);
      n = p;
    }
    sets[i].lru_queue = NULL;
  }
}

void lru_fetch(Set *set, unsigned int tag, LRUResult *result)
{
  // TODO:
  // Implement the LRU algorithm to determine which line in
  // the cache should be accessed.
  //
  LRUNode *currNode = set->lru_queue;
  if (currNode == NULL)
  {
    result->access = 1;
    printf("help i have no idea what to do");
  }
  if ((currNode->line->tag == tag) && (currNode->line->valid !=0))
  {
    result->line = currNode->line;
    result->access = 0;
    return;
  }
  int curr_set_count = 1;
  while (currNode->next != NULL)
  {
    curr_set_count += 1;
    if ((currNode->next->line->tag = tag) && (currNode->next->line->valid == 1))
    {
      result->line = currNode->next->line;
      result->access = 0;

      LRUNode *temp = currNode->next;
      currNode->next = currNode->next->next;
      temp->next = set->lru_queue;
      set->lru_queue = temp;
      return;
    }
    currNode = currNode->next;
  }
  if (curr_set_count < set->line_count)
  {
    result->access = COLD_MISS;
    currNode = set->lru_queue;
    if (currNode->line->valid == 0)
    {
      currNode->line->tag = tag;
      currNode->line->valid = 1;
      result->line = currNode->line;
      return;
    }
    while (currNode->next != NULL)
    {
      if (currNode->next->line->valid == 0)
      {
        currNode->next->line->tag = tag;
        currNode->next->line->valid = 1;
        result->line = currNode->next->line;
        LRUNode *temp = currNode->next;
        currNode->next = currNode->next->next;
        temp->next = set->lru_queue;
        set->lru_queue = temp;
        return;
      }
      currNode = currNode->next;
    }
    return;
  }
  else
  {
    result->access = CONFLICT_MISS;

    LRUNode *newNode = set->lru_queue;
    if(newNode->next == NULL){
      newNode->line->tag = tag;
      newNode->line->valid = 1;
      result->line = newNode->line;
      return;
    }
    while(newNode->next->next!=NULL){
      newNode = newNode->next;
    }
    newNode->next->line->tag = tag;
    newNode->next->line->valid = 1;
    newNode->next->next = set->lru_queue;
    set->lru_queue = newNode->next;
    newNode->next = NULL;
    result->line = set->lru_queue->line;
    return;
    /*
    currNode->line->tag = tag;
    currNode->line->valid = 1;
    result->line = currNode->line;
    LRUNode *temp = currNode;
    currNode = set->lru_queue;
    while (currNode->next != NULL)
    {
      if (currNode->next == temp)
      {
        temp->next = set->lru_queue;
        set->lru_queue = temp;
        currNode->next = NULL;
        return;
      }
      currNode = currNode->next;
    }
    printf("shouldnt have reached here");
    return;
  }*/
  }
}
