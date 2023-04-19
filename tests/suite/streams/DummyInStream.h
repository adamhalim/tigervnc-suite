#ifndef __DUMMYINSTREAM_H__
#define __DUMMYINSTREAM_H__
#include "rdr/MemInStream.h"
#include "rdr/InStream.h"

class DummyInStream : public rdr::MemInStream
{
public:
  DummyInStream();
  ~DummyInStream();
  DummyInStream(rdr::U8* data, size_t length);

private:
  static const int BUF_SIZE = 10 << 20; // 10MB
  rdr::U8* buf[BUF_SIZE]; 
};
#endif