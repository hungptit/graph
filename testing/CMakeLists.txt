PROJECT(TOOLS)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

set(PROJECTS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/../../")

# set(CMAKE_BUILD_TYPE Debug)
set (CMAKE_BUILD_TYPE Release)

# For gcc-5.xx
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pedantic -O4")

# For clang
set(CMAKE_CXX_COMPILER "${PROJECTS_FOLDER}3p/llvm/bin/clang++")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pedantic -O3 -Wno-deprecated-register -Wno-keyword-macro -Wno-unused-local-typedef")

ENABLE_TESTING()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Used libraries
set(LIB_GTEST "${PROJECTS_FOLDER}/3p/gtest/lib/libgtest.a")
set(LIB_GTEST_MAIN "${PROJECTS_FOLDER}/3p/gtest/lib/libgtest_main.a")
set(LIB_LEVELDB "${PROJECTS_FOLDER}/3p/leveldb/out-static/libleveldb.a")
set(LIB_FMT "${PROJECTS_FOLDER}/3p/fmt/lib/libfmt.a")
set(LIB_SNAPPY "${PROJECTS_FOLDER}/3p/snappy/lib/libsnappy.a")

# Include folder
message(${PROJECTS_FOLDER})
include_directories(${PROJECTS_FOLDER})
include_directories("${PROJECTS_FOLDER}/3p/gtest/include")
include_directories("${PROJECTS_FOLDER}/3p/fmt/include")
include_directories ("${PROJECTS_FOLDER}/3p/leveldb/include")
include_directories ("${PROJECTS_FOLDER}/3p/cereal/include")

# This option make sure that we use the local boost version. Note that if the 
# system boost is installed then CMake might use that boost version.
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_NO_SYSTEM_PATHS TRUE) 
if (Boost_NO_SYSTEM_PATHS)
  set(BOOST_ROOT "${PROJECTS_FOLDER}/3p/boost")
  set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}/include")
  set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/lib")
endif (Boost_NO_SYSTEM_PATHS)

message(${BOOST_ROOT})

find_package(Boost 1.60 REQUIRED system filesystem)

if (Boost_FOUND)
  message(${Boost_LIBRARIES})
  include_directories(${BOOST_INCLUDE_DIRS})
  set(UNITTEST_SRC_FILES  tGraph)
  foreach (src_file ${UNITTEST_SRC_FILES})
    ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
    TARGET_LINK_LIBRARIES(${src_file} ${Boost_LIBRARIES} ${LIB_GTEST} ${LIB_GTEST_MAIN}  ${LIB_FMT} ${LIB_LEVELDB} ${LIB_SNAPPY} -lpthread)
    ADD_TEST(${src_file} ./${src_file})
  endforeach (src_file)
endif (Boost_FOUND)
