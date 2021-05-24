cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_COVERAGE "Build code coverage" OFF)

set(
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("tools/gate/cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/ruslo/hunter/archive/v0.23.214.tar.gz"
  SHA1 "e14bc153a7f16d6a5eeec845fb0283c8fad8c358"
)

project(builder)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wnon-virtual-dtor")

hunter_add_package(
  Boost
  COMPONENTS
  program_options
  thread
  filesystem
)
find_package(
  Boost
  CONFIG REQUIRED
  program_options
  thread
  filesystem
)
hunter_add_package(Async++)
find_package(Async++ CONFIG REQUIRED)
find_package(Threads)

add_executable(
  ${PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/menu.cpp
 # ${CMAKE_CURRENT_SOURCE_DIR}/sources/Settings.cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/sources/LogSetup.cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/sources/Timer.cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/sources/Utils.cpp
)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/boost-process/include
)

target_link_libraries(
  ${PROJECT_NAME}
  Async++::Async++
  Boost::program_options
  Boost::thread
  Boost::filesystem
)
if(BUILD_COVERAGE)
  set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/coverage/cmake")
  find_package(codecov)
  string(APPEND CMAKE_CXX_FLAGS " --coverage")
  add_coverage(${PROJECT_NAME})
  #add_coverage(tests)
  #coverage_evaluate()
endif()

if(BUILD_DOCS)
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/_docs)
  set(DOXYGEN_EXAMPLE_PATH examples)
  set(DOXYGEN_GENERATE_HTML NO)
  set(DOXYGEN_GENERATE_MAN YES)
  find_package(Doxygen)
  doxygen_add_docs(docs include)
  install(DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}/man DESTINATION share)
  # add_dependencies(install docs) need to call target docs before install or package
endif()

include(CPackConfig.cmake)

#install(TARGETS ${PROJECT_NAME} demo
 # EXPORT "${PROJECT_NAME}-targets"
  #RUNTIME DESTINATION bin
  #ARCHIVE DESTINATION lib
  #LIBRARY DESTINATION lib
  #INCLUDES DESTINATION include
#)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)

#install(EXPORT "${PROJECT_NAME}-targets"
  #NAMESPACE "${PROJECT_NAME}::"
  #DESTINATION "lib/cmake"
#)
