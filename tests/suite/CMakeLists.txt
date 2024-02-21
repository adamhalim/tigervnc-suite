include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/common/os)
include_directories(${CMAKE_SOURCE_DIR}/unix)
include_directories(${CMAKE_SOURCE_DIR}/unix/common)
include_directories(${X11_INCLUDE_DIR})
include_directories(${FLTK_INCLUDE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_executable(suite suite.cxx)
target_sources(suite PRIVATE
  Manager.cxx
  Server.cxx
  Client.cxx
  ../../vncviewer/PlatformPixelBuffer.cxx
  ../../vncviewer/Surface.cxx
  ../../vncviewer/Surface_X11.cxx
  bruteforce/BruteForce.cxx
  bruteforce/TimedBruteForceEncoder.cxx
  benchmark/Benchmark.cxx
  codec/PPMDecoder.cxx
  codec/Image.cxx
  codec/QOIDecoder.cxx
  codec/JPEGDecoder.cxx
  codec/decoderFactory.cxx
  codec/lib/qoi.h
  codec/timed/TimedEncoder.cxx
  codec/timed/timedEncoderFactory.cxx
  io/FrameInStream.cxx
  io/statswriter.cxx
  streams/DummyInStream.cxx
  streams/DummyOutStream.cxx
  stats/ManagerStats.cxx
  stats/EncoderStats.cxx
)

target_compile_features(suite PRIVATE cxx_std_11)

target_link_libraries(suite PRIVATE rfb ${FLTK_LIBRARIES})

# Recording tool
add_subdirectory(recorder)
