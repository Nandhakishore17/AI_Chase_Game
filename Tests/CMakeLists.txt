set(TEST_PROJECT_NAME GameQualityTests)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

# Recommended for GoogleTest on Windows/MSVC
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(${TEST_PROJECT_NAME}
    PathfindingTests.cpp
    GameplayRulesTests.cpp
    StuckAgentDetectorTests.cpp
    SteeringRulesTests.cpp
    ValidationResultTests.cpp
    ValidationResultCollectorTests.cpp
    ValidationResultJsonWriterTests.cpp

    ../CSC8503/Pathfinding.cpp
    ../CSC8503/NavigationNode.cpp
    ../CSC8503/GameplayRules.cpp
    ../CSC8503/StuckAgentDetector.cpp
    ../CSC8503/SteeringRules.cpp
    ../CSC8503/ValidationResultCollector.cpp
    ../CSC8503/ValidationResultJsonWriter.cpp
)

target_include_directories(${TEST_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/CSC8503
    ${CMAKE_SOURCE_DIR}/NCLCoreClasses
)

target_precompile_headers(${TEST_PROJECT_NAME} PRIVATE
    <cmath>
    <iostream>
)

target_link_libraries(${TEST_PROJECT_NAME} PRIVATE
    GTest::gtest_main
    NCLCoreClasses
)

include(GoogleTest)

gtest_discover_tests(${TEST_PROJECT_NAME})