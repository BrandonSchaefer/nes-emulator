find_package (PkgConfig)

set (NES_EMULATOR_REQUIRED
    sdl2
)

pkg_check_modules(NES_EMULATOR REQUIRED ${NES_EMULATOR_REQUIRED})

set (NES_EMULATOR_LOADER_SRC
     cpu.cpp
     cpu_instructions.cpp
     ppu.cpp
)

set (NES_EMULATOR_LOADER_HDR
     cpu.h
     cpu_instructions.h
     ppu.h
)

include_directories (${NES_EMULATOR_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

add_library (nes_emulator ${NES_EMULATOR_LOADER_SRC} ${NES_EMULATOR_LOADER_HDR})

target_link_libraries (nes_emulator ${NES_EMULATOR_LIBRARIES} ${NES_EMULATOR_LDFLAGS})

add_executable (nes main.cpp)

target_link_libraries (nes nes_emulator)
