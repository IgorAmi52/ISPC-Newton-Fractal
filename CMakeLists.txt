cmake_minimum_required(VERSION 3.20)
project(ispc_newton_fractal LANGUAGES CXX)

# ================================  C++ standard & build type
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ================================  Host optimizations
if (MSVC)
  add_compile_options(/O2)
else()
  add_compile_options(-O3 -march=native)
endif()

# ================================  ISPC setup
find_program(ISPC_EXE ispc REQUIRED)

# Target auto-pick (override with -DISPC_TARGET=...)
if (NOT DEFINED ISPC_TARGET)
  if (APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    set(ISPC_TARGET "neon-i32x4")
  elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|amd64|x86")
    set(ISPC_TARGET "avx2-i32x8")
  else()
    set(ISPC_TARGET "avx2-i32x8")
  endif()
endif()
message(STATUS "Using ISPC target: ${ISPC_TARGET}")

# On Apple Silicon, if ISPC target is AVX*, force host to x86_64 to avoid arch mismatch
if (APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64" AND ISPC_TARGET MATCHES "avx")
  message(STATUS "Forcing host arch to x86_64 to match ISPC AVX target on Apple Silicon")
  set(CMAKE_OSX_ARCHITECTURES x86_64 CACHE STRING "" FORCE)
endif()

# ================================  Paths
set(ISPC_DIR  ${CMAKE_SOURCE_DIR}/ispc)
set(ISPC_SRC  ${ISPC_DIR}/fractal.ispc)
set(ISPC_HELP ${ISPC_DIR}/helper.ispc)

set(GEN_HDR ${CMAKE_BINARY_DIR}/fractal_ispc.h)
if (MSVC)
  set(GEN_OBJ ${CMAKE_BINARY_DIR}/fractal_ispc.obj)
else()
  set(GEN_OBJ ${CMAKE_BINARY_DIR}/fractal_ispc.o)
endif()

# ================================  Compile ISPC -> object + header
add_custom_command(
  OUTPUT  ${GEN_OBJ} ${GEN_HDR}
  COMMAND ${ISPC_EXE} -I "${ISPC_DIR}" "${ISPC_SRC}"
          -o "${GEN_OBJ}" -h "${GEN_HDR}" --target=${ISPC_TARGET}
  DEPENDS ${ISPC_SRC} ${ISPC_HELP}
  VERBATIM
)
add_custom_target(gen_ispc DEPENDS ${GEN_OBJ} ${GEN_HDR})

# ================================  ISPC task runtime (your tasksys.cpp/.h)
# Put your found tasksys.cpp here, or vendor it into third_party/ispc_tasksys/
add_library(ispc_tasksys STATIC
  third_party/ispc_tasksys/tasksys.cpp
)
target_include_directories(ispc_tasksys PUBLIC
  third_party/ispc_tasksys
)

# Ako tvoj tasksys.cpp koristi pthreads, uključi Threads (bezbedno je dodati uvek):
find_package(Threads REQUIRED)
target_link_libraries(ispc_tasksys PUBLIC Threads::Threads)

# ================================  Host executable
set(HOST_SOURCES
  src/main.cpp
  src/host_render.cpp
  src/config.cpp
)

add_executable(newton ${HOST_SOURCES})
add_dependencies(newton gen_ispc)

target_include_directories(newton PRIVATE
  ${CMAKE_BINARY_DIR}           # fractal_ispc.h (generated)
  ${CMAKE_SOURCE_DIR}/include
)

# Link ISPC object + tasksys
if (MSVC)
  target_link_options(newton PRIVATE "${GEN_OBJ}")
else()
  target_link_libraries(newton PRIVATE "${GEN_OBJ}")
endif()
target_link_libraries(newton PRIVATE ispc_tasksys)

# ================================  Tests (CTest)

include(CTest)
enable_testing()

# test_config
add_executable(test_config tests/test_config.cpp src/config.cpp)
add_dependencies(test_config gen_ispc)
target_include_directories(test_config PRIVATE ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_config PRIVATE ispc_tasksys)
add_test(NAME test_config COMMAND test_config)

# test_roots
add_executable(test_roots tests/test_roots.cpp)
add_dependencies(test_roots gen_ispc)
target_include_directories(test_roots PRIVATE ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_roots PRIVATE ispc_tasksys)
add_test(NAME test_roots COMMAND test_roots)

# test_ppm
add_executable(test_ppm tests/test_ppm.cpp)
add_dependencies(test_ppm gen_ispc)
target_include_directories(test_ppm PRIVATE ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_ppm PRIVATE ispc_tasksys)
add_test(NAME test_ppm COMMAND test_ppm)


# ================================
