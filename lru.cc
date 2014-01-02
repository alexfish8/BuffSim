/*
 * lru.cc
 */

#include <map>
#include <list>

#include "trace.h"
#include <stdio.h>
#include "cache.h"
#include "lru.h"

LRU::LRU(MQueue<Trace>*queue, int max_cache_size, int block_size) : Cache(queue, max_cache_size, block_size) {}

void LRU::do_cache_request(int inode, int block, Trace t) {
  cache_map::iterator it = in_cache.find(std::make_pair(inode, block));
  // cache miss
  if (it == in_cache.end()) {
    if (cache.size() == get_cache_size()) {
      // cache is full, evict least recently used block
      elem evict_block = cache.back();
      in_cache.erase(evict_block);
      cache.pop_back();
    }

    // add new block to cache
    elem new_block = std::make_pair(inode, block);
    cache.push_front(new_block);
    in_cache.insert(std::make_pair(new_block, cache.begin()));
  }

    // cache hit
    else {
      // move block to front
      cache.erase(it->second);
      cache.push_front(std::make_pair(inode, block));
      set_num_hits(get_num_hits() + 1);
    }

    set_num_requests(get_num_requests() + 1);
  }



