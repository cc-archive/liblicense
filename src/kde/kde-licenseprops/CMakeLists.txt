PROJECT( licenseprops )
FIND_PACKAGE(KDE4 REQUIRED)
INCLUDE_DIRECTORIES( ${KDE4_INCLUDES} )

SET(propsLiblicenseSources licenseprops.cpp)
kde4_add_ui_files(propsLiblicenseSources license_chooser.ui )

kde4_add_plugin(licenseprops ${propsLiblicenseSources})

KDE4_AUTOMOC( ${propsLiblicenseSources} )

TARGET_LINK_LIBRARIES(licenseprops license ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KPARTS_LIBS} )
 
install(TARGETS licenseprops  DESTINATION ${PLUGIN_INSTALL_DIR} )
install( FILES licenseprops.desktop  DESTINATION ${SERVICES_INSTALL_DIR}  )

add_subdirectory( icons )

