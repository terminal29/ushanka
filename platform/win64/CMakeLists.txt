cmake_minimum_required (VERSION 3.6)
project(ushanka_win64)


# List all the source files
file(GLOB_RECURSE SOURCE_FILES
	src/*
)

file(GLOB_RECURSE HEADER_FILES 
	include/*
)

# glad
file(GLOB_RECURSE GLAD_SOURCES
	thirdparty/glad/src/*
)

file(GLOB_RECURSE GLAD_HEADERS
	thirdparty/glad/include/*
)

# glfw
file(GLOB_RECURSE GLFW_SOURCES
	thirdparty/glfw/src/*
)

file(GLOB_RECURSE GLFW_HEADERS
	thirdparty/glfw/include/*
)

set(WIN64_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
foreach(COMMON_HEADER_DIR ${COMMON_WIN64_HEADER_DIR})
	file(GLOB_RECURSE COMMON_WIN64_HEADER_FILES_TMP ${COMMON_HEADER_DIR}/*.h ${COMMON_HEADER_DIR}/*.hpp)
	list(APPEND COMMON_WIN64_HEADER_FILES ${COMMON_WIN64_HEADER_FILES_TMP})
endforeach()

message(STATUS "${HEADER_FILES}")
message(STATUS "${COMMON_HEADER_FILES}")

set(THIRDPARTY_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

add_executable(
	ushanka_win64 
	${SOURCE_FILES} 
	${HEADER_FILES} 
	${COMMON_SOURCE_FILES}
	${COMMON_HEADER_FILES}
	${GLAD_SOURCES}
	${GLFW_SOURCES}
	${COMMON_WIN64_INCLUDE_DIRECTORIES}
	${COMMON_WIN64_SOURCE_FILES}
	${WIN64_HEADER_DIR}
) 

target_include_directories(ushanka_win64 PUBLIC ${COMMON_INCLUDE_DIRECTORIES} ${COMMON_THIRDPARTY_INCLUDE_DIRECTORIES} ${THIRDPARTY_INCLUDE_DIRECTORIES} ${GLAD_HEADERS} ${GLFW_HEADERS} ${COMMON_WIN64_INCLUDE_DIRECTORIES} ${WIN64_HEADER_DIR})

target_link_libraries(ushanka_win64 ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/lib/glfw3.lib)

target_compile_options(ushanka_win64 PRIVATE -fsanitize=address)
target_link_options(ushanka_win64 PRIVATE -fsanitize=address)
