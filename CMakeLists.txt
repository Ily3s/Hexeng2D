cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(Hexeng2D)

file(GLOB_RECURSE HXG_SOURCES Hexeng2D/src/*.cpp)
file(GLOB_RECURSE HXG_HEADERS Hexeng2D/src/*.hpp)

find_package(OpenGL REQUIRED)

add_library(Hexeng2D SHARED ${HXG_SOURCES} ${HXG_HEADERS})

target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hexeng2D/src)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hexeng2D/dependencies/glfw/include)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hexeng2D/dependencies/glad/include)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hexeng2D/dependencies/al-soft/include)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hexeng2D/dependencies/stb/include)

target_link_libraries(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hexeng2D/dependencies/glfw/lib/glfw3.lib)
target_link_libraries(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hexeng2D/dependencies/glad/lib/Glad32s.lib)
target_link_libraries(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hexeng2D/dependencies/al-soft/lib/OpenAL32.lib)
target_link_libraries(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hexeng2D/dependencies/stb/lib/STB.lib)
target_link_libraries(Hexeng2D PUBLIC opengl32.lib)

file(GLOB_RECURSE SB_SOURCES Sandbox/src/*.cpp)
file(GLOB_RECURSE SB_LIB_HEADERS Sandbox/src/*.hpp)

add_executable(Sandbox ${SB_SOURCES} ${SB_HEADERS})

target_include_directories(Sandbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hexeng2D/src)
target_include_directories(Sandbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hexeng2D/dependencies/glfw/include)
target_include_directories(Sandbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hexeng2D/dependencies/glad/include)
target_include_directories(Sandbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hexeng2D/dependencies/al-soft/include)
target_include_directories(Sandbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hexeng2D/dependencies/stb/include)

get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(isMultiConfig)
	target_link_libraries(Sandbox PUBLIC $<$<CONFIG:DEBUG>:bin/Debug/Hexeng2D> $<$<CONFIG:RELEASE>:bin/Release/Hexeng2D>)
elseif(CMAKE_BUILD_TYPE MATCHES "DEBUG")
	target_link_libraries(Sandbox PUBLIC bin/Debug/Hexeng2D)
else()
	target_link_libraries(Sandbox PUBLIC bin/Release/Hexeng2D)
endif()


add_dependencies(Sandbox Hexeng2D)