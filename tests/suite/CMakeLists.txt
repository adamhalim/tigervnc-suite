include_directories(${CMAKE_SOURCE_DIR}/common)
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
)

target_link_libraries(suite rfb)
target_compile_features(suite PRIVATE cxx_std_11)
