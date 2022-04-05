file(GLOB_RECURSE HXG_SOURCES *.cpp *.c)
file(GLOB_RECURSE HXG_HEADERS *.hpp *.h)

add_library(Hexeng2D SHARED ${HXG_SOURCES} ${HXG_HEADERS})

target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw/include)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/al-soft/include)
target_include_directories(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb/include)

if (CMAKE_GENERATOR_PLATFORM MATCHES "Win32")
	target_link_libraries(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw/lib/Win32/glfw3.lib)
else ()
	target_link_libraries(Hexeng2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw/lib/x64/glfw3.lib)
endif()