cmake_minimum_required(VERSION 3.5.0)
project(cpp-sandbox VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

set(
  SOURCE_LIST
  Main.cpp
  TestCacheLine.cpp
  TestThreadLocal.cpp
  TestThreadLocalThread.cpp
  TestIdGen.cpp
  TestScope.cpp
  TestString.cpp
  TestStrongId.cpp)

add_executable(cpp-sandbox ${SOURCE_LIST})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
