# SPDX-License-Identifier: Unlicense

cmake_minimum_required(VERSION 3.22)

# Opt into the new install-destination normalization behavior
if(POLICY CMP0177)
    cmake_policy(SET CMP0177 NEW)
endif()

message(STATUS "Using CMake ${CMAKE_VERSION}")

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in the source directory. Please use a build subdirectory.")
endif()

# Add custom CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# Enable LTO for release if supported
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)
if(ipo_supported)
    message(STATUS "IPO/LTO supported: enabling interprocedural optimization")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO/LTO not supported: skipping LTO flags (${ipo_output})")
endif()

# Output compile commands for editor integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build universal library on macOS
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "")
endif()

# Project definition
project(SteamAudioGodot
        VERSION 0.1.0
        LANGUAGES CXX
)

# Create the shared library target
add_library(${PROJECT_NAME} SHARED)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Determine architecture label
set(LIB_ARCH ${CMAKE_SYSTEM_PROCESSOR})
if(APPLE)
    set(LIB_ARCH "universal")
endif()

# Where Godot expects library binaries
set(LIB_DIR "lib/${CMAKE_SYSTEM_NAME}-${LIB_ARCH}")
message(STATUS "Building ${PROJECT_NAME} for ${LIB_ARCH} on ${CMAKE_SYSTEM_NAME}")

# Output directories for built library
set(BUILD_OUTPUT_DIR "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/")
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/${LIB_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/${LIB_DIR}"
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN TRUE
)

# Append debug postfix if needed
if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "-d")
endif()

# (Optional) Copy support_files icons and templates next to build output
# if you need them in your local addon for testing. Exclude the 'lib' folder to avoid nested structure.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/support_files/icons"
        "${BUILD_OUTPUT_DIR}/icons"
)

# Build source files
add_subdirectory(src)

# ─────────────────────────────────────────────────────────────────────────────
# Stage Steam Audio runtimes alongside the built GDExtension
# ─────────────────────────────────────────────────────────────────────────────

# Detect platform-specific subdirectories
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_SA_RUNTIME_SUBDIR "windows-x64")
        set(_SA_GDE_DIR       "Windows-AMD64")
    else()
        set(_SA_RUNTIME_SUBDIR "windows-x86")
        set(_SA_GDE_DIR       "Windows-386")
    endif()
elseif(APPLE)
    set(_SA_RUNTIME_SUBDIR "ios")    # switch to "osx" if targeting desktop
    set(_SA_GDE_DIR       "Darwin")
elseif(ANDROID)
    set(_SA_RUNTIME_SUBDIR "${ANDROID_ABI}")
    set(_SA_GDE_DIR       "${ANDROID_ABI}")
elseif(UNIX)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_SA_RUNTIME_SUBDIR "linux-x64")
        set(_SA_GDE_DIR       "Linux-x64")
    else()
        set(_SA_RUNTIME_SUBDIR "linux-x86")
        set(_SA_GDE_DIR       "Linux-x86")
    endif()
else()
    message(FATAL_ERROR "Unsupported platform for SteamAudioGodot")
endif()

# Path to prebuilt Steam Audio runtime files
set(_SA_RUNTIME_DIR "${CMAKE_CURRENT_SOURCE_DIR}/support_files/lib/${_SA_RUNTIME_SUBDIR}")
# Where Godot will load the binaries (target output directory already equals build_output/lib/Platform)
set(_SA_OUTPUT_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>")

# Copy the runtime files after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${_SA_OUTPUT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${_SA_RUNTIME_DIR}"
        "${_SA_OUTPUT_DIR}"
)

# Include paths for Steam Audio SDK and godot-cpp
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/steam-audio/include
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-cpp/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Installation setup
set(INSTALL_DIR "${CMAKE_SOURCE_DIR}/install/${PROJECT_NAME}/")
message(STATUS "Install directory: ${INSTALL_DIR}")
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${INSTALL_DIR}/${LIB_DIR}
        LIBRARY DESTINATION ${INSTALL_DIR}/${LIB_DIR}
)
# Install Steam Audio runtime libraries into the addon install folder
install(
        DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/support_files/lib/${_SA_RUNTIME_SUBDIR}/"
        DESTINATION "${INSTALL_DIR}/${LIB_DIR}/"
        FILES_MATCHING
        PATTERN "*.dll"
        PATTERN "*.so"
        PATTERN "*.dylib"
        PATTERN "*.a"
)

# Templates subdir
add_subdirectory(templates)

# ccache integration
include(ccache)

# Install Steam Audio runtime libraries into the addon install folder
install(
        DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/support_files/lib/${_SA_RUNTIME_SUBDIR}/"
        DESTINATION "${INSTALL_DIR}/${LIB_DIR}/"
        FILES_MATCHING
        PATTERN "*.dll"
        PATTERN "*.so"
        PATTERN "*.dylib"
        PATTERN "*.a"
)

# Code formatting
include(ClangFormat)
include(ccache)
# Code formatting
include(ClangFormat)

# godot-cpp bindings
if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/extern/godot-cpp/Makefile")
    message(FATAL_ERROR "[${PROJECT_NAME}] Missing godot-cpp submodule. Run: git submodule update --init --recursive.")
endif()
set(GODOT_CPP_SYSTEM_HEADERS ON CACHE BOOL "" FORCE)
add_subdirectory(extern/godot-cpp)
set_target_properties(godot-cpp PROPERTIES CXX_VISIBILITY_PRESET hidden)

# Link against godot-cpp and Steam Audio phonon library
# Add Steam Audio phonon lib search path
target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/steam-audio/lib/${_SA_RUNTIME_SUBDIR}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        godot-cpp
        phonon
)
