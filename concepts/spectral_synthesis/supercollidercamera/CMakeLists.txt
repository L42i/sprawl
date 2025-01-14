####################################################################################################
# CMakeLists file for SuperCollider Camera
# Generated by Evan Murray
# 2024-11-15
####################################################################################################

####################################################################################################
# basic project config
cmake_minimum_required(VERSION 3.12)
set(project_name "SuperCollider Camera")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 17)

####################################################################################################
# load modules
include(SuperColliderServerPlugin RESULT_VARIABLE server_plugin_found)
if(NOT server_plugin_found)
    message(FATAL_ERROR "Could not find server plugin functions module")
endif()

include(SuperColliderCompilerConfig RESULT_VARIABLE compiler_config_found)
if(NOT compiler_config_found)
    message(FATAL_ERROR "Could not find compiler config module")
endif()

# Windows - puts redistributable DLLs in install directory
include(InstallRequiredSystemLibraries)

sc_check_sc_path("${SC_PATH}")
message(STATUS "Found SuperCollider: ${SC_PATH}")
set(SC_PATH "${SC_PATH}" CACHE PATH
    "Path to SuperCollider source. Relative paths are treated as relative to this script" FORCE)

include("${SC_PATH}/SCVersion.txt")
message(STATUS "Building plugins for SuperCollider version: ${SC_VERSION}")

# set project here to avoid SCVersion.txt clobbering our version info
project(${project_name})
sc_do_initial_compiler_config() # do after setting project so compiler ID is available

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR NOT CMAKE_INSTALL_PREFIX)
    message(WARNING "No install prefix provided, defaulting to $BUILD_DIR/install")
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install prefix" FORCE)
endif()

message(STATUS "Install directory set to: ${CMAKE_INSTALL_PREFIX}")

####################################################################################################
# options
option(SUPERNOVA "Build plugins for supernova" ON)
option(SCSYNTH "Build plugins for scsynth" ON)
option(NATIVE "Optimize for native architecture" OFF)
option(STRICT "Use strict warning flags" OFF)
option(NOVA_SIMD "Build plugins with nova-simd support." ON)

####################################################################################################
# include libraries

if (NOVA_SIMD)
	add_definitions(-DNOVA_SIMD)
	include_directories(${SC_PATH}/external_libraries/nova-simd)
endif()

####################################################################################################
# Begin target Camera

set(Camera_cpp_files
    plugins/Camera/Camera.cpp
)
set(Camera_sc_files
    plugins/Camera/Camera.sc
)
set(Camera_schelp_files
    plugins/Camera/Camera.schelp
)

sc_add_server_plugin(
    "SuperColliderCamera/Camera" # desination directory
    "Camera" # target name
    "${Camera_cpp_files}"
    "${Camera_sc_files}"
    "${Camera_schelp_files}"
)

# End target Camera
####################################################################################################
# Third-party libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(OpenCV REQUIRED opencv4)
target_include_directories(Camera_supernova PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(Camera_supernova ${OpenCV_LIBRARIES})
target_include_directories(Camera_scsynth PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(Camera_scsynth ${OpenCV_LIBRARIES})

set(CMAKE_VERBOSE_MAKEFILE ON)
####################################################################################################
# END PLUGIN TARGET DEFINITION
####################################################################################################

message(STATUS "Generating plugin targets done")
