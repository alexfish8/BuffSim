#include <map>
#include <list>

#include "lru.h"

using namespace std;

LRU::LRU(int max_size) {
  num_hits = 0;
  num_requests = 0;
  this->max_size = max_size;
  cache = new Cache();
  block_positions = new CacheMap();
}

int LRU::get_num_hits() {
  return num_hits;
}

int LRU::get_num_requests() {
  return num_requests;
}

int LRU::get_max_size() {
  return max_size;
}

void LRU::cache_access(int block) {
  CacheMap::iterator cache_entry = block_positions->find(block);

  // cache miss
  if (cache_entry == block_positions->end()) {

    // evict block
    if (block_positions->size() == max_size) {
      int lru_block = cache->back();
      cache->pop_back();
      block_positions->erase(block_positions->find(lru_block));
    }

    cache->push_front(block);
    block_positions->insert(std::pair<int, Cache::iterator>(block, cache->begin()));
  }

  // cache hit
  else {
    cache->erase(cache_entry->second);
    cache->push_front(block);
    num_hits++;
  }

  num_requests++;
}

/*
  public:
    LRU();
    ~LRU();

    void cache_access(int);
    int get_num_hits();
    int get_num_requests();

  private:
    int num_hits;
    int num_requests;

    map<int, list<int>::iterator> *cache_position;
    list<int> *cache;
};
*/
