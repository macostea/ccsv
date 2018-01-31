cmake_minimum_required (VERSION 2.6)
project (CCSV)

set (CCSV_VERSION_MAJOR 1)
set (CCSV_VERSION_MINOR 0)

include (CheckFunctionExists)
include (CheckTypeSize)
check_function_exists (getline HAVE_GETLINE)
check_type_size (ssize_t HAVE_SSIZE)
check_function_exists (strsep HAVE_STRSEP)

configure_file (
	"${PROJECT_SOURCE_DIR}/src/ccsv_config.h.in"
	"${PROJECT_BINARY_DIR}/ccsv_config.h"
	)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")

configure_file(
	"${PROJECT_SOURCE_DIR}/src/test/res/csv_test_file.csv"
	"${PROJECT_BINARY_DIR}/csv_test_file.csv"
	COPYONLY
	)

set (CCSV_SOURCES ${PROJECT_SOURCE_DIR}/src/ccsv/ccsv.c)

if (NOT HAVE_STRSEP OR NOT HAVE_GETLINE)
	list(APPEND CCSV_SOURCES ${PROJECT_SOURCE_DIR}/src/ccsv/win_string.c)
endif (NOT HAVE_STRSEP OR NOT HAVE_GETLINE)

add_library(ccsv ${CCSV_SOURCES})

set (CCSV_TEST_SOURCES ${PROJECT_SOURCE_DIR}/src/test/main.c)
add_executable(ccsv_test ${CCSV_TEST_SOURCES})

target_link_libraries(ccsv_test ccsv)
