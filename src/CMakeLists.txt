add_subdirectory(utils)

add_library(core STATIC mml.cpp)
target_link_libraries(core
    INTERFACE utils
)

install(TARGETS core
    EXPORT mml
    #LIBRARY DESTINATION ${installlibdir}
    #ARCHIVE DESTINATION ${installlibdir}
    #RUNTIME DESTINATION ${installbindir}
    # Only this one usefull so far
    # This will render include_directories optional when using this lib as IMPORTED
    INCLUDES DESTINATION ${installincludedir}
)