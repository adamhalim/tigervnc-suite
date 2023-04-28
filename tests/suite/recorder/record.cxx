#include "Recorder.h"
#include "../codec/decoderFactory.h"
#include <algorithm>

using namespace suite;
int main(int argc, char* argv[])
{

  if(argc < 3) {

    std::string decoderString;
    std::for_each(decodersMap.begin(), decodersMap.end(), [&](const auto &s){
      decoderString += s.first + " ";
  });
    std::cerr << "Error, incorrect arguments\n" 
              << "Usage:\n\t"
              << argv[0] << " <X display> <output filename> " 
              << "<encoder (default=QOI)>\n\t"
              << "Available encoders: " << decoderString << std::endl;
    exit(1);
  }

  // FIXME: Validate display
  std::string display = argv[1];
  std::string filename = argv[2];

  ImageDecoder* decoder = argc > 3 ? constructDecoder(argv[3])
                                   : constructDecoder(QOI);

  Recorder recorder = Recorder(filename, decoder, display);

  recorder.startRecording();

  // FIXME: Add some way of stopping recording, like a duration argument.
  return 0;
}
