include(CMakePackageConfigHelpers)

set(PACKAGE_INCLUDE_INSTALL_DIR "${includedir}/sqlite_orm")
set(PACKAGE_CMAKE_INSTALL_DIR "${cmakedir}/sqlite_orm")

# In CYGWIN enviroment below commands does not work properly
if (NOT CYGWIN)
		configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/sqlite_ormConfig.cmake.in"
						"${CMAKE_CURRENT_BINARY_DIR}/sqlite_ormConfig.cmake"
						INSTALL_DESTINATION "${CMAKE_INSTALL_DIR}/sqlite_orm"
						PATH_VARS
						PACKAGE_INCLUDE_INSTALL_DIR
						PACKAGE_CMAKE_INSTALL_DIR
						)

		write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/sqlite_ormConfigVersion.cmake"
						VERSION ${sqlite_orm_VERSION}
						COMPATIBILITY SameMajorVersion
						)

		install(FILES "${CMAKE_CURRENT_BINARY_DIR}/sqlite_ormConfig.cmake"
						"${CMAKE_CURRENT_BINARY_DIR}/sqlite_ormConfigVersion.cmake"
						DESTINATION "${CMAKE_INSTALL_DIR}/sqlite_orm")
endif()