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

message(STATUS "${HEADER_FILES}")
message(STATUS "${COMMON_HEADER_FILES}")

add_executable(
	ushanka_win64 
	${SOURCE_FILES} 
	${HEADER_FILES} 
	${COMMON_SOURCE_FILES}
	${COMMON_HEADER_FILES}
	${GLAD_SOURCES}
	${GLFW_SOURCES}
) 

target_include_directories(ushanka_win64 PUBLIC ${COMMON_INCLUDE_DIRECTORIES})

target_link_libraries(ushanka_win64 ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/lib/glfw3.lib)

target_compile_options(ushanka_win64 PRIVATE -fsanitize=address)
target_link_options(ushanka_win64 PRIVATE -fsanitize=address)
