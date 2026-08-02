cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_STANDARD 11)

set(ENVSDK $ENV{PLAYDATE_SDK_PATH})

if (NOT ${ENVSDK} STREQUAL "")
  # Convert path from Windows
  file(TO_CMAKE_PATH ${ENVSDK} SDK)
else()
  execute_process(
   COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
   COMMAND head -n 1
   COMMAND cut -c9-
   OUTPUT_VARIABLE SDK
   OUTPUT_STRIP_TRAILING_WHITESPACE
 )
endif()

if (NOT EXISTS ${SDK})
  message(FATAL_ERROR "SDK Path not found; set ENV value PLAYDATE_SDK_PATH")
  return()
endif()

find_program(
  MRBC mrbc
  HINTS "/usr/local/bin"
  DOC "mrbc is the mruby bytecode compiler")

if (NOT MRBC)
  message(FATAL_ERROR "NO MRBC found!")
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# Game Name Customization
set(PLAYDATE_GAME_NAME hello_mruby)
set(PLAYDATE_GAME_DEVICE hello_mruby_DEVICE)

# Ruby Binding Files
set(RUBY_BINDINGS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/bindings/rubybind_base.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/bindings/system.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/bindings/graphics.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/bindings/sprite.c
)

# Ruby Library - compiled in
set(RUBY_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/core.rb
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/pyrite/core.rb
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/pyrite/logger.rb
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/pyrite/inputs.rb
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/pyrite/cartridge.rb
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/pyrite/scenes.rb
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/pyrite/entities.rb
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/pyrite/timers.rb
)

set(RUBY_BYTECODE_C ${CMAKE_CURRENT_BINARY_DIR}/ruby_source.c)
add_custom_command(
  OUTPUT ${RUBY_BYTECODE_C}
  COMMAND ${MRBC} -g -B ruby_source -o ${RUBY_BYTECODE_C} ${RUBY_SOURCES}
  DEPENDS ${RUBY_SOURCES} ${MRBC}
  COMMENT "Compiling ruby to ruby_source.c"
  VERBATIM
)

# Ruby cartridge files - set up to be part of the pdx and loaded dynamic on game init
file(GLOB_RECURSE CARTRIDGE_RB CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/cartridge/*.rb)
set(CARTRIDGE_MRB "")
foreach(rb ${CARTRIDGE_RB})
  file(RELATIVE_PATH rel ${CMAKE_CURRENT_SOURCE_DIR}/src ${rb})
  string(REGEX REPLACE "\\.rb$" ".mrb" relmrb ${rel})
  set(out ${CMAKE_CURRENT_SOURCE_DIR}/Source/${relmrb})
  get_filename_component(outdir ${out} DIRECTORY)
  add_custom_command(
    OUTPUT  ${out}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${outdir}
    COMMAND ${MRBC} -g -o ${out} ${rel}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src
    DEPENDS ${rb} ${MRBC}
    COMMENT "mrbc ${rel} -> Source/${relmrb}"
    VERBATIM
  )
  list(APPEND CARTRIDGE_MRB ${out})
endforeach()
message(STATUS "cartridge rb: ${CARTRIDGE_RB}")

set_source_files_properties(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
  PROPERTIES OBJECT_DEPENDS "${CARTRIDGE_MRB}"
)

project(${PLAYDATE_GAME_NAME} C ASM)

# --- mruby --

if (TOOLCHAIN STREQUAL "armgcc")
  add_compile_options(
    -mthumb
    -mcpu=cortex-m7
    -mfloat-abi=hard
    -mfpu=fpv5-sp-d16
    -mword-relocations
    -fno-common
    -fno-exceptions
    -fno-unwind-tables
    -fno-asynchronous-unwind-tables
  )
  add_compile_definitions(__FPU_USED=1)
endif()
add_compile_definitions(
  MRB_NO_STDIO
  MRB_USE_FLOAT32
  MRB_INT32
)
add_library(mruby STATIC ${CMAKE_SOURCE_DIR}/vendor/mruby/mruby.c)
target_compile_definitions(mruby PRIVATE
  MRB_NO_STDIO
  MRB_USE_FLOAT32
  MRB_INT32
)
target_include_directories(mruby PUBLIC ${CMAKE_SOURCE_DIR}/vendor/mruby)

add_custom_target(cartridge ALL DEPENDS ${CARTRIDGE_MRB})

if (TOOLCHAIN STREQUAL "armgcc")
  add_executable(${PLAYDATE_GAME_DEVICE} src/main.c ${RUBY_BINDINGS})
  target_link_libraries(${PLAYDATE_GAME_DEVICE} mruby)
  target_link_options(${PLAYDATE_GAME_DEVICE} PRIVATE --specs=nano.specs --specs=nosys.specs)
else()
  add_library(${PLAYDATE_GAME_NAME} SHARED src/main.c ${RUBY_BINDINGS})
  target_link_libraries(${PLAYDATE_GAME_NAME} mruby)
endif()

include(${SDK}/C_API/buildsupport/playdate_game.cmake)

if (TARGET ${PLAYDATE_GAME_DEVICE})
  add_dependencies(${PLAYDATE_GAME_DEVICE} cartridge)
  target_sources(${PLAYDATE_GAME_DEVICE} PRIVATE ${RUBY_BYTECODE_C})
endif()
if (TARGET ${PLAYDATE_GAME_NAME})
  add_dependencies(${PLAYDATE_GAME_NAME} cartridge)
  target_sources(${PLAYDATE_GAME_NAME} PRIVATE ${RUBY_BYTECODE_C})
endif()
