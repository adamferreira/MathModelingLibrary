
cmake_minimum_required(VERSION 3.16)
project(mml--)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set some install directories variables
get_filename_component(installdir ${CMAKE_INSTALL_PREFIX}/mml ABSOLUTE)
get_filename_component(installlibdir ${installdir}/lib ABSOLUTE)
get_filename_component(installincludedir ${installdir}/include ABSOLUTE)
get_filename_component(installdocdir ${installdir}/doc ABSOLUTE)
get_filename_component(installbindir ${installdir}/bin ABSOLUTE)
get_filename_component(installsharedir ${installdir}/share ABSOLUTE)

add_subdirectory(src)

install(EXPORT mml NAMESPACE mml:: DESTINATION ${installdir})
