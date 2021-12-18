add_subdirectory(utils)
#include_directories(utils)

add_library(core STATIC mml.cpp)
target_link_libraries(core
    INTERFACE utils
)

install(TARGETS core EXPORT mml DESTINATION ${installlibdir})