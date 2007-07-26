PROJECT( kcmLiblicense )
FIND_PACKAGE(KDE4 REQUIRED)
INCLUDE_DIRECTORIES( ${KDE4_INCLUDES} )

SET(kcmLiblicenseSources kcm_liblicense.cpp)
kde4_add_ui_files(kcmLiblicenseSources license_chooser.ui )

kde4_add_plugin(kcm_liblicense ${kcmLiblicenseSources})

set(licenseprops_LIB_SRCS licensepropsplugin.cpp)
kde4_add_ui_files(licenseprops_LIB_SRCS license_chooser.ui )

KDE4_AUTOMOC( ${kcmLiblicenseSources} ${licenseprops_LIB_SRCS} )

TARGET_LINK_LIBRARIES(kcm_liblicense ${KDE4_KPARTS_LIBS} )
 
install(TARGETS kcm_liblicense  DESTINATION ${PLUGIN_INSTALL_DIR} )
install( FILES liblicense.desktop  DESTINATION ${SERVICES_INSTALL_DIR}  )

target_link_libraries( kcmLiblicense )

add_subdirectory( icons )

