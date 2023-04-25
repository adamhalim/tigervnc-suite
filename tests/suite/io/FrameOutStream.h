#ifndef __FRAME_OUT_STREAM_H__
#define __FRAME_OUT_STREAM_H__

#include "ImageUpdate.h"
#include <cstdint>
#include <fstream>
#include <mutex>
#include <string>

namespace suite {

  const int BUF_SIZE = 100 << 20; 

  /* This class is used to create the output file of a recorded VNC session.
     The file structure is as follows:

      ___________________________________________
      | imageSize width height x_offset y_offset |  <-- Image metadata in 
      |******************************************|      plaintext. 
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


      The image header has information about the next image stored in the file
      and is marked by ending with a '\n'.
      On the next line, the image can be read by reading imageSize many bytes
      from the file. Images are also terminated with a '\n'.
      There is no way of knowing how many images are stores in the file
      beforehand, you have to parse the whole file to know that.
      Knowledge of which image encoding was used to write is necessary to
      decode images in the file.

      FIXME: Add framebuffer resolution to header metadata.
      FIXME: Add which encoder was used to header metadata.
      FIXME: Add timing information to header metadata.
      
  */

  class FrameOutStream
  {
  public:
    FrameOutStream(std::string filename);
    ~FrameOutStream();

    void addUpdate(ImageUpdate* update);
    void addUpdate(rdr::U8* data, int width, int height, int x_offset, 
                  int y_offset, int size);
                  
    // Check if data of size will fit in buffer
    bool check(size_t size);
  protected:
    rdr::U8 buffer[BUF_SIZE];  
    rdr::U8* head;
    rdr::U8* end;
  private:
    std::string filename;
    std::ofstream file;
    std::mutex lock; // In case updates are encoded in parallel.
  };

}


#endif