cmake_minimum_required(VERSION 3.15)
project(${SKBUILD_PROJECT_NAME} LANGUAGES CXX C)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

add_subdirectory(src/submodules)

include_directories(src/c)
include_directories(src/submodules/spike/)

file(GLOB SOFTFLOAT_SRC submodules/spike/softfloat/*.c)

add_library(coverfloat STATIC src/c/coverfloat.c)
target_link_libraries(coverfloat PRIVATE softfloat)
set_target_properties(coverfloat PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)


pybind11_add_module(_core MODULE src/c/main.cpp)
target_link_libraries(_core PRIVATE coverfloat)

install(TARGETS _core DESTINATION ${SKBUILD_PROJECT_NAME})
