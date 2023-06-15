#include "Recorder.h"
#include "../codec/decoderFactory.h"
#include <algorithm>

using namespace suite;
int main(int argc, char* argv[])
{
  if(argc < 4) {
    std::string decoderString;
    std::for_each(decodersMap.begin(), decodersMap.end(), [&](const auto &s) {
      decoderString += s.first + " ";
    });
    std::cerr << "Error, incorrect arguments\n" 
              << "Usage:\n\t"
              << argv[0] << " <X display> <output filename> " 
              << "<framerate> <encoder (default=JPEG)>\n\t"
              << "Available encoders: " << decoderString << std::endl;
    exit(1);
  }

  // FIXME: Validate display
  std::string display = argv[1];
  std::string filename = argv[2];
  int framerate = atoi(argv[3]);

  ImageDecoder* decoder = argc > 4 ? constructDecoder(argv[4])
                                   : constructDecoder(JPEG);

  Recorder recorder = Recorder(filename, decoder, display, framerate);

  recorder.startRecording();

  // FIXME: Add some way of stopping recording, like a duration argument.
  return 0;
}
