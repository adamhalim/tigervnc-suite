#include "DummyOutStream.h"

DummyOutStream::DummyOutStream() : rdr::MemOutStream()
{
}

DummyOutStream::~DummyOutStream()
{
}

size_t DummyOutStream::length()
{
  return ptr - start;
}


bool DummyOutStream::write(rdr::U8* data, size_t length)
{
  overrun(length);
  memcpy(start, data, length);
  reposition(length);
  return true;

}
size_t DummyOutStream::size()
{
  return end - ptr;
}

rdr::U8* DummyOutStream::dump()
{
  return start;
}
