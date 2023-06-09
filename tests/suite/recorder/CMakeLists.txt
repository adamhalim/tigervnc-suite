add_executable(recorder record.cxx)

target_sources(recorder PRIVATE
  Recorder.cxx
  util.cxx
  ../io/FrameOutStream.cxx
  ../io/ImageUpdate.cxx
  ../codec/ImageDecoder.h
  ../codec/PPMDecoder.cxx
  ../codec/PNGDecoder.cxx
  ../codec/QOIDecoder.cxx
  ../codec/JPEGDecoder.cxx
  ../codec/Image.cxx
  ../codec/decoderFactory.cxx
  ../codec/lib/fpng.cpp
  ../../../unix/x0vncserver/Image.cxx
  ../../../unix/x0vncserver/XPixelBuffer.cxx
  ../../../unix/x0vncserver/PollingManager.cxx
  ../../../unix/x0vncserver/Geometry.cxx
)

add_definitions(-DHAVE_XDAMAGE)
target_link_libraries(recorder ${X11_Xdamage_LIB})

target_link_libraries(recorder rfb tx ${X11_LIBRARIES})

target_compile_features(recorder PRIVATE cxx_std_14)