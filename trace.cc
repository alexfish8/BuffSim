/*
 * trace.cc
 *
 */

#include <string>
#include <iterator>
#include <boost/tokenizer.hpp>

#include "trace.h"

using namespace std;
using namespace boost;


// adapated from boost tokenizer tutorial
Trace::Trace(string s) {
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




