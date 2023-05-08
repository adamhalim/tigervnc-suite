#include "Server.h"
#include "Manager.h"
#include "codec/timed/TimedEncoder.h"
#include "codec/timed/timedEncoderFactory.h"
#include "rfb/SConnection.h"
#include "rfb/SMsgWriter.h"

namespace suite {

  Server::Server(int width, int height, rfb::PixelFormat pf)
  {
    init(width, height, pf);
    manager = new Manager(this);
  }

  Server::Server(int width, int height, std::array<EncoderClass,
                                                   ENCODERS_COUNT> encoders,
                                                   rfb::PixelFormat pf)
  {
    auto encoders_ = std::array<rfb::Encoder*, ENCODERS_COUNT>();

    for (int i = 0; i < ENCODERS_COUNT; i++) {
      encoders_[i] = constructEncoder(encoders[i], this); 
    }

    init(width, height, pf);
    manager = new Manager(this, encoders_);
  }

  void Server::init(int width, int height, rfb::PixelFormat pf)
  {
    out = new DummyOutStream();
    in = new DummyInStream();
    setStreams(in, out);
    setWriter(new rfb::SMsgWriter(&client, out));

    this->updates = rfb::SimpleUpdateTracker();

    setPixelFormat(fbPF);
    client.setPF(fbPF);

    pb = new rfb::ManagedPixelBuffer(pf, width,  height);
    setEncodings(sizeof(encodings) / sizeof(*encodings), encodings);
  }

  Server::~Server() 
  {
    delete out;
    delete in;
    delete manager;
    delete pb;
  }

  void Server::loadImage(Image *image, int x, int y)
  {
    rfb::Rect rect(x,y, x + image->width, y + image->height);
    int stride;
    pb->getBuffer(rect, &stride);
    pb->imageRect(rect, image->getBuffer());
    
    rfb::UpdateInfo ui;
    const rfb::Region changed(rect);

    updates.add_changed(rect);
    updates.getUpdateInfo(&ui, changed);

    writeUpdate(ui, pb);
  }

  void Server::writeUpdate(const rfb::UpdateInfo& ui,
                           const rfb::PixelBuffer* pb)
  {
    manager->writeUpdate(ui, pb, NULL);
  }


  std::map<EncoderClass, encoderStats> Server::stats()
  {
    return manager->stats();
  }

  void Server::setDesktopSize(int fb_width, int fb_height,
                              const rfb::ScreenSet &layout)
  {
  }
}
