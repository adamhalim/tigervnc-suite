#ifndef __FRAME_OUT_STREAM_H__
#define __FRAME_OUT_STREAM_H__

#include "ImageUpdate.h"
#include "../codec/ImageDecoder.h"
#include <fstream>
#include <mutex>
#include <string>

namespace suite {

  const int BUF_SIZE = 100 << 20; 

  /* This class is used to create the output file of a recorded VNC session.
     The file structure is as follows:

      ___________________________________________
      |           encoding width height          |  <-- File header 
      | imageSize width height x_offset y_offset |  <-- Image metadata
      |******************************************|
      |******************************************|
      |************* RAW IMAGE DATA *************|  <-- Raw dump of image data.
      |******************************************|
      |******************************************|
      |******************************************|
      | imageSize width height x_offset y_offset |  <-- Next image
      |******************************************|
      |******************************************| 
      |************* RAW IMAGE DATA *************|
      |******************************************|
      |__________________________________________|


      The file metadata tells which encoding was used for the images, as well
      as the framebuffer width & height. The header must be parsed first before
      processing the rest of the file. The file header ends with a '\n'.
      The image header has information about the next image stored in the file
      and is marked by ending with a '\n'.
      On the next line, the image can be read by reading imageSize many bytes
      from the file. Images are also terminated with a '\n'.
      There is no way of knowing how many images are stores in the file
      beforehand, you have to parse the whole file to know that.
      Knowledge of which image encoding was used to write is necessary to
      decode images in the file.

      FIXME: Add timing information to header metadata.
      
  */

  class FrameOutStream
  {
  public:
    FrameOutStream(std::string filename, ImageDecoder* decoder);
    ~FrameOutStream();

    void addUpdate(ImageUpdate* update);
    void addUpdate(rdr::U8* data, int width, int height, int x_offset, 
                  int y_offset, int size);
                  
    // Check if data of size will fit in buffer
    bool check(size_t size);
    void writeHeader(int width, int height);
  protected:
    rdr::U8 buffer[BUF_SIZE];  
    rdr::U8* head;
    rdr::U8* end;
  private:
    std::ofstream file;
    std::mutex lock; // In case updates are encoded in parallel.
    bool headerWritten;
    const std::string decoder;
  };

}


#endif