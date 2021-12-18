set(UTILS_HEADER_FILES
    Hash.h
    Object.h
    Types.h
)

# An INTERFACE library target does not compile sources and does not produce a library artifact on disk.
add_library(utils INTERFACE)
target_include_directories(utils
    INTERFACE
    # The location of the headers before installation
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    # The location of the headers after installation
    #$<INSTALL_INTERFACE:${installincludedir}/utils>
)

install(FILES ${UTILS_HEADER_FILES} DESTINATION ${installincludedir}/utils)
install(TARGETS utils
    EXPORT mml
    #LIBRARY DESTINATION ${installlibdir}
    #ARCHIVE DESTINATION ${installlibdir}
    #RUNTIME DESTINATION ${installbindir}
    INCLUDES DESTINATION ${installincludedir}/utils
)