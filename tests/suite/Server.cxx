#include "Server.h"
#include "rfb/SMsgWriter.h"

namespace suite {

  Server::Server(int width, int height, rfb::PixelFormat pf)
  {
    out = new DummyOutStream();
    in = new DummyInStream();
    setStreams(in, out);
    setWriter(new rfb::SMsgWriter(&client, out));

    manager = new Manager(this);
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


  std::vector<encoderStats> Server::stats()
  {
    return manager->stats();
  }

  void Server::setDesktopSize(int fb_width, int fb_height,
                              const rfb::ScreenSet &layout)
  {
  }
}
