set(SOURCES
      capture.cpp
      confWidget.cpp
      grabber.cpp
      jackcapture.cpp
      main.cpp
      mainWidget.cpp
      meter.cpp
      microphone.cpp
      ringbuffer.cpp
      scanLine.cpp
      settings.cpp
   )

set(HEADERS
      capture.h
      config.h
      confWidget.h
      grabber.h
      jackcapture.h
      mainWidget.h
      meter.h
      microphone.h
      ringbuffer.h
      scanLine.h
      settings.h
   )

set(UI_FILES
      confWidget.ui
      mainWidget.ui
      tmpWidget.ui
   )

set(RC_FILES
      nadir.qrc
   )

qt4_wrap_cpp(MOC_SOURCES ${HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_FILES})
qt4_add_resources(RC_SOURCES ${RC_FILES})
include_directories(${CMAKE_BINARY_DIR}/src)

add_executable(nadir
                 ${SOURCES}
                 ${MOC_SOURCES}
                 ${UI_HEADERS}
                 ${RC_SOURCES}
              )

target_link_libraries(nadir 
                 ${QT_LIBRARIES}
                 ${ASOUND2_LIBRARY}
                 ${JACK_LIBRARY}
                 ${X11_LIBRARY}
                 ${XTST_LIBRARY}
              )

install(TARGETS nadir DESTINATION bin)
