/*
 * sim_manager.cc
 */

#include <boost/thread.hpp>
#include <string>
#include <iostream>
#include <stdio.h>

#include "trace.h"
#include "cache.h"
#include "lru.h"
#include "message_queue.h"

using namespace std;
int num_sims = 4;
int cache_size = 1024;
int block_size = 4096;

int main() {
  string buffer;
  vector<MQueue<Trace>*> mqueues;
  vector<LRU*> sims;

  for (int i = 0; i < num_sims; i++) {
    MQueue<Trace> *mqueue = new MQueue<Trace>();
    mqueues.push_back(mqueue);
    LRU *lru = new LRU(mqueue, cache_size, block_size);
    sims.push_back(lru);
    boost::thread t(boost::bind(&Cache::simulate, lru));
  }

  while (true) {
    getline(cin, buffer);
    Trace t(buffer);
    for (int i = 0; i < num_sims; i++) {
      mqueues[i]->enqueue(t);
      printf("cache_hits: %d, cache_requests:%d\n", sims[i]->get_num_hits(), sims[i]->get_num_requests());
    }
    long i = t.inode_no;
    printf("type: <%d>, inode: <%d>, offset: <%d>, num_bytes: <%d>\n", t.type, t.inode_no, t.offset, t.num_bytes);


  }
}
