cmake_minimum_required (VERSION 3.5)
project(ushanka_3ds)
enable_language(ASM) # ASM must be enabled for add_shbin_library

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DKA_SUGGESTED_C_FLAGS}") # Use the devkitArm suggested flags. This is not mandatory.
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${DKA_SUGGESTED_C_FLAGS}") # Use the devkitArm suggested flags. This is not mandatory.

# Note that you must copy the cmake folder and the DevkitArm3DS.cmake file in this directory
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake) # Add the cmake folder to the modules paths, so that we can use the tools and find_package for ctrulib
include(Tools3DS) # Include all the macros and tools needed for 3ds development.

find_package(CTRULIB REQUIRED) # Look for ctrulib
find_package(CITRO3D REQUIRED) # Look for ctrulib
find_package(CITRO2D REQUIRED) # Look for ctrulib

# List all the shader files
file(GLOB_RECURSE SHADERS_FILES
	assets/shaders/*.pica
)

if(NOT "${SHADERS_FILES}" STREQUAL "")
	add_shbin_library(shaders ${SHADERS_FILES})# create a library that assembles and embeds the shaders shbin.
	set(SHADERS_LIB "shaders")
	message(STATUS "SHADERS FOUND")
else()
	message(STATUS "NO SHADERS FOUND")
	set(SHADERS_LIB "")
endif()


# List all the source files
file(GLOB_RECURSE SOURCE_FILES
	src/*
)

file(GLOB_RECURSE HEADER_FILES 
	include/*
)

set(3DS_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
foreach(CTR_HEADER_DIR ${3DS_HEADER_DIR})
	file(GLOB_RECURSE COMMON_3DS_HEADER_FILES_TMP ${CTR_HEADER_DIR}/*.h ${CTR_HEADER_DIR}/*.hpp)
	list(APPEND COMMON_3DS_HEADER_FILES ${COMMON_3DS_HEADER_FILES_TMP})
endforeach()

message(STATUS "HEADER_FILES=${HEADER_FILES}")

add_executable(
	ushanka_3ds 
	${SOURCE_FILES} 
	${HEADER_FILES} 
	${COMMON_SOURCE_FILES}
	${COMMON_HEADER_FILES}
	${COMMON_3DS_INCLUDE_DIRECTORIES}
	${COMMON_3DS_SOURCE_FILES}
	${3DS_HEADER_DIR}
	) # Create the elf file

target_include_directories(ushanka_3ds PUBLIC 
	${COMMON_INCLUDE_DIRECTORIES} 
	${COMMON_THIRDPARTY_INCLUDE_DIRECTORIES} 
	${THIRDPARTY_INCLUDE_DIRECTORIES} 
	${COMMON_3DS_INCLUDE_DIRECTORIES}
	${3DS_HEADER_DIR}
)

target_link_libraries(ushanka_3ds ${SHADERS_LIB} 3ds::citro2d 3ds::citro3d 3ds::ctrulib m) # Link the texture, shaders, ctrulib and libm. This will also add the include directories needed for ctrulib and the embedded files.


# List all the texture bin files
#file(GLOB_RECURSE BIN_FILES
#	assets/textures/*.bin
#)

#target_embed_file(ushanka_3ds ${BIN_FILES}) # Embeds a file as a binary data array the same way devkitArm makefiles do.

add_3dsx_target(ushanka_3ds NO_SMDH) # Generate the .3dsx and .smdh from the textured_cube target