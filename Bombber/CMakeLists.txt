# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_compile_definitions(UNICODE)

file(GLOB SOURCES1
    ${PROJECT_SOURCE_DIR}/Bombber/*/*.cpp
    ${PROJECT_SOURCE_DIR}/Bombber/*.cpp
    ${PROJECT_SOURCE_DIR}/Bombber/Engine/Resource/*.cpp
)

file(GLOB_RECURSE 
    LIBS_SRC
    LIST_DIRECTORIES true
    ${PROJECT_SOURCE_DIR}/Libs/*/lib/include/
)

# Add source to this project's executable.
add_executable (Bombber 
    "Bombber.cpp" 
    "Bombber.h"
    ${SOURCES1}
)

target_include_directories(Bombber PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/Bombber"
                          ${LIBS_SRC}
)


target_link_libraries(Bombber PUBLIC
    "${PROJECT_SOURCE_DIR}/Libs/SDL2/lib/bin/x64/SDL2.lib"
)

target_link_libraries(Bombber PUBLIC
    "${PROJECT_SOURCE_DIR}/Libs/SDL2_image/lib/bin/x64/SDL2_image.lib"
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(Bombber PUBLIC
        "${PROJECT_SOURCE_DIR}/Libs/box2d/lib/bin/Debug/box2d.lib"
    )
else()
    target_link_libraries(Bombber PUBLIC
        "${PROJECT_SOURCE_DIR}/Libs/box2d/lib/bin/Release/box2d.lib"
    )
endif()

if(WIN32)
    add_compile_definitions(WIN32_WINDOW)
endif()

target_link_libraries(Bombber PUBLIC
    "${PROJECT_SOURCE_DIR}/Libs/glew/lib/bin/x64/glew32.lib"
    "${PROJECT_SOURCE_DIR}/Libs/glew/lib/bin/x64/glew32s.lib"
)

# TODO: Add tests and install targets if needed.


