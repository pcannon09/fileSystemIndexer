# PCANNON CMAKELISTS.TXT v1.2S - FROM PCANNON PROJECT STANDARDS
# STANDARD: 20250913
# https://github.com/pcannon09/pcannonProjectStandards

cmake_minimum_required(VERSION 3.24)

# Link exec if only changed
cmake_policy(SET CMP0147 NEW)

include(CMakePackageConfigHelpers)

# Build mode configuration
option(DEV_MODE "Enable development mode" ON)
set(BUILD_MODE "Development" CACHE STRING "Build mode")

if(NOT DEV_MODE)
    set(DEV_MODE ON)
    set(BUILD_MODE "Development")
endif()

if(DEV_MODE)
    set(BUILD_MODE "Development" CACHE STRING "Build mode")
else()
    set(BUILD_MODE "Production" CACHE STRING "Build mode")
endif()

# Read metadata
file(READ ".private/project.json" projectInfo)
string(JSON projectName GET "${projectInfo}" exeName)
string(JSON projectVersion GET "${projectInfo}" version)
string(JSON projectVersionState GET "${projectInfo}" versionState)
string(JSON projectVersionSTD GET "${projectInfo}" versionSTD)

# Version configuration
set(VERSION "${projectVersion}")
set(VERSION_STATE "${projectVersionState}")
set(VERSION_STD "${projectVersionSTD}")

project(${projectName} VERSION ${projectVersion} LANGUAGES CXX C)

# Build Configuration

# Development mode flags
set(DEV_COMPILE_FLAGS
    -fsanitize=address,leak
    -g
    -Wunused
    -Wall
    -Wno-range-loop-analysis
    -DFSI_DEV=true
)

set(DEV_LINK_FLAGS
    -fsanitize=address,leak
    -g
)

# Production mode flags
set(PROD_COMPILE_FLAGS
    -O3
    -DNDEBUG
    -DFSI_DEV=false
)

set(PROD_LINK_FLAGS
    -O3
)

# Apply flags based on mode
if(DEV_MODE)
    set(PROGRAM_COMPILE_FLAGS ${DEV_COMPILE_FLAGS})
    set(PROGRAM_LINK_FLAGS ${DEV_LINK_FLAGS})
else()
    set(PROGRAM_COMPILE_FLAGS ${PROD_COMPILE_FLAGS})
    set(PROGRAM_LINK_FLAGS ${PROD_LINK_FLAGS})
endif()

# Project Configuration
set(CPP_STD 17)
set(C_STD 11)

set(CMAKE_CXX_STANDARD ${CPP_STD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD ${C_STD})
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_AUTOGEN_VERBOSE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

# Directory Setup
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(INC_DIR "${CMAKE_SOURCE_DIR}/inc/${projectName}")

# Source Files
set(HEADERS
    ${INC_DIR}/FSIpredefines.h

	${INC_DIR}/core/FSI_dirUtils.h

	${INC_DIR}/core/interfaces/FSI_dirUtils_posix.h
	${INC_DIR}/core/interfaces/FSI_dirUtils_windows.h
)

set(SOURCES
	${SRC_DIR}/main.cpp

	${SRC_DIR}/lib/FSI_Indexer.cpp

	${SRC_DIR}/core/FSI_dirUtils.c

	${SRC_DIR}/core/interfaces/FSI_dirUtils_posix.c
	${SRC_DIR}/core/interfaces/FSI_dirUtils_windows.c
)

# Build Targets
add_library(${projectName}_static STATIC ${SOURCES} ${HEADERS})
add_library(${projectName}_shared SHARED ${SOURCES} ${HEADERS})

add_subdirectory(${VENDOR_DIR}/ciof ciof)
add_subdirectory(${VENDOR_DIR}/cstr cstr)
add_subdirectory(${VENDOR_DIR}/cvec cvec)

include_directories(${VENDOR_DIR}/ciof/inc)
include_directories(${VENDOR_DIR}/cstr/inc)
include_directories(${VENDOR_DIR}/cvec/inc)

add_executable(${projectName} ${SOURCES})

function(configure_target tgt)
    target_compile_options(${tgt} PRIVATE ${PROGRAM_COMPILE_FLAGS})
    target_link_options(${tgt} PRIVATE ${PROGRAM_LINK_FLAGS})
    target_include_directories(${tgt} PRIVATE ${INC_DIR})
    target_link_libraries(${tgt} PRIVATE
    	${projectName}_static

    	ciof_static
    	cstr_static
    	cvec_static
    )
endfunction()

configure_target(${projectName})

# Output directory
set_target_properties(
    ${projectName}

    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# INSTALL / EXPORT CONFIGURATION
# Install libraries
install(TARGETS ${projectName}_static ${projectName}_shared
    EXPORT ${projectName}Target
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Install headers
install(DIRECTORY ${INC_DIR}/ ${SRC_DIR}/
    DESTINATION include/${projectName}
)

install(EXPORT ${projectName}Target
    FILE ${projectName}Target.cmake
    NAMESPACE FSI::
    DESTINATION lib/cmake/${projectName}
)

# Export package

# Generate config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_package_config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${projectName}Config.cmake"
    INSTALL_DESTINATION lib/cmake/${projectName}
)

# Install package config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${projectName}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${projectName}ConfigVersion.cmake"
    DESTINATION lib/cmake/${projectName}
)

# UNINSTALL
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    @ONLY
)

add_custom_target(fsi_uninstall
    COMMAND ${CMAKE_COMMAND} -P
    	${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    COMMENT "Uninstalling ${projectName} ${VERSION}-${VERSION_STATE}..."
)

# Generate a version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${projectName}ConfigVersion.cmake"
    VERSION ${VERSION}
    COMPATIBILITY AnyNewerVersion
)

# PACKAGE CONFIG
set(CPACK_PACKAGE_NAME ${projectName})
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Generated by pcannon Project Standards")
set(CPACK_PACKAGE_VENDOR "pcannon")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

include(CPack)

# BUILD INFO
message(STATUS "Project: ${projectName}")
message(STATUS "Version: ${VERSION}-${VERSION_STATE}")
message(STATUS "Standard: ${VERSION_STD}")
message(STATUS "Build Mode: ${BUILD_MODE}")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "CXX Standard: C++${CPP_STD}")

message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C Standard: C${C_STD}")

# Usage instructions
message(STATUS "Usage:")
message(STATUS "  Development build: cmake -DDEV_MODE=ON ..")
message(STATUS "  Production build:  cmake -DDEV_MODE=OFF ..")
message(STATUS "  Install:           ./compile.sh install {PATH}")
message(STATUS "  Uninstall:         ./compile.sh uninstall")

