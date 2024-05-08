# - Try to find ctrulib
# Once done this will define
#  LIBCTRU_FOUND - System has ctrulib
#  LIBCTRU_INCLUDE_DIRS - The ctrulib include directories
#  LIBCTRU_LIBRARIES - The libraries needed to use ctrulib
#
# It also adds an imported target named `3ds::ctrulib`.
# Linking it is the same as target_link_libraries(target ${LIBCTRU_LIBRARIES}) and target_include_directories(target ${LIBCTRU_INCLUDE_DIRS})

# DevkitPro paths are broken on windows, so we have to fix those
macro(msys_to_cmake_path MsysPath ResultingPath)
    string(REGEX REPLACE "^/([a-zA-Z])/" "\\1:/" ${ResultingPath} "${MsysPath}")
endmacro()

if(NOT DEVKITPRO)
    msys_to_cmake_path("$ENV{DEVKITPRO}" DEVKITPRO)
endif()

set(CITRO3D_PATHS $ENV{CTRULIB} libcitro3d citro3d ${DEVKITPRO}/libctru ${DEVKITPRO}/ctrulib)

find_path(LIBCITRO3D_INCLUDE_DIR citro3d.h
          PATHS ${CITRO3D_PATHS}
          PATH_SUFFIXES include libctru/include )

find_library(LIBCITRO3D_LIBRARY NAMES citro3d libcitro3d.a
          PATHS ${CITRO3D_PATHS}
          PATH_SUFFIXES lib libctru/lib )

set(LIBCITRO3D_LIBRARIES ${LIBCITRO3D_LIBRARY} )
set(LIBCITRO3D_INCLUDE_DIRS ${LIBCITRO3D_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBCTRU_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(CITRO3D  DEFAULT_MSG
LIBCITRO3D_LIBRARY LIBCITRO3D_INCLUDE_DIR)

mark_as_advanced(LIBCITRO3D_INCLUDE_DIR LIBCITRO3D_LIBRARY )
if(CITRO3D_FOUND)
    set(CITRO3D ${LIBCITRO3D_INCLUDE_DIR}/..)
    message(STATUS "setting CITRO3D to ${CITRO3D}")

    add_library(3ds::citro3d STATIC IMPORTED GLOBAL)
    set_target_properties(3ds::citro3d PROPERTIES
        IMPORTED_LOCATION "${LIBCITRO3D_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${LIBCITRO3D_INCLUDE_DIR}"
    )
endif()
