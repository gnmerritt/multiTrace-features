# Get all the Qt goodies
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

# The project's files:
set(GUI_SOURCES
  mainwindow.cpp
  layerviewer.cpp
  cortexviewer.cpp
  cortexthread.cpp
  main.cpp
)

set(GUI_HEADERS
  mainwindow.hpp
  layerviewer.hpp
  cortexthread.hpp
  cortexviewer.hpp
)

set(GUI_FORMS
  mainwindow.ui
  cortexviewer.ui
  layerviewer.ui
)

# Invoke the QT pre-processor
QT4_WRAP_UI(GUI_FORMS_MOC ${GUI_FORMS})
QT4_WRAP_CPP(GUI_HEADERS_MOC ${GUI_HEADERS})

ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)

# necessary to include the parsed UI files
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

set(GUI_LIBRARIES
  MultiTrace
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
)

add_executable(qTrace
  ${GUI_SOURCES}
  ${GUI_HEADERS_MOC}
  ${GUI_FORMS_MOC}
)
target_link_libraries(qTrace ${GUI_LIBRARIES})
