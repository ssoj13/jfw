cmake_minimum_required(VERSION 3.16)
project(jfw VERSION 0.1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)

# Use GNUInstallDirs for standard installation paths
include(GNUInstallDirs)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release'.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

######################### Dependencies
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

# Find Maya (required)
find_package(Maya 2025 REQUIRED)
if(Maya_FOUND)
    if(NOT Maya_FIND_QUIETLY)
        message(STATUS "Maya version: ${Maya_MAJOR_VERSION}.${Maya_MINOR_VERSION}.${Maya_SUBMINOR_VERSION}")
        message(STATUS "Found the following Maya libraries:")
    endif()
    foreach(COMPONENT ${Maya_FIND_COMPONENTS})
        string(TOUPPER ${COMPONENT} UPPERCOMPONENT)
        if(Maya_${UPPERCOMPONENT}_FOUND)
            if(NOT Maya_FIND_QUIETLY)
                message(STATUS "  ${COMPONENT}")
            endif()
            set(Maya_LIBRARIES ${Maya_LIBRARIES} ${Maya_${UPPERCOMPONENT}_LIBRARY})
        endif()
    endforeach()
else()
    if(Maya_FIND_REQUIRED)
        message(SEND_ERROR "Unable to find the requested Maya libraries.\n${Maya_ERROR_REASON}")
    endif()
endif()
message(STATUS "Maya executable: ${MAYA_EXECUTABLE}")
message(STATUS "Maya include dir: ${MAYA_INCLUDE_DIR}")
message(STATUS "Maya library dir: ${MAYA_LIBRARY_DIR}")
message(STATUS "Maya libs: ${MAYA_LIBRARIES}")


# Fetch simplexnoise
FetchContent_Declare(simplexnoise GIT_REPOSITORY https://github.com/ssoj13/simplexnoise.git GIT_TAG main)
FetchContent_MakeAvailable(simplexnoise)

FetchContent_Declare(fmt GIT_REPOSITORY git@github.com:fmtlib/fmt.git GIT_TAG master)
FetchContent_MakeAvailable(fmt)

# Find required packages
find_package(Threads REQUIRED)

# vcpkg packages need CONFIG mode
find_package(fmt CONFIG REQUIRED)
message(STATUS "fmt found: ${fmt_VERSION}")

find_package(glm REQUIRED)
message(STATUS "glm found: ${glm_VERSION}")


#################### Collect source files
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

# Create static library
add_library(jfw STATIC ${SOURCES} ${HEADERS})

# Set include directories
target_include_directories(jfw PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/jfw>
)

# Add Maya include directories
target_include_directories(jfw PRIVATE ${MAYA_INCLUDE_DIR})

# Link libraries
target_link_libraries(jfw PUBLIC
    simplexnoise::simplexnoise
	glm::glm-header-only
    Threads::Threads
    fmt::fmt
)

# Link Maya libraries
target_link_libraries(jfw PRIVATE ${MAYA_LIBRARIES})

# Create alias for modern CMake usage
add_library(jfw::jfw ALIAS jfw)

# Generate version header
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
    @ONLY
)

# Add generated version header to headers list
list(APPEND HEADERS "${CMAKE_CURRENT_BINARY_DIR}/version.h")

# Installation
install(TARGETS jfw
    EXPORT jfwTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install headers
install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jfw
)

# Install export targets
install(EXPORT jfwTargets
    FILE jfwTargets.cmake
    NAMESPACE jfw::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jfw
)

# Generate and install config files
include(CMakePackageConfigHelpers)

# Create the config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/jfwConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/jfwConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jfw
)

# Create the version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/jfwConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Install config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/jfwConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/jfwConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jfw
)