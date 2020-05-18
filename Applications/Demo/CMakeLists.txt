find_package(Qt5 COMPONENTS OpenGL REQUIRED)

file(GLOB sources src/*.cpp)
file(GLOB headers include/*.hpp)
file(GLOB ui UI/*.ui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
qt5_wrap_ui(uifile ${ui})

add_executable(Demo ${sources} ${headers} ${uifile})

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

target_compile_features(Demo PRIVATE cxx_std_17) 
target_compile_options(Demo PRIVATE ${flags}) 

add_dependencies(Demo AVLIT-core)

target_link_libraries(Demo 
                      Qt5::OpenGL 
                      AVLIT-core)

target_include_directories(Demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
