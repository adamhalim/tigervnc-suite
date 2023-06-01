#ifndef __SUITE_STATSWRITER_H__
#define __SUITE_STATSWRITER_H__
#include "../Server.h"

namespace suite {
// -=- statswriter.h - saves ManagerStats/EncoderStats to file.

  const char* const ROOT_DIR  ="stats/";
  const char* const DELAYED_FRAMES = "delayedframes.txt";
  const char* const ENCODER_SETTINGS = "encodersettings.txt";
  const char* const WRITE_UPDATES = "writeupdates.txt";

  const char* const ENCODER_STATS  ="encoderstats.txt";
  const char* const ENCODER_WRITE_RETCS = "encoderwriterects.txt";
  const char* const ENCODER_WRITE_SOLID_RECTS = "encoderwritesolidrects.txt";

  void saveStats(Server* server);
  std::string createEncoderSettingsDir(EncoderSettings& settings,
                                       std::string dir);
  std::string createEncoderDir(TimedEncoder* encoder, std::string dir);
  void saveEncoderSettings(EncoderSettings& settings, std::string dir);
  void saveEncoderStats(TimedEncoder* encoder, std::string dir);
  void saveEncoderWriteUpdates(TimedEncoder* encoder, std::string dir);
  void saveManagerWriteUpdateStats(ManagerStats& manager, std::string dir);
  void saveManagerDelayedFrames(ManagerStats& manager, std::string dir);
}
#endif // __SUITE_STATSWRITER_H__
