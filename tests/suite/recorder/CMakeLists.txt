add_executable(recorder record.cxx)

target_sources(recorder PRIVATE
  Recorder.cxx
  ../io/FrameOutStream.cxx
  ../io/ImageUpdate.cxx
  ../codec/ImageDecoder.h
  ../codec/PPMDecoder.cxx
  ../codec/PNGDecoder.cxx
  ../codec/QOIDecoder.cxx
  ../codec/Image.cxx
  ../codec/decoderFactory.cxx
  ../codec/lib/fpng.cpp
  ../../../unix/x0vncserver/Image.cxx
  ../../../unix/x0vncserver/XPixelBuffer.cxx
  ../../../unix/x0vncserver/PollingManager.cxx
  ../../../unix/x0vncserver/Geometry.cxx
)

target_link_libraries(recorder rfb ${X11_LIBRARIES})

target_compile_features(recorder PRIVATE cxx_std_14)