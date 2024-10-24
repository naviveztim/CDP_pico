# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/mnt/c/Users/PC/pico/pico-sdk")

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(cdp_c_plus_pico C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add source files from the `src` directory
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

# Include the main file along with all source files from the `src` directory
add_executable(cdp_c_plus_pico
    cdp_c_plus_pico.cpp  # Main file
    ${SRC_FILES}         # All source files from the 'src' directory
)
pico_set_program_name(cdp_c_plus_pico "cdp_c_plus_pico")
pico_set_program_version(cdp_c_plus_pico "0.1")

# Add the binary file
# Convert the binary file to an object file
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Classifiers.o
    COMMAND ${CMAKE_OBJCOPY} -I binary -O elf32-littlearm -B arm ${CMAKE_CURRENT_LIST_DIR}/models/Classifiers.bin ${CMAKE_CURRENT_BINARY_DIR}/Classifiers.o
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/models/Classifiers.bin
    COMMENT "Generating object file from binary model"
)

# Add the generated object file to the target
target_sources(cdp_c_plus_pico PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/Classifiers.o)

pico_enable_stdio_uart(cdp_c_plus_pico 1)
pico_enable_stdio_usb(cdp_c_plus_pico 1)

# Add the standard library to the build
target_link_libraries(cdp_c_plus_pico
        pico_stdlib)

# Include header files from the `header` directory
target_include_directories(cdp_c_plus_pico PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}          # Current directory (for main.cpp)
    ${CMAKE_CURRENT_LIST_DIR}/header   # Header files
    ${CMAKE_CURRENT_LIST_DIR}/src      # Include src for any in-source includes
    ${CMAKE_CURRENT_LIST_DIR}/..       # For common lwipopts or other standard includes, if needed
)

# Add any user requested libraries
target_link_libraries(cdp_c_plus_pico 
        hardware_spi
        hardware_i2c
        hardware_interp
        )

# Compiler optimizations for smaller size
target_compile_options(cdp_c_plus_pico PRIVATE -Os -ffunction-sections -fdata-sections)

pico_add_extra_outputs(cdp_c_plus_pico)

