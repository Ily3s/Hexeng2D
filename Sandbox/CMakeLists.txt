file(GLOB_RECURSE SB_SOURCES src/*.cpp)
file(GLOB_RECURSE SB_HEADERS src/*.hpp)

add_executable(Sandbox ${SB_SOURCES} ${SB_HEADERS})

target_include_directories(Sandbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/src)
target_include_directories(Sandbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/dependencies/glfw/include)
target_include_directories(Sandbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/dependencies/glad/include)
target_include_directories(Sandbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/dependencies/stb/include)
target_include_directories(Sandbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/dependencies/portaudio/include)

add_dependencies(Sandbox Hexeng2D)
target_link_libraries(Sandbox PUBLIC Hexeng2D)