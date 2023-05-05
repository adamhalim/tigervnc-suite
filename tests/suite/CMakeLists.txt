include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/unix)
include_directories(${X11_INCLUDE_DIR})
include_directories(${FLTK_INCLUDE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_executable(suite suite.cxx)
target_sources(suite PRIVATE 
  fileUtil.cxx
  Manager.cxx
  Server.cxx
  Client.cxx
  benchmark/Benchmark.cxx
  codec/PPMDecoder.cxx
  codec/PNGDecoder.cxx
  codec/Image.cxx
  codec/QOIDecoder.cxx
  codec/decoderFactory.cxx
  codec/lib/fpng.cpp
  codec/lib/qoi.h
  codec/timed/TimedEncoder.cxx
  codec/timed/TimedRawEncoder.cxx
  codec/timed/TimedRREEncoder.cxx
  codec/timed/TimedHextileEncoder.cxx
  codec/timed/TimedTightEncoder.cxx
  codec/timed/TimedTightJPEGEncoder.cxx
  codec/timed/TimedZRLEEncoder.cxx
  io/FrameInStream.cxx
  streams/DummyInStream.cxx
  streams/DummyOutStream.cxx
)

target_link_libraries(suite rfb)
target_compile_features(suite PRIVATE cxx_std_11)

# Recording tool
add_subdirectory(recorder)