cmake_minimum_required(VERSION 3.6)
project(ushanka)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

MACRO(GROUP_SRC SRCS)
  foreach(FILE ${SRCS}) 
    #convert source file to absolute
    get_filename_component(ABSOLUTE_PATH "${FILE}" ABSOLUTE)
    # Get the directory of the absolute source file
    get_filename_component(PARENT_DIR "${ABSOLUTE_PATH}" DIRECTORY)
    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    # Group into "Source Files" and "Header Files"
    if ("${FILE}" MATCHES ".*\\.cpp")
      set(GROUP "Source Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
      set(GROUP "Header Files${GROUP}")
    endif()
    source_group("${GROUP}" FILES "${FILE}")
  endforeach()
ENDMACRO(GROUP_SRC)

set (CMAKE_CXX_STANDARD 23)

set(COMMON_INCLUDE_DIRECTORIES 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/common/include
)

set (COMMON_THIRDPARTY_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
)

set(COMMON_HEADER_FILES "")
foreach(COMMON_HEADER_DIR ${COMMON_INCLUDE_DIRECTORIES})
	file(GLOB_RECURSE COMMON_HEADER_FILES_TMP ${COMMON_HEADER_DIR}/*.h ${COMMON_HEADER_DIR}/*.hpp)
	list(APPEND COMMON_HEADER_FILES ${COMMON_HEADER_FILES_TMP})
endforeach()


# List all the source files
file(GLOB_RECURSE COMMON_SOURCE_FILES
	src/*
	platform/common/src/*
)
file(TO_CMAKE_PATH "${COMMON_SOURCE_FILES}" COMMON_SOURCE_FILES)

# List all the source files
file(GLOB_RECURSE COMMON_WIN64_SOURCE_FILES
	platform/win64/src/*
)

set (COMMON_WIN64_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/win64/include
)


# Group the files
GROUP_SRC("${COMMON_SOURCE_FILES}")
GROUP_SRC("${COMMON_HEADER_FILES}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   add_subdirectory(./platform/3ds)
endif()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
   add_subdirectory(./platform/win64)
endif()