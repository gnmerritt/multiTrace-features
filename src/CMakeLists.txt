set(SOURCES_LIST
  SonntagUpdate
  HebbianLearning
  Layer
  Connection
  Cortex
  Assembly
  UNR
)

add_library( MultiTrace ${SOURCES_LIST} )

add_subdirectory(test)
add_subdirectory(include)