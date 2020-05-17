file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE headers ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(AVLIT-core SHARED ${headers} ${sources})

target_include_directories(AVLIT-core PRIVATE Base/include Renderer/include Renderer/OpenGL/include ../Dependencies/assimp/include ${CMAKE_BINARY_DIR}/Dependencies/assimp/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DALIT_DEBUG)
    if(MSVC)
        set(flags "/W4" "/std:c++17")
    else()
        set(flags "-g" "-Wall" "-std=c++17")
    endif()
else()
    if(MSVC)
        set(flags "/O2" "/W4" "/std:c++17")
    else()
        set(flags "-O3" "-Wall" "-std=c++17")
    endif()
endif()

add_dependencies(AVLIT-core assimp)
add_dependencies(AVLIT-core glm)

target_link_libraries(AVLIT-core assimp)
target_link_libraries(AVLIT-core glm)


target_compile_features(AVLIT-core PRIVATE cxx_std_17) 
target_compile_options(AVLIT-core PRIVATE ${flags})
target_compile_definitions(AVLIT-core PRIVATE -DDLL_EXPORT)
