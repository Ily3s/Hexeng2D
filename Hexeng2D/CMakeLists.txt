file(GLOB_RECURSE HXG_SOURCES src/*.cpp)
file(GLOB_RECURSE HXG_HEADERS src/*.hpp)

add_library(Hexeng2D SHARED ${HXG_SOURCES} ${HXG_HEADERS})

target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw/include)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/al-soft/include)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb/include)

target_link_libraries(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw/lib/glfw3.lib)
target_link_libraries(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/lib/Glad32s.lib)
target_link_libraries(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/al-soft/lib/OpenAL32.lib)
target_link_libraries(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb/lib/STB.lib)
target_link_libraries(Hexeng2D PUBLIC opengl32.lib)