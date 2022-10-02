file(GLOB_RECURSE SB_SOURCES src/*.cpp)
file(GLOB_RECURSE SB_LIB_HEADERS src/*.hpp)

add_executable(EmptyProject ${SB_SOURCES} ${SB_HEADERS})

target_include_directories(EmptyProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/src)
target_include_directories(EmptyProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/dependencies/glfw/include)
target_include_directories(EmptyProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/dependencies/glad/include)
target_include_directories(EmptyProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/dependencies/stb/include)
target_include_directories(EmptyProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/dependencies/portaudio/include)

add_dependencies(EmptyProject Hexeng2D)
target_link_libraries(EmptyProject PUBLIC Hexeng2D)