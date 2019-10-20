project(SplineInterpolation)

cmake_minimum_required(VERSION 3.10)

aux_source_directory(. DIR_SRCS)

add_executable(SplineInterpolationTest ${DIR_SRCS})

# set(LIB_SRC ./pycall_dll.cpp)

# add_library(SplineInterpolationLib SHARED ${LIB_SRC})

# install(TARGETS SplineInterpolationLib RUNTIME DESTINATION ./build)

# set_target_properties(SplineInterpolationLib PROPERTIES LINKER_LANGUAGE C)
