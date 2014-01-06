/*
 * sim_manager.cc
 */

#include <time.h>
#include <boost/thread.hpp>
#include <boost/tokenizer.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "trace.h"
#include "cache.h"
#include "lru.h"
#include "message_queue.h"

using namespace std;
using namespace boost;

static const int num_args = 0;
static const int default_num_sims = 4;
static const int default_cache_size = 1024;
static const int default_block_size = 4096;

int main(int argc, char *argv[]) {
  string buffer;
  vector<MQueue<Trace>*> mqueues;
  vector<Cache*> sims;
  vector<boost::thread*> workers;
  vector<struct timespec> start_times;

  if (argc < num_args) {
    printf("Usage: sim_manager <config file>\n");
    exit(EXIT_FAILURE);
  }

  const char *fname = argv[1];
  ifstream config(fname);
  if (!config.is_open()) {
    printf("Could not open file %s\n", fname);
    exit(EXIT_FAILURE);
  }

  while (getline(config, buffer)) {
    Cache *c;
    tokenizer<> tok(buffer);

    if (distance(tok.begin(), tok.end()) < 3)
      continue;

    MQueue<Trace> *mqueue = new MQueue<Trace>();

    // cache size 
    tokenizer<>::iterator it = tok.begin();
    int cache_size = atoi(it->c_str());
    it++;

    // block size
    int block_size = atoi(it->c_str());
    it++;

    // simulation type
    switch(atoi(it->c_str())) {
      case SIM_LRU:
        c = new LRU(mqueue, cache_size, block_size);
        break;
      default:
        c = new LRU(mqueue, cache_size, block_size);
        break;
    }

    mqueues.push_back(mqueue);
    sims.push_back(c);
    workers.push_back(new boost::thread(boost::bind(&Cache::simulate, c)));

    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);
    start_times.push_back(start);
   
  }


  
  

/*
  for (int i = 0; i < num_sims; i++) {
    MQueue<Trace> *mqueue = new MQueue<Trace>();
    mqueues.push_back(mqueue);
    LRU *lru = new LRU(mqueue, cache_size, block_size);
    sims.push_back(lru);
    workers.push_back(new boost::thread(boost::bind(&Cache::simulate, lru)));

    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);
    start_times.push_back(start);
  }
*/

  int num_sims = sims.size();
  int j = 0;
  while (true) {
    if (getline(cin, buffer) == NULL)
      break;

    Trace t(buffer);
    if (t.type == Trace::END)
      break;
    for (int i = 0; i < num_sims; i++) {
      mqueues[(i + j) % num_sims]->enqueue(t);
    }
    j++;
  }


  for (int i = 0; i < num_sims; i++) {
    mqueues[i]->enqueue(Trace::end());
  }

  for (int i = 0; i < num_sims; i++) {
    struct timespec end;
    workers[i]->join();
    clock_gettime(CLOCK_MONOTONIC, &end);

    printf("\n");
    printf("Simulation %d\n", i);
    printf("----------\n");
    printf("Type: %s\n", sims[i]->name().c_str());
    printf("Cache Size: %d blocks\n", sims[i]->get_cache_size());
    printf("Block Size: %d bytes\n", sims[i]->get_block_size());
 
    // simulation results
    printf("cache hits: %d\n", sims[i]->get_num_hits());
    printf("cache requests: %d\n", sims[i]->get_num_requests());
    printf("cache hit ratio: %f\n", (double) sims[i]->get_num_hits() / sims[i]->get_num_requests());

    // performance information
    int num_traces = sims[i]->get_num_traces();
    double elapsed = Trace::elapsed(start_times[i], end);
    printf("total latency: %10.9f sec\n", elapsed); 
    printf("average latency per trace: %10.9f sec\n", elapsed / num_traces);
  }
}

