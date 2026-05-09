cmake_minimum_required(VERSION 3.20)
project(SkyRoofUnixC VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(SKYROOF_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(SKYROOF_BUILD_TESTS "Build tests" ON)

include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(SOAPYSDR QUIET SoapySDR)
  pkg_check_modules(LIQUID QUIET liquid)
endif()

find_package(FFTW3f QUIET)

add_library(skyroof_core STATIC
  src/SkyRoofCore.c
  src/SampleBuffer.c
  src/CatControl.c
  src/Satellite.c
)

target_include_directories(skyroof_core
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(FFTW3F_FOUND)
  target_compile_definitions(skyroof_core PUBLIC SKYROOF_HAS_FFTW3F=1)
endif()

if(SOAPYSDR_FOUND)
  target_compile_definitions(skyroof_core PUBLIC SKYROOF_HAS_SOAPYSDR=1)
endif()

if(LIQUID_FOUND)
  target_compile_definitions(skyroof_core PUBLIC SKYROOF_HAS_LIQUID=1)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(skyroof_core PRIVATE -Wall -Wextra -Wpedantic)
  if(SKYROOF_WARNINGS_AS_ERRORS)
    target_compile_options(skyroof_core PRIVATE -Werror)
  endif()
endif()

add_library(skyroof_unix_adapter STATIC
  adapters/unix/UnixAudioStub.c
)
target_link_libraries(skyroof_unix_adapter PUBLIC skyroof_core)
target_include_directories(skyroof_unix_adapter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(skyroof_cli
  apps/skyroof_cli.c
)
target_link_libraries(skyroof_cli PRIVATE skyroof_core skyroof_unix_adapter m)

enable_testing()
if(SKYROOF_BUILD_TESTS)
  add_executable(test_sample_buffer tests/test_sample_buffer.c)
  target_link_libraries(test_sample_buffer PRIVATE skyroof_core)

  add_executable(test_cat tests/test_cat.c)
  target_link_libraries(test_cat PRIVATE skyroof_core)

  add_executable(test_satellite tests/test_satellite.c)
  target_link_libraries(test_satellite PRIVATE skyroof_core m)

  add_test(NAME sample_buffer COMMAND test_sample_buffer)
  add_test(NAME cat_parser COMMAND test_cat)
  add_test(NAME satellite_math COMMAND test_satellite)
endif()

install(TARGETS skyroof_core skyroof_unix_adapter skyroof_cli
  EXPORT SkyRoofUnixCTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
