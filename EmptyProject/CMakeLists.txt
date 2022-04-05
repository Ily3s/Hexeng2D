file(GLOB_RECURSE SB_SOURCES src/*.cpp)
file(GLOB_RECURSE SB_LIB_HEADERS src/*.hpp)

add_executable(EmptyProject ${SB_SOURCES} ${SB_HEADERS})

target_include_directories(EmptyProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/src)
target_include_directories(EmptyProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/dependencies/glfw/include)
target_include_directories(EmptyProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/dependencies/glad/include)
target_include_directories(EmptyProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/dependencies/al-soft/include)
target_include_directories(EmptyProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Hexeng2D/dependencies/stb/include)

get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(isMultiConfig)
	target_link_libraries(EmptyProject PUBLIC $<$<CONFIG:DEBUG>:../bin/Debug/Hexeng2D> $<$<CONFIG:RELEASE>:../bin/Release/Hexeng2D>)
elseif(CMAKE_BUILD_TYPE MATCHES "DEBUG")
	target_link_libraries(EmptyProject PUBLIC ../bin/Debug/Hexeng2D)
else()
	target_link_libraries(EmptyProject PUBLIC ../bin/Release/Hexeng2D)
endif()