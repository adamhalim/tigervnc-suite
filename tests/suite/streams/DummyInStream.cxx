#include "DummyInStream.h"

DummyInStream::DummyInStream() : rdr::MemInStream(buf, BUF_SIZE, false)
{
}

DummyInStream::~DummyInStream()
{
}
DummyInStream::DummyInStream(rdr::U8* data, size_t length)
                           : rdr::MemInStream(data, length, true)
{
}
