#!/bin/bash
export HEADERS="-I/media/sf_projects/tbb/include -I/media/sf_projects/gsl-lite/include -I."
export BASE_SOURCES="unit_tests/test_slab_allocator.cpp log.cpp slab_allocator.cpp"
export BASE_TEST_SOURCES="unit_tests/test_base_main.cpp unit_tests/test_result.cpp unit_tests/test_segmented_list.cpp"
export TIME_CONTAINER_TEST_SOURCES="unit_tests/test_time_containers.cpp unit_tests/test_schema_common.cpp unit_tests/test_time_containers_main.cpp"

set -x #echo on
g++-6 -g -o test_base -DTEST_BASE_MAIN -std=c++11 $HEADERS $BASE_SOURCES $BASE_TEST_SOURCES -lpthread 


g++-6 -g -o test_time_containers -DTEST_TIME_CONTAINERS_MAIN -std=c++11 $HEADERS $BASE_SOURCES $TIME_CONTAINER_TEST_SOURCES -lpthread 

