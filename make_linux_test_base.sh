#!/bin/bash
export HEADERS="-I../tbb/include -I../gsl-lite/include -I. -I../openvr_clean/openvr/headers -I../vrstrings/headers"

export BASE_SOURCES="base_serialization.cpp crc_32.cpp log.cpp slab_allocator.cpp url_named.cpp"
export BASE_TEST_SOURCES="unit_tests/test_base_main.cpp unit_tests/test_result.cpp unit_tests/test_segmented_list.cpp unit_tests/test_slab_allocator.cpp"

export TIME_CONTAINER_TEST_SOURCES="unit_tests/test_time_containers.cpp unit_tests/test_schema_common.cpp unit_tests/test_time_containers_main.cpp"

export VR_BASE_SOURCES="vr_tmp_vector.cpp openvr_broker.cpp tracker_config.cpp"

export VR_STRINGS_SOURCES="../vrstrings/src/openvr_string_gen_enums.cpp ../vrstrings/src/openvr_string_structs.cpp"

export VR_KEYS_TEST_SOURCES="unit_tests/test_vr_keys_main.cpp unit_tests/test_indexers.cpp unit_tests/tracker_test_context.cpp"

export INDEXER_SOURCES="vr_applications_indexer.cpp vr_applications_properties_indexer.cpp vr_device_properties_indexer.cpp vr_mime_types_indexer.cpp vr_overlay_indexer.cpp vr_properties_indexer.cpp vr_resources_indexer.cpp vr_settings_indexer.cpp vr_string_indexer.cpp"

export TRACKER_TEST_SOURCES="unit_tests/test_gui_usecase.cpp unit_tests/test_tracker_main.cpp unit_tests/tracker_test_context.cpp"

export TRAVERSE_SOURCES="vr_tracker_traverse.cpp"

export TRAVERSE_TEST_SOURCES="unit_tests/UPDATE.cpp unit_tests/test_traverse_main.cpp unit_tests/tracker_test_context.cpp unit_tests/test_tracker_serialization.cpp"

export CURSOR_SOURCES="vr_applications_cursor.cpp vr_chaperone_cursor.cpp vr_chaperone_setup_cursor.cpp vr_compositor_cursor.cpp vr_cursor_context.cpp vr_cursor.cpp vr_extended_display_cursor.cpp vr_overlay_cursor.cpp vr_render_models_cursor.cpp vr_resources_cursor.cpp vr_settings_cursor.cpp vr_system_cursor.cpp vr_tracked_camera_cursor.cpp openvr_cppstub.cpp"

export CURSOR_TEST_SOURCES="unit_tests/test_cursors.cpp unit_tests/test_cursors_main.cpp unit_tests/tracker_test_context.cpp"

export TBB_LIB="-L../tbb/build/linux_intel64_gcc_cc5.4.1_libc2.23_kernel4.4.0_debug -ltbb_debug"

export OPENVR_LIB="-L../openvr_clean/openvr/lib/linux64 -lopenvr_api"

export COMMON_FLAGS="-g -fpermissive -std=c++11 -DHAVE_OPEN_VR_RAW -DOPENVR_STRINGS_DISABLE_PASSTHRU"


#export CXX="clang++-3.8 -Wno-undefined-inline -Wno-address-of-temporary"
export CXX="g++-6 -Wno-undefined-inline -Wno-address-of-temporary"
#export CXX="g++ -Wno-undefined-inline -Wno-address-of-temporary"

export TARGETS="test_cursors test_tracker test_traverse test_vr_keys test_time_containers test_base"

rm $TARGETS

set -x #echo on

# cursors
$CXX $COMMON_FLAGS -o test_cursors -DTEST_CURSORS_MAIN $HEADERS $BASE_SOURCES $VR_BASE_SOURCES $CURSOR_SOURCES $CURSOR_TEST_SOURCES $INDEXER_SOURCES $TRAVERSE_SOURCES -lpthread $TBB_LIB $OPENVR_LIB $VR_STRINGS_SOURCES

# traverse
$CXX $COMMON_FLAGS -o test_traverse -DTEST_TRAVERSE_MAIN $HEADERS $BASE_SOURCES $VR_BASE_SOURCES $TRAVERSE_SOURCES $TRAVERSE_TEST_SOURCES $INDEXER_SOURCES -lpthread $TBB_LIB $OPENVR_LIB $VR_STRINGS_SOURCES

# tracker 
$CXX $COMMON_FLAGS -o test_tracker -DTEST_VR_TRACKER_MAIN $HEADERS $BASE_SOURCES $VR_BASE_SOURCES $TRACKER_TEST_SOURCES $INDEXER_SOURCES -lpthread $TBB_LIB $OPENVR_LIB $VR_STRINGS_SOURCES

# base
$CXX -g -o test_base -DTEST_BASE_MAIN -std=c++11 $HEADERS $BASE_SOURCES $BASE_TEST_SOURCES -lpthread $TBB_LIB 

# time_containers
$CXX $COMMON_FLAGS -o test_time_containers -DTEST_TIME_CONTAINERS_MAIN $HEADERS $BASE_SOURCES $TIME_CONTAINER_TEST_SOURCES -lpthread $TBB_LIB 

# test_vr_keys
$CXX $COMMON_FLAGS -o test_vr_keys -DTEST_VR_KEYS_MAIN $HEADERS $BASE_SOURCES $VR_BASE_SOURCES $VR_KEYS_TEST_SOURCES $INDEXER_SOURCES -lpthread $TBB_LIB $OPENVR_LIB $VR_STRINGS_SOURCES


exit 1 


