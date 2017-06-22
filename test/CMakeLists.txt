set(SOURCE_FILES
           test_leetcode.cpp)
add_executable(StarTL ${SOURCE_FILES})
target_link_libraries(StarTL)

include_directories(../oms_api_sbe)