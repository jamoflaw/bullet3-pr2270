cmake_minimum_required (VERSION 2.8.11)

# Disable in source build
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif ()

project (bullet_mt_test)

SET(CMAKE_SYSTEM_NAME Linux)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)

SET(DEPENDS_ROOT $ENV{HOME}/toolchains/Linux_x64)

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH ${DEPENDS_ROOT} /usr)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Ensure we find bullet libs as they are in weird places on Linux
find_package(Bullet REQUIRED)
include_directories(${PROJECT_NAME} ${BULLET_INCLUDE_DIRS})

add_executable (bullet_mt_test main.cpp)

target_link_libraries (bullet_mt_test ${BULLET_LIBRARIES} pthread)
