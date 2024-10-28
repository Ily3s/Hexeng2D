file(GLOB_RECURSE HXG_SOURCES src/*.cpp src/*.c)
file(GLOB_RECURSE HXG_HEADERS src/*.hpp src/*.h)

file(GLOB_RECURSE GLAD_FILES dependencies/glad/*.c dependencies/glad/*.h)
file(GLOB_RECURSE STB_FILES dependencies/stb/*.c dependencies/stb/*.h)

add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/portaudio)

add_library(Hexeng2D SHARED ${HXG_SOURCES} ${HXG_HEADERS} ${GLAD_FILES} ${STB_FILES})

add_dependencies(Hexeng2D glfw)
add_dependencies(Hexeng2D portaudio_static)

target_link_libraries(Hexeng2D PUBLIC glfw)
target_link_libraries(Hexeng2D PUBLIC portaudio_static)

target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw/include)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb/include)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/portaudio/include)
