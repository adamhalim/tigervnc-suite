#include "Client.h"
#include "codec/JPEGDecoder.h"
#include <rfb/CMsgReader.h>
#include <rfb/CMsgWriter.h>

using namespace suite;

Client::Client(int width, int height, rfb::PixelFormat pf)
{
  in = new DummyInStream();
  out = new DummyOutStream();
  setStreams(in, out);

  setState(RFBSTATE_NORMAL);

  reader = new rfb::CMsgReader(this,in);
  writer = new rfb::CMsgWriter(&server, out);
  setReader(reader);
  setWriter(writer);

  setPixelFormat(fbPF);
  pb = new rfb::ManagedPixelBuffer(pf, width, height);
  server.setDimensions(pb->width(),pb->height());
  setFramebuffer(pb);
}

Client::~Client()
{
  delete in;
  delete out;
  // reader & writer get deleted in superclass
}

void Client::setInStream(DummyInStream* in)
{
  delete this->in;
  this->in = in;
  setStreams(this->in, this->out);

  delete reader;
  reader = new rfb::CMsgReader(this, in);
  setReader(reader);
}

void Client::copyOutStreamToInStream(DummyOutStream* out)
{
  uint8_t* data = new uint8_t[out->length()];
  memcpy(data, out->dump(), out->length());
  DummyInStream* is = new DummyInStream(data, out->length());
  setInStream(is);
}


void Client::resizeFramebuffer()
{
  rfb::ModifiablePixelBuffer *pb;
  pb = new rfb::ManagedPixelBuffer(server.pf(), server.width(),
                                                server.height());
  setFramebuffer(pb);
}

void Client::framebufferUpdateStart()
{
  rfb::CConnection::framebufferUpdateStart();
}

void Client::framebufferUpdateEnd()
{
  #ifdef _DEBUG
    // Dump framebuffer to file after each framebufferUpdate
    rfb::PixelBuffer* pb;
    int stride;
    pb = getFramebuffer();

    const uint8_t* data = pb->getBuffer(pb->getRect(), &stride);

    JPEGDecoder decoder = JPEGDecoder();
    decoder.encodeImageTofile(data, pb->width(), pb->height(),
                                          "debug_output.png");
  #endif

  rfb::CConnection::framebufferUpdateEnd();
}

bool Client::dataRect(const rfb::Rect&r , int encoding)
{
  return rfb::CConnection::dataRect(r, encoding);
}
