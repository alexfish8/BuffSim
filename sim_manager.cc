/*
 * sim_manager.c
 *
 * Author: Alex Fish
 *
 * The program listens for simulation requests from client processes,
 * and manages the simulations accordingly. It runs the simulations on
 * streams of I/O traces.
 *
 */

#include "simulation.h"
#include "lru.h"


static const int cache_size = 1024;


int main () {

  LRU *lru_sim = new LRU(cache_size);
  return 0;
}




