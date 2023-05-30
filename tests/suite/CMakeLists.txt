include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/common/os)
include_directories(${CMAKE_SOURCE_DIR}/unix)
include_directories(${CMAKE_SOURCE_DIR}/unix/common)
include_directories(${X11_INCLUDE_DIR})
include_directories(${FLTK_INCLUDE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_executable(suite suite.cxx)
target_sources(suite PRIVATE 
  fileUtil.cxx
  Manager.cxx
  Server.cxx
  Client.cxx
  TestWindow.cxx
  ../../vncviewer/PlatformPixelBuffer.cxx
  ../../vncviewer/Surface.cxx
  ../../vncviewer/Surface_X11.cxx
  #../../unix/xserver/hw/vnc/XserverDesktop.cc
  #../../unix/xserver/hw/vnc/vncBlockHandler.c
  #../../common/os/os.cxx
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
  codec/timed/timedEncoderFactory.cxx
  io/FrameInStream.cxx
  streams/DummyInStream.cxx
  streams/DummyOutStream.cxx
  stats/ManagerStats.cxx
  stats/EncoderStats.cxx
)

target_compile_features(suite PRIVATE cxx_std_11)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(MyTarget PUBLIC OpenMP::OpenMP_CXX)
endif()

# Recording tool
add_subdirectory(recorder)