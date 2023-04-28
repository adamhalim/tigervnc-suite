include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/unix)
include_directories(${CMAKE_SOURCE_DIR}/unix/tx)
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
  benchmark/Benchmark.cxx
  codec/PPMDecoder.cxx
  codec/PNGDecoder.cxx
  codec/Image.cxx
  codec/QOIDecoder.cxx
  codec/decoderFactory.cxx
  codec/lib/fpng.cpp
  codec/lib/qoi.h
  streams/DummyInStream.cxx
  streams/DummyOutStream.cxx
  recorder/Recoder.cxx
  ../../unix/x0vncserver/Image.cxx
  ../../unix/x0vncserver/XPixelBuffer.cxx
  ../../unix/x0vncserver/PollingManager.cxx
  ../../unix/x0vncserver/Geometry.cxx
)

target_link_libraries(suite rfb ${X11_LIBRARIES})
target_compile_features(suite PRIVATE cxx_std_11)
