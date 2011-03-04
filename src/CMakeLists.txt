set(SOURCES capture.cpp jackcapture.cpp meter.cpp scanLine.cpp confWidget.cpp  main.cpp microphone.cpp settings.cpp grabber.cpp mainWidget.cpp ringbuffer.cpp)
set(HEADERS capture.h confWidget.h jackcapture.h meter.h ringbuffer.h settings.h config.h grabber.h mainWidget.h microphone.h scanLine.h)
set(UI_FILES confWidget.ui mainWidget.ui tmpWidget.ui)
set(RCS nadir.qrc)
qt4_wrap_cpp(MOC_SOURCES ${HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_FILES})
qt4_add_resources(RC_SOURCES ${RCS})
include_directories(${CMAKE_BINARY_DIR}/src)

add_executable(nadir ${SOURCES} ${MOC_SOURCES} ${UI_HEADERS} ${RC_SOURCES})
target_link_libraries(nadir ${QT_LIBRARIES} ${ASOUND2_LIBRARY} ${JACK_LIBRARY} ${X11_LIBRARY} ${XTST_LIBRARY})

install(TARGETS nadir DESTINATION bin)
