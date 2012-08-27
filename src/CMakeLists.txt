set(SOURCES
      capture.cpp
      confWidget.cpp
      jackcapture.cpp
      key.cpp
      keyboard.cpp
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
      fixx11h.h
      jackcapture.h
      key.h
      keyboard.h
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

 set(TS_FILES
      translations/nadir_en.ts
      translations/nadir_es.ts
   )

qt4_wrap_cpp(MOC_SOURCES ${HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_FILES})
qt4_add_resources(RC_SOURCES ${RC_FILES})
include_directories(${CMAKE_BINARY_DIR}/src)

QT4_CREATE_TRANSLATION(TRANSLATION ${TS_FILES})

add_executable(nadir
                 ${SOURCES}
                 ${MOC_SOURCES}
                 ${UI_HEADERS}
                 ${RC_SOURCES}
                 ${TRANSLATION}
              )

target_link_libraries(nadir 
                 ${QT_LIBRARIES}
                 ${ASOUND2_LIBRARY}
                 ${JACK_LIBRARY}
                 ${X11_LIBRARY}
                 ${XTST_LIBRARY}
              )

# Main binary
install(TARGETS nadir DESTINATION bin)

# Translations
install(FILES
        ${CMAKE_BINARY_DIR}/src/nadir_en.qm
        ${CMAKE_BINARY_DIR}/src/nadir_es.qm
        DESTINATION share/nadir/translations
        )

