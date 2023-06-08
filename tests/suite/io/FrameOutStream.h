#ifndef __SUITE_FRAME_OUT_STREAM_H__
#define __SUITE_FRAME_OUT_STREAM_H__

#include "ImageUpdate.h"
#include "../codec/ImageDecoder.h"
#include <chrono>
#include <fstream>
#include <mutex>
#include <string>

namespace suite {

  /* This class is used to create the output file of a recorded VNC session.
     The file structure is as follows:

      ______________________________________________________
      |               encoding width height interval     \n| <-- File header 
      | imageSize width height x_offset y_offset frameTime | <-- Image metadata
      |       lostArea overArea encodingTime margin      \n|
      |****************************************************|
      |****************************************************|
      |****************** RAW IMAGE DATA ******************| <-- Raw dump of 
      |****************************************************|     image data.
      |****************************************************|
      |**************************************************\n|
      | imageSize width height x_offset y_offset frameTime | <-- Next image
      |       lostArea overArea encodingTime margin      \n| 
      |****************************************************|
      |****************************************************| 
      |****************** RAW IMAGE DATA ******************|
      |**************************************************\n|
      |____________________________________________________|

      FileStructure: {
        encoding:     Which encoder was used.
        width:        Framebuffer width.
        height:       Framebuffer height.
        interval:     Specified interval between frame updates (milliseconds).
        -----------------------------------------------------------------------
        imageSize:    Image size in bytes.
        width:        Image width.
        height:       Image height.
        x_offset:     Image top left x-coordinate.
        y_offset:     Image top left y-coordinate.
        frameTime:    Time until next frame update.

        lostArea:     How large of an "area" that was lost due to multiple
                      DAMAGE events detected between frame updates.

        overArea:     How much area was overdimensioned by coalescing multiple
                      DAMAGE events into a single bouding retangle.

        encodingTime: How long it took to encode the update (milliseconds).

        margin:       How many milliseconds we have to spare before the next
                      framebuffer update is recorded. Negative number indicates
                      how long we exceeded our interval.
        -----------------------------------------------------------------------
        RAW IMAGE DATA: encoded image data.
      }

      The file metadata tells which encoding was used for the images, as well
      as the framebuffer width & height, as well as the interval between each
      frame update. The header must be parsed first before processing the rest
      of the file. The file header ends with a '\n'. The image header has
      information about the next image stored in the file and is also marked by
      ending with a '\n'. On the next line, the image can be read by reading
      imageSize many bytes from the file, using the corresponding encoder.
      The raw image is also terminated with a '\n'. There is no way of knowing
      how many images are stores in the file beforehand, you have to parse the
      whole file to know that.
  */

  class FrameOutStream
  {
  public:
    FrameOutStream(std::string filename, ImageDecoder* decoder);
    ~FrameOutStream();

    // Assumes ownership if ImageUpdate
    void addUpdate(ImageUpdate* update);
    void addUpdate(rdr::U8* data, int width, int height, int x_offset, 
                   int y_offset, int size);
                  
    void writeHeader(int width, int height, double interval);
  private:
    std::ofstream file;
    std::mutex lock; // In case updates are encoded in parallel.
    bool headerWritten;
    const std::string decoder;
    std::chrono::steady_clock::time_point lastFrameTime;
  };

}


#endif // __SUITE_FRAME_OUT_STREAM_H__
