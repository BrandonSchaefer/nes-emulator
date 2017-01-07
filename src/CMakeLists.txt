find_package (PkgConfig)

set (NES_EMULATOR_REQUIRED
    sdl2
)

pkg_check_modules(NES_EMULATOR REQUIRED ${NES_EMULATOR_REQUIRED})

set (NES_EMULATOR_LOADER_SRC
     main.c
)

set (NES_EMULATOR_LOADER_HDR
)

include_directories (${NES_EMULATOR_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

add_executable (nes_emulator ${NES_EMULATOR_LOADER_SRC} ${NES_EMULATOR_LOADER_HDR})

target_link_libraries (nes_emulator ${NES_EMULATOR_LIBRARIES} ${NES_EMULATOR_LDFLAGS})
