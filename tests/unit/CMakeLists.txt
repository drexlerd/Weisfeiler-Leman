enable_testing()

find_package(GTest "1.11.0" REQUIRED COMPONENTS GTest Main PATHS ${CMAKE_PREFIX_PATH} NO_DEFAULT_PATH)
# Set result variables
find_package(GTest)

file(GLOB_RECURSE WL_TEST_SOURCE_FILES
    "*.cpp" "**/*.cpp")
file(GLOB_RECURSE WL_TEST_HEADER_FILES
    "*.hpp" "**/*.hpp")

set(TEST_NAME ${CMAKE_PROJECT_NAME}_tests)

# add_executable(${TEST_NAME} ${WL_TEST_SOURCE_FILES} ${WL_TEST_HEADER_FILES})

add_executable(${TEST_NAME}
    "canonical_color_refinement.cpp"
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        wl::core
        GTest::GTest
        GTest::Main)
target_link_options(${TEST_NAME} PRIVATE -static-libstdc++)
add_test(wl_gtests ${TEST_NAME})

