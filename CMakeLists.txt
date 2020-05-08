cmake_minimum_required (VERSION 3.2)
cmake_policy(SET CMP0057 NEW)

# Handling C++ standard version to use
option(SQLITE_ORM_ENABLE_CXX_17 "Enable C++ 17" OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(SQLITE_ORM_ENABLE_CXX_17)
	set(CMAKE_CXX_STANDARD 17)
	# message(STATUS "SQLITE_ORM: Build with C++17 features")
else()
	# fallback to C++14 if there is no special instruction
	set(CMAKE_CXX_STANDARD 14)
	# message(STATUS "SQLITE_ORM: Build with C++14 features")
endif()
set(CMAKE_CXX_EXTENSIONS OFF)


# PACKAGE_VERSION is used by cpack scripts currently
# Both sqlite_orm_VERSION and PACKAGE_VERSION should be the same for now

set(sqlite_orm_VERSION "1.3.0")
set(PACKAGE_VERSION ${sqlite_orm_VERSION})

project("sqlite_orm" VERSION ${PACKAGE_VERSION})

set(CMAKE_VERBOSE_MAKEFILE ON)

message(STATUS "Configuring ${PROJECT_NAME} ${sqlite_orm_VERSION}")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/build/cmake")

# Build time options are defined here
include(DefineInstallationPaths)

# Generate the sqlite_ormConfig.cmake module
include(GenerateConfigModule)

set(ProjectName "sqlite_orm")

option(SQLITE_ORM_BUILD_TESTS "Build sqlite_orm unit tests" OFF)
option(SQLITE_ORM_BIILD_EXAMPLES "Build sqlite_orm examples" OFF)

set(sqlite_orm_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
add_library(sqlite_orm INTERFACE)

target_sources(sqlite_orm INTERFACE
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/sqlite_orm/sqlite_orm.h>
	$<INSTALL_INTERFACE:include/sqlite_orm/sqlite_orm.h>)

target_include_directories(sqlite_orm INTERFACE
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)

include(ucm)

if (MSVC)
	string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
	string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	add_compile_options(/EHsc)
	add_compile_options(/Zc:__cplusplus)

	if ("${CMAKE_GENERATOR}" MATCHES "(Win64|x64)")
		message(STATUS "Add /bigobj flag to compiler")
    	add_compile_options(/bigobj)
	endif()
endif()

# ucm_print_flags()
# message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# Tests
include(CTest)
if(SQLITE_ORM_BIILD_TESTS AND BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

if(SQLITE_ORM_BIILD_EXAMPLES)
    add_subdirectory(examples)
endif()

install(TARGETS sqlite_orm EXPORT "${ProjectName}Targets"
	INCLUDES DESTINATION "${INCLUDE_INSTALL_DIR}/${ProjectName}"
	PUBLIC_HEADER DESTINATION "${INCLUDE_INSTALL_DIR}/${ProjectName}")

install(FILES "include/sqlite_orm/sqlite_orm.h"
	DESTINATION "${INCLUDE_INSTALL_DIR}/${ProjectName}")

export(EXPORT "${ProjectName}Targets"
	FILE "${CMAKE_CURRENT_BINARY_DIR}/${ProjectName}/${ProjectName}Targets.cmake"
	NAMESPACE "sqlite_orm::")

install(EXPORT "${ProjectName}Targets"
	FILE "${ProjectName}Targets.cmake"
	NAMESPACE "sqlite_orm::"
	DESTINATION "${CMAKE_INSTALL_DIR}/sqlite_orm")
