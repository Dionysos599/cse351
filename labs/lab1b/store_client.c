/*                                                                              
 * CSE 351 Lab 1b (Manipulating Bits in C)
 *
 * Name(s): Hinyeung Lam, Yinuo Yang  
 * NetID(s): hlam3, yinuoy
 *
 * This is a file for managing a store of various aisles, represented by an
 * array of 64-bit integers. See aisle_manager.c for details on the aisle
 * layout and descriptions of the aisle functions that you may call here.
 *
 * Written by Porter Jones (pbjones@cs.washington.edu)
 */

#include <stddef.h>  // To be able to use NULL
#include "aisle_manager.h"
#include "store_client.h"
#include "store_util.h"

// Number of aisles in the store
#define NUM_AISLES 10

// Number of sections per aisle
#define SECTIONS_PER_AISLE 4

// Number of items in the stockroom (2^6 different id combinations)
#define NUM_ITEMS 64

// Global array of aisles in this store. Each unsigned long in the array
// represents one aisle.
unsigned long aisles[NUM_AISLES];

// Array used to stock items that can be used for later. The index of the array
// corresponds to the item id and the value at an index indicates how many of
// that particular item are in the stockroom.
int stockroom[NUM_ITEMS];


/* Starting from the first aisle, refill as many sections as possible using
 * items from the stockroom. A section can only be filled with items that match
 * the section's item id. Prioritizes and fills sections with lower addresses
 * first. Sections with lower addresses should be fully filled (if possible)
 * before moving onto the next section.
 */
void refill_from_stockroom() {
  unsigned short id;
  unsigned short diff;
  for (int i = 0; i < NUM_AISLES; i++) {
    for (int j = 0; j < SECTIONS_PER_AISLE; j++) {
      id = get_id(&aisles[i], j);
      diff = num_items(&aisles[i], j);
      add_items(&aisles[i], j, stockroom[id]);
      diff = num_items(&aisles[i], j) - diff;
      stockroom[id] -= diff;
    }
  }
}

/* Remove at most num items from sections with the given item id, starting with
 * sections with lower addresses, and return the total number of items removed.
 * Multiple sections can store items of the same item id. If there are not
 * enough items with the given item id in the aisles, first remove all the
 * items from the aisles possible and then use items in the stockroom of the
 * given item id to finish fulfilling an order. If the stockroom runs out of
 * items, you should remove as many items as possible.
 */
int fulfill_order(unsigned short id, int num) {
  int removed_total = 0;
  for (int i = 0; i < NUM_AISLES; i++) {
    for (int j = 0; j < SECTIONS_PER_AISLE; j++) {
      if (get_id(&aisles[i], j) == id && num > 0) {
        int current_num = num_items(&aisles[i], j);
        remove_items(&aisles[i], j, num);
        int removed_num = current_num - num_items(&aisles[i], j); 
        removed_total += removed_num;
        num -= removed_num; 
      }
    }
  }
  if (num > 0) {
    if (stockroom[id] >= num) {
      stockroom[id] = stockroom[id] - num;
      removed_total += num;
    } else {
      removed_total += stockroom[id];
      stockroom[id] = 0;
    }
  }
  return removed_total;
}
                                                                                
/* Return a pointer to the first section in the aisles with the given item id
 * that has no items in it or NULL if no such section exists. Only consider
 * items stored in sections in the aisles (i.e., ignore anything in the
 * stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short* empty_section_with_id(unsigned short id) {
  for (int i = 0; i < NUM_AISLES; i++) {
    for (int j = 0; j < SECTIONS_PER_AISLE; j++) {
      if (get_id(&aisles[i], j) == id && num_items(&aisles[i], j) == 0) {
        return (unsigned short*)&aisles[i] + j;
      }
    }
  }
  return NULL;
}

/* Return a pointer to the section with the most items in the store. Only
 * consider items stored in sections in the aisles (i.e., ignore anything in
 * the stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short* section_with_most_items() {
  int max;
  int max_aisle;
  int max_sec;
  max = 0;
  max_aisle = 0;
  max_sec = 0;
  for (int i = 0; i < NUM_AISLES; i++) {
    for (int j = 0; j < SECTIONS_PER_AISLE; j++) {
      int current = num_items(&aisles[i], j);
      if (max < current) {
        max = current;
        max_aisle = i;
        max_sec = j;
      }
    }
  }
  return (unsigned short*)&aisles[max_aisle] + max_sec;
}
