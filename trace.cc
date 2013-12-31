/*
 * trace.cc
 *
 */

#include <string>
#include <iterator>
#include <boost/tokenizer.hpp>
#include <time.h>


#include "trace.h"

using namespace std;
using namespace boost;

static const long NSEC_PER_SEC = 1000000000L;


// adapated from boost tokenizer tutorial
Trace::Trace(string s) {
  clock_gettime(CLOCK_MONOTONIC, &start);
  tokenizer<> tok(s);

  if (distance(tok.begin(), tok.end()) < 4)
    return;

  // type
  tokenizer<>::iterator it = tok.begin();
  if (it->compare("R") == 0)
    type = READ;
  else
    type = WRITE;

  // inode
  it++;
  inode_no = atoi(it->c_str());

  // offset
  it++;
  offset = atoi(it->c_str());

  // num_bytes
  it++;
  num_bytes = atoi(it->c_str());
}

Trace::Trace() {
  type = END;
}

struct timespec Trace::age() {
  struct timespec ret;
  struct timespec end;

  clock_gettime(CLOCK_MONOTONIC, &end);
  int b = 0; // borrow

  ret.tv_nsec = end.tv_nsec - start.tv_nsec;
  if (ret.tv_nsec < 0) {
    b = 1;
    ret.tv_nsec += NSEC_PER_SEC;
  }

  ret.tv_sec = end.tv_sec - start.tv_sec - b;


  return ret;
}

struct timespec Trace::timespec_add(struct timespec t1, struct timespec t2) {
  struct timespec ret;
  int c = 0; // carry

  ret.tv_nsec = (t1.tv_nsec + t2.tv_nsec);
  if (ret.tv_nsec >= NSEC_PER_SEC) {
    ret.tv_nsec -= NSEC_PER_SEC;
    c = 1;
  }

  ret.tv_sec = t1.tv_sec + t2.tv_sec + c;
  return ret;
    
}

Trace Trace::end() {
  return Trace();
}


