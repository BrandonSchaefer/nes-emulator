set (GTEST_BACKEND_SOURCE
   test_main.cpp
   test_cpu.cpp
)

include_directories (${NES_EMULATOR_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src)

add_executable (gtest-test-nes-emulator ${GTEST_BACKEND_SOURCE})

target_link_libraries (
    gtest-test-nes-emulator
    pthread
    nes_emulator
    ${GTEST_BOTH_LIBRARIES}
    ${GMOCK_LIBRARY}
    ${GMOCK_MAIN_LIBRARY}
)

add_custom_target (check COMMAND gtest-test-nes-emulator)

add_dependencies(gtest-test-nes-emulator GMock)
