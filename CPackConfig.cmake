set(CPACK_GENERATOR "ZIP")  # Set the generator to create a ZIP file
set(CPACK_PACKAGE_NAME "Disaster World")  # Set the name of the package
set(CPACK_PACKAGE_VERSION "0.0.1")  # Set the version of the package
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "")  # Set a brief description of the package
set(CPACK_PACKAGE_VENDOR "PyXiion")  # Set the name of the package vendor

# Set the install directory to "MyProject-1.0.0"
set(CPACK_SET_DESTDIR TRUE)
set(CPACK_INSTALL_PREFIX "/${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")

# Add the "run" and "resource" directories to the package
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources DESTINATION .)
install(TARGETS DisasterWorld RUNTIME DESTINATION .)

include(CPack)