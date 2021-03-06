# Main code
PROJECT(TOOLS)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../")
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
# set(CMAKE_CXX_COMPILER "clang++")

# Extra compiling flags
add_cxx_compiler_flag(-O3)
add_cxx_compiler_flag(-std=c++14)
add_cxx_compiler_flag(-Wall)
# add_cxx_compiler_flag(-Wextra)
add_cxx_compiler_flag(-Wshadow)
# add_cxx_compiler_flag(-Werror RELEASE)
# add_cxx_compiler_flag(-Werror RELWITHDEBINFO)
# add_cxx_compiler_flag(-Werror MINSIZEREL)
add_cxx_compiler_flag(-pedantic)
# add_cxx_compiler_flag(-pedantic-errors)
add_cxx_compiler_flag(-Wshorten-64-to-32)
add_cxx_compiler_flag(-Wfloat-equal)
add_cxx_compiler_flag(-Wint-to-void-pointer-cast)
add_cxx_compiler_flag(-fstrict-aliasing)

add_cxx_compiler_flag(-DCEREAL_RAPIDJSON_HAS_CXX11_RVALUE_REFS)
add_cxx_compiler_flag(-DFMT_HEADER_ONLY)

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

# Include folder
include_directories(${ROOT_DIR})
include_directories("${ROOT_DIR}/../3p/include")

# Celero
set(LIB_CELERO "${ROOT_DIR}../3p/lib/libcelero.a")
# set(LIB_CELERO "${ROOT_DIR}../3p/lib/libcelero.dylib")

set(UNITTEST_SRC_FILES  tGraph tdfs tbfs tTopologicalSortedList)
foreach (src_file ${UNITTEST_SRC_FILES})
  ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
  TARGET_LINK_LIBRARIES(${src_file} -lpthread)
  ADD_TEST(${src_file} ./${src_file})
endforeach (src_file)

link_directories(${ROOT_DIR}/bin)
include_directories(${BOOST_INCLUDE_DIRS})
set(PERF_TEST_SRC_FILES tPerf)
foreach (src_file ${PERF_TEST_SRC_FILES})
  ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
  TARGET_LINK_LIBRARIES(${src_file} ${LIB_CELERO} ${LIB_FMT} -lpthread)
endforeach (src_file)
