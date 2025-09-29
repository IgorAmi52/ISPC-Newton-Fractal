cmake_minimum_required(VERSION 3.20)
project(ispc_newton_fractal LANGUAGES CXX)

# ================================
# C++ standard & build type
# ================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to Release if not specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ================================
# Host compiler optimizations
# ================================
if (MSVC)
  add_compile_options(/O2)
else()
  add_compile_options(-O3 -march=native)
endif()

# ================================
# ISPC setup
# ================================
find_program(ISPC_EXE ispc REQUIRED)

# Allow override from command line: -DISPC_TARGET=avx512skx-i32x16, neon-i32x4, ...
if (NOT DEFINED ISPC_TARGET)
  if (APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    # Apple Silicon (requires ISPC build with ARM/NEON support)
    set(ISPC_TARGET "neon-i32x4")
  elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|amd64|x86")
    set(ISPC_TARGET "avx2-i32x8")
  else()
    set(ISPC_TARGET "avx2-i32x8")
  endif()
endif()
message(STATUS "Using ISPC target: ${ISPC_TARGET}")

# Paths
set(ISPC_DIR  ${CMAKE_SOURCE_DIR}/ispc)
set(ISPC_SRC  ${ISPC_DIR}/fractal.ispc)
set(ISPC_HELP ${ISPC_DIR}/helper.ispc)

set(GEN_HDR ${CMAKE_BINARY_DIR}/fractal_ispc.h)
if (MSVC)
  set(GEN_OBJ ${CMAKE_BINARY_DIR}/fractal_ispc.obj)
else()
  set(GEN_OBJ ${CMAKE_BINARY_DIR}/fractal_ispc.o)
endif()

# Compile ISPC -> object + header
# -I ensures #include "helper.ispc" is found even when running from build/
add_custom_command(
  OUTPUT  ${GEN_OBJ} ${GEN_HDR}
  COMMAND ${ISPC_EXE} -I "${ISPC_DIR}" "${ISPC_SRC}"
          -o "${GEN_OBJ}" -h "${GEN_HDR}" --target=${ISPC_TARGET}
  DEPENDS ${ISPC_SRC} ${ISPC_HELP}
  VERBATIM
)
add_custom_target(gen_ispc DEPENDS ${GEN_OBJ} ${GEN_HDR})

# ================================
# Host executable
# ================================
# Add your host sources here:
set(HOST_SOURCES
  src/main.cpp
  src/host_render.cpp
  src/config.cpp
)

add_executable(newton ${HOST_SOURCES})
add_dependencies(newton gen_ispc)

# Include the build/ dir so C++ can see generated fractal_ispc.h
target_include_directories(newton PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/include
)

# Link the ISPC-produced object file
target_link_libraries(newton PRIVATE "${GEN_OBJ}")

# ================================
# Helpful status
# ================================
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Build type:   ${CMAKE_BUILD_TYPE}")
