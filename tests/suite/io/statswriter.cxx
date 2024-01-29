#include "statswriter.h"
#include "../stats/ManagerStats.h"
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>

namespace suite {

  static int counter = 0;

  // -=- statswriter.cxx - the stats are structured as follows:
  /*
    |--root
      recorderstats.txt
      |--1
        |--EncoderSetting-compressionlevel(-qualitylevel)
        |-- encodersettings.txt
        |-- delayedframes.txt
        |-- writeupdates.txt
          |--Encoder(1)
            |--encoderstats.txt
            |--encoderwriterects.txt
            |--encoderwritesolidrects.txt
          |--Encoder(2)
            |--encoderstats.txt
            |--encoderwriterects.txt
            |--encoderwritesolidrects.txt
          |--     .
          |--     .
          |--     .
          |--Encoder(N)
      |--2
        |--EncoderSetting-compressionlevel(-qualitylevel)
        |-- encodersettings.txt
        |-- delayedframes.txt
        |-- writeupdates.txt
          |--     .
          |--     .
          |--     .
  */

  void saveEncodingStats(Server *server)
  {
    counter++;

    std::string counterStr;
    std::string dir;
    ManagerStats stats;

    counterStr = ROOT_DIR + std::to_string(counter) + "/";
    struct stat sb;
    if (stat(ROOT_DIR, &sb) == -1) {
      if (mkdir(ROOT_DIR, 0700)) {
        fprintf(stderr, "error creating dir \"%d\": %s\n",
                counter, strerror(errno));
        return;
      }
    }

    if (mkdir(counterStr.c_str(), 0700)) {
      fprintf(stderr, "error creating dir \"%d\": %s\n",
              counter, strerror(errno));
      return;
    }

    dir = createEncoderSettingsDir(server->settings, counterStr);
    saveEncoderSettings(server->settings, dir);

    stats = server->stats();
    saveManagerWriteUpdateStats(stats, dir);
    saveManagerDelayedFrames(stats, dir);

    for (auto const& es : stats.encoders) {
      if (es->currentWriteUpdate == 0)
        continue;
      std::string encoderDir = createEncoderDir(es, dir);
      saveEncoderStats(es, encoderDir);
      saveEncoderWriteUpdates(es, encoderDir);
    }
  }

  void saveRecordingStats(RecorderStats &stats)
  {
    /*
      File structure follows struct RecorderStats (one row per frame update):
        lostDataArea overDimensionedArea encodingTime margin
    */

    std::string filename = std::string(ROOT_DIR) + RECORDER_STATS;
    std::ofstream of(filename);

    for (const ImageUpdateStats& update : stats.stats) {
      of << update.lostDataArea << " " << update.overDimensionedArea << " "
         << update.encodingTime << " " << update.margin << "\n";
    }
  }

  std::string createEncoderSettingsDir(EncoderSettings& settings,
                                       std::string dir)
  {
    std::string encoderclass;
    std::stringstream ss;
    std::string encoderRootDir;

    encoderclass = encoderClasstoString(settings.encoderClass);

    ss << dir << settings.name << "_" << settings.compression
       << "_" << settings.quality;

    // EncoderSetting-compressionlevel(-qualitylevel)
    encoderRootDir = ss.str();

    if (mkdir(ss.str().c_str(), 0700))
      throw std::ios_base::failure(strerror(errno));

    return ss.str();
  }

  std::string createEncoderDir(TimedEncoder* encoder, std::string dir)
  {
    std::string encoderDir = dir + "/"
                           + encoderClasstoString(encoder->encoderClass);

    if (mkdir(encoderDir.c_str(), 0700)) {
      throw std::ios_base::failure(strerror(errno));
    }
    return encoderDir;
  }

  void saveEncoderSettings(EncoderSettings& settings, std::string dir)
  {
    /* File structure follows struct EncoderSettings:
      EncoderClass encoderClass;
      int rfbEncoding1, rfbEncoding2,..., rfbEncodingN;
      size_t encodingSize;
      PseudoEncodingLevel quality;
      PseudoEncodingLevel compression;
      std::string name;
    */

    std::ofstream of(dir + "/" + ENCODER_SETTINGS);

    of << settings.encoderClass << "\n";
    for (uint i = 0; i < settings.encodingSize; i++) {
      of << settings.rfbEncoding[i] << " ";
    }
    of << "\n" << settings.encodingSize << "\n"
       << settings.quality << "\n"
       << settings .compression << "\n"
       << settings.name;
  }

  void saveEncoderStats(TimedEncoder* encoder, std::string dir)
  {
    EncoderStats* stats;

     std::ofstream of(dir + "/"
                    + encoderClasstoString(encoder->encoderClass)
                    + "_" + ENCODER_STATS);

    /* File structure follows struct EncoderStats:
      ---------------------------------
        double writeRectEncodetime;
        double writeSolidRectEncodetime;
        int inputSizeRects;
        int outputSizeRects;
        int inputSizeSolidRects;
        int outputSizeSolidRects;
        int nRects;
        int nSolidRects;
        std::string name;
      ---------------------------------
      The field
        std::map<int,WriteRects> writeUpdates;
      is stored in separate files (saveEncoderWriteUpdates()).
    */

    stats = encoder->stats();
    of << stats->writeRectEncodetime << "\n"
       << stats->writeSolidRectEncodetime << "\n"
       << stats->inputSizeRects << "\n"
       << stats->outputSizeRects << "\n"
       << stats->inputSizeSolidRects << "\n"
       << stats->outputSizeSolidRects << "\n"
       << stats->nRects << "\n"
       << stats->nSolidRects << "\n"
       << stats->name << "\n";
  }

  void saveEncoderWriteUpdates(TimedEncoder* encoder, std::string dir)
  {
    EncoderStats* stats;

    std::ofstream rectsFile(dir + "/"
                          + encoderClasstoString(encoder->encoderClass)
                          + "_" + ENCODER_WRITE_RETCS);

    std::ofstream solidRectsFile(dir + "/"
                               + encoderClasstoString(encoder->encoderClass)
                               + "_" + ENCODER_WRITE_SOLID_RECTS);

    /* File structure for encoderWriteRects (one row per writeRect):
        writeUpdate timeSpent pixelCount


      File structure for encoderWriteSolidRects (one row per writeSolidRect):
        writeUpdate timeSpent pixelCount

      Since a single frameUpdate can contain both writeRect() and
      writeSolidRect() calls, we use two files and map them to
      their respective writeUpdate.
    */

    stats = encoder->stats();
    for (const auto& update : stats->writeUpdates) {
      const WriteRects writeRects = update.second;
      const int frameUpdate = update.first;
      for (const WriteRect& writeRect : writeRects.writeRects) {
        rectsFile << frameUpdate << " " << writeRect.timeSpent << " "
                  << writeRect.pixelCount << "\n";
      }

      for (const WriteRect& writeSolidRect : writeRects.writeSolidRects) {
        solidRectsFile << frameUpdate << " " << writeSolidRect.timeSpent
                       << " " << writeSolidRect.pixelCount << "\n";
      }
    }
  }

  void saveManagerWriteUpdateStats(ManagerStats& manager, std::string dir)
  {
     std::ofstream of(dir + "/" + WRITE_UPDATES);

    /* File structure (ms, per writeUpdate):
      timeRequred timeSpent pixelCount
    */
    for (WriteUpdate& update : manager.writeUpdateStats) {
      of << update.timeRequired << " " << update.timeSpent
         << " " << update.size << "\n";
    }
  }

  void saveManagerDelayedFrames(ManagerStats& stats, std::string dir)
  {
    std::ofstream of(dir + "/" + DELAYED_FRAMES);
    of << stats.delayedFrames;
  }
}