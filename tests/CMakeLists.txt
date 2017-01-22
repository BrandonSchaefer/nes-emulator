set (GTEST_BACKEND_SOURCE
   test_main.cpp
   test_cpu.cpp
   test_cpu_instructions.cpp
)

include_directories (${NES_EMULATOR_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src)

add_executable (nes-test-emulator ${GTEST_BACKEND_SOURCE})

target_link_libraries (
    nes-test-emulator
    pthread
    nes_emulator
    ${GTEST_BOTH_LIBRARIES}
    ${GMOCK_LIBRARY}
    ${GMOCK_MAIN_LIBRARY}
)

add_custom_target (check COMMAND nes-test-emulator)

add_dependencies(nes-test-emulator GMock)
