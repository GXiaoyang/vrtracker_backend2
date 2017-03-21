#!/bin/bash
export HEADERS="-I/media/sf_projects/tbb/include -I/media/sf_projects/gsl-lite/include -I. -I/media/sf_projects/openvr_clean/openvr/headers -I/media/sf_projects/vrstrings/headers"
export BASE_SOURCES="unit_tests/test_slab_allocator.cpp log.cpp slab_allocator.cpp"
export BASE_TEST_SOURCES="unit_tests/test_base_main.cpp unit_tests/test_result.cpp unit_tests/test_segmented_list.cpp"
export TIME_CONTAINER_TEST_SOURCES="unit_tests/test_time_containers.cpp unit_tests/test_schema_common.cpp unit_tests/test_time_containers_main.cpp"

export VR_BASE_SOURCES="vr_tmp_vector.cpp openvr_broker.cpp tracker_config.cpp"

export VR_STRINGS_SOURCES="/media/sf_projects/vrstrings/src/openvr_string_gen_enums.cpp /media/sf_projects/vrstrings/src/openvr_string_structs.cpp"

export VR_KEYS_TEST_SOURCES="unit_tests/test_vr_keys_main.cpp unit_tests/test_indexers.cpp unit_tests/tracker_test_context.cpp"

export INDEXER_SOURCES="vr_applications_indexer.cpp vr_applications_properties_indexer.cpp vr_device_properties_indexer.cpp vr_mime_types_indexer.cpp vr_overlay_indexer.cpp vr_properties_indexer.cpp vr_resources_indexer.cpp vr_settings_indexer.cpp vr_string_indexer.cpp"

export TRACKER_TEST_SOURCES="unit_tests/test_gui_usecase.cpp unit_tests/test_tracker_main.cpp"

export TRAVERSE_SOURCES="vr_tracker_traverse.cpp"

export TRAVERSE_TEST_SOURCES="unit_tests/UPDATE.cpp unit_tests/test_traverse_main.cpp unit_tests/tracker_test_context.cpp"

export CURSOR_SOURCES="vr_applications_cursor.cpp vr_chaperone_cursor.cpp vr_chaperone_setup_cursor.cpp vr_compositor_cursor.cpp vr_cursor_context.cpp vr_cursor.cpp vr_extended_display_cursor.cpp vr_overlay_cursor.cpp vr_render_models_cursor.cpp vr_resources_cursor.cpp vr_settings_cursor.cpp vr_system_cursor.cpp vr_tracked_camera_cursor.cpp"

export CURSOR_TEST_SOURCES="unit_tests/test_cursors.cpp unit_tests/test_cursors_main.cpp"

export TBB_LIB="-L/media/sf_projects/tbb/build/linux_intel64_gcc_cc5.4.1_libc2.23_kernel4.4.0_debug -ltbb_debug"

export OPENVR_LIB="-L/media/sf_projects/openvr_clean/openvr/lib/linux64 -lopenvr_api"

export COMMON_FLAGS="-g -fpermissive -std=c++11 -DHAVE_OPEN_VR_RAW -DOPENVR_STRINGS_DISABLE_PASSTHRU"



set -x #echo on


# cursors
g++-6 -g -o test_cursors -DTEST_CURSORS_MAIN -std=c++11 $HEADERS $BASE_SOURCES $VR_BASE_SOURCES $CURSOR_SOURCES $CURSOR_TEST_SOURCES $INDEXER_SOURCES -lpthread $TBB_LIB $OPENVR_LIB $VR_STRINGS_SOURCES

# time_containers
g++-6 -g -o test_time_containers -DTEST_TIME_CONTAINERS_MAIN -std=c++11 $HEADERS $BASE_SOURCES $TIME_CONTAINER_TEST_SOURCES -lpthread 

# traverse
g++-6 $COMMON_FLAGS -o test_traverse -DTEST_TRAVERSE_MAIN $HEADERS $BASE_SOURCES $VR_BASE_SOURCES $TRAVERSE_SOURCES $TRAVERSE_TEST_SOURCES $INDEXER_SOURCES -lpthread $TBB_LIB $OPENVR_LIB $VR_STRINGS_SOURCES

# base
g++-6 -g -o test_base -DTEST_BASE_MAIN -std=c++11 $HEADERS $BASE_SOURCES $BASE_TEST_SOURCES -lpthread 

# tracker 
g++-6 $COMMON_FLAGS -o test_tracker -DTEST_VR_TRACKER_MAIN $HEADERS $BASE_SOURCES $VR_BASE_SOURCES $TRACKER_TEST_SOURCES $INDEXER_SOURCES -lpthread $TBB_LIB $OPENVR_LIB $VR_STRINGS_SOURCES

# test_vr_keys
g++-6 $COMMON_FLAGS -o test_vr_keys -DTEST_VR_KEYS_MAIN $HEADERS $BASE_SOURCES $VR_BASE_SOURCES $VR_KEYS_TEST_SOURCES $INDEXER_SOURCES -lpthread $TBB_LIB $OPENVR_LIB $VR_STRINGS_SOURCES

