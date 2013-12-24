/*
 * trace.h
 *
 */

#ifndef _TRACE_H_
#define _TRACE_H_

#include <string>

struct Trace {
  long inode_no; // i-node of file being read/written to
  long offset;   // offset in file
  int num_bytes; // number of bytes being read or written
  enum Type {READ, WRITE} type;

  Trace(std::string);

};

#endif

