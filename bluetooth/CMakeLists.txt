
cmake_minimum_required(VERSION 3.20)
project(bt5)

# ------------------------- 
# SETUP AVR AS COMPILER

set(CMAKE_C_COMPILER /usr/bin/avr-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/avr-gcc)

set(AVR_PROCESSOR atmega328p CACHE STRING "Board processor")
set(F_CPU 16000000 CACHE STRING "Processor frequency")
set(BAUDRATE 57600 CACHE STRING "Flash baudrate")
set(DEV_PORT /dev/ttyUSB0 CACHE STRING "Device port on host")

set(AVR_CFLAGS -Os -mmcu=${AVR_PROCESSOR} -DF_CPU=${F_CPU} CACHE STRING "C Flags")


# -------------------------
# Generate compile_commands.json

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# -------------------------
# .o FILES BUILD

add_library(objects OBJECT
    main.c
)

target_compile_options(objects PRIVATE "${AVR_CFLAGS}")

# ------------------------- 
# .elf FILE BUILD

add_custom_target(elf_file ALL
    COMMAND ${CMAKE_C_COMPILER} ${AVR_CFLAGS} $<TARGET_OBJECTS:objects> -o main.elf
    COMMENT "Producing main.elf"
)

# -------------------------
# .bin FILE BUILD

add_custom_target(${PROJECT_NAME} ALL 
    COMMAND avr-objcopy -j .text -j .data -O binary main.elf ${PROJECT_NAME}.bin
        COMMENT "Producing binary file"
    )


# -------------------------
# flash the binary
install(
    CODE
    "execute_process(COMMAND bash -c \"avrdude -b ${BAUDRATE} -c arduino -p ${AVR_PROCESSOR} -P ${DEV_PORT} -U flash:w:${PROJECT_NAME}.bin\")"
    COMPONENT Custom
)

