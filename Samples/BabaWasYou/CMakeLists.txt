file(GLOB_RECURSE SB_SOURCES src/*.cpp)
file(GLOB_RECURSE SB_LIB_HEADERS src/*.hpp)

add_executable(BabaWasYou ${SB_SOURCES} ${SB_HEADERS})

target_include_directories(BabaWasYou PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Hexeng2D/src)
target_include_directories(BabaWasYou PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Hexeng2D/dependencies/glfw/include)
target_include_directories(BabaWasYou PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Hexeng2D/dependencies/glad/include)
target_include_directories(BabaWasYou PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Hexeng2D/dependencies/stb/include)
target_include_directories(BabaWasYou PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Hexeng2D/dependencies/portaudio/include)

add_dependencies(BabaWasYou Hexeng2D)
target_link_libraries(BabaWasYou PUBLIC Hexeng2D)
