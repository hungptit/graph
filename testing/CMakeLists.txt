# Main code
PROJECT(TOOLS)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

set(PROJECTS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/../../")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Read the git tags to determine the project version
include(GetGitVersion)
get_git_version(GIT_VERSION)

# Tell the user what versions we are using
string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" VERSION ${GIT_VERSION})
message("-- Version: ${VERSION}")

# The version of the libraries
set(GENERIC_LIB_VERSION ${VERSION})
string(SUBSTRING ${VERSION} 0 1 GENERIC_LIB_SOVERSION)

include(CheckCXXCompilerFlag)
include(AddCXXCompilerFlag)
include(CXXFeatureCheck)

# set(CMAKE_BUILD_TYPE Debug)
set (CMAKE_BUILD_TYPE Release)

# For clang
set(CMAKE_CXX_COMPILER "${PROJECTS_FOLDER}3p/llvm/bin/clang++")

# Extra compiling flags
add_cxx_compiler_flag(-O3)
add_cxx_compiler_flag(-std=c++14)
add_cxx_compiler_flag(-Wall)
add_cxx_compiler_flag(-Wextra)
add_cxx_compiler_flag(-Wshadow)
add_cxx_compiler_flag(-Werror RELEASE)
add_cxx_compiler_flag(-Werror RELWITHDEBINFO)
add_cxx_compiler_flag(-Werror MINSIZEREL)
add_cxx_compiler_flag(-pedantic)
add_cxx_compiler_flag(-pedantic-errors)
add_cxx_compiler_flag(-Wshorten-64-to-32)
add_cxx_compiler_flag(-Wfloat-equal)
add_cxx_compiler_flag(-Wint-to-void-pointer-cast)
add_cxx_compiler_flag(-fstrict-aliasing)
add_cxx_compiler_flag(-DCEREAL_RAPIDJSON_HAS_CXX11_RVALUE_REFS)
if (HAVE_CXX_FLAG_FSTRICT_ALIASING)
  add_cxx_compiler_flag(-Wstrict-aliasing)
endif()
add_cxx_compiler_flag(-Wthread-safety)
if (HAVE_WTHREAD_SAFETY)
  add_definitions(-DHAVE_WTHREAD_SAFETY)
  cxx_feature_check(THREAD_SAFETY_ATTRIBUTES)
endif()

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

# Celero
include_directories ("${PROJECTS_FOLDER}/3p/Celero/include/")
set(LIB_CELERO "${PROJECTS_FOLDER}/3p/Celero/bin/libcelero.so")

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
  set(UNITTEST_SRC_FILES  tGraph tdfs tbfs tTopologicalSortedList)
  foreach (src_file ${UNITTEST_SRC_FILES})
    ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
    TARGET_LINK_LIBRARIES(${src_file} ${Boost_LIBRARIES} ${LIB_GTEST} ${LIB_GTEST_MAIN}  ${LIB_FMT} ${LIB_SNAPPY} -lpthread)
    ADD_TEST(${src_file} ./${src_file})
  endforeach (src_file)

  include_directories(${BOOST_INCLUDE_DIRS})
  set(PERF_TEST_SRC_FILES tPerf)
  foreach (src_file ${PERF_TEST_SRC_FILES})
    ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
    TARGET_LINK_LIBRARIES(${src_file} ${Boost_LIBRARIES} ${LIB_CELERO} ${LIB_FMT} ${LIB_SNAPPY} -lpthread)
  endforeach (src_file)
endif (Boost_FOUND)
