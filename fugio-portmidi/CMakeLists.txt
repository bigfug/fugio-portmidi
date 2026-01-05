cmake_minimum_required( VERSION 3.14 )

#-------------------------------------------------------------------------------
# PLUGIN DEFINES

set( PLUGIN_NAME "PortMidi" )
set( PLUGIN_DESCRIPTION_SHORT "PortMidi nodes" )
set( PLUGIN_DESCRIPTION_LONG "PortMidi nodes" )
set( PLUGIN_FILENAME "fugio-portmidi" )

set( PLUGIN_UUID "71d15bdb-166d-49bf-9725-16d42ccee2bd" )
set( PLUGIN_IID "com.bigfug.fugio.portmidi" )

set( PLUGIN_RELEASE_VERSION "1.0.0" )
set( PLUGIN_COMPATIBILITY_VERSION "1.0.0" )
set( PLUGIN_API_COMPATIBILITY "4.0.0" )

set( PLUGIN_RELEASE_DATE "2019-10-25" )
set( PLUGIN_AUTHOR_NAME  "Alex May" )
set( PLUGIN_AUTHOR_CONTACT "https://github.com/bigfug" )

set( PLUGIN_WIKI_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/PortMidi_Plugin" )
set( PLUGIN_WIKI_NODE_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/PortMidi_Node_" )
set( PLUGIN_WIKI_PIN_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/PortMidi_Pin_" )

set( PLUGIN_REQUIREMENTS "\
	{ \"plugin\": \"com.bigfug.fugio.core\", \"version\": \"1.0.0\" }"
)

#-------------------------------------------------------------------------------

project( ${PLUGIN_FILENAME} VERSION ${PLUGIN_RELEASE_VERSION} LANGUAGES CXX )

set( HEADER_FILES
	include/fugio/portmidi/uuid.h
)

set( SOURCE_FILES
	source/portmidiplugin.cpp
	source/portmidiplugin.h

	source/devicemidi.cpp
	source/devicemidi.h
	source/portmidiinputnode.cpp
	source/portmidiinputnode.h
	source/portmidioutputnode.cpp
	source/portmidioutputnode.h
)

set( QRC_FILES
#	resources.qrc
)

add_library( ${PROJECT_NAME} MODULE ${SOURCE_FILES} ${HEADER_FILES} ${QRC_FILES} )

set_target_properties( ${PROJECT_NAME} PROPERTIES
  BUNDLE TRUE
)

#------------------------------------------------------------------------------
# Includes

target_include_directories( ${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/../../fugio-core-plugins/fugio-core/include
	${CMAKE_CURRENT_SOURCE_DIR}/../../fugio-core-plugins/fugio-midi/include
)

#------------------------------------------------------------------------------
# Fugio API

if( FUGIO_API AND EXISTS ${FUGIO_API} )
	target_include_directories( ${PROJECT_NAME} PRIVATE ${FUGIO_API} )

	target_link_libraries( ${PROJECT_NAME} PUBLIC libfugio )
endif()

#------------------------------------------------------------------------------
# PortMidi

find_package(PortMidi CONFIG REQUIRED)

target_link_libraries( ${PROJECT_NAME} PRIVATE PortMidi::portmidi)

target_compile_definitions( ${PROJECT_NAME} PRIVATE PORTMIDI_SUPPORTED )

# if( WIN32 )
# 	target_link_libraries( ${PROJECT_NAME} PRIVATE
# 		${CMAKE_BINARY_DIR}/portmidi/lib/portmidi.lib
# 		winmm.lib
# 	)
# endif( WIN32 )

# if( APPLE )
# 	find_library( CoreMidi_LIBRARY CoreMidi )
# 	find_library( CoreFoundation_LIBRARY CoreFoundation )
# 	find_library( CoreServices_LIBRARY CoreServices )
# 	find_library( CoreAudio_LIBRARY CoreAudio )

# 	target_link_libraries( ${PROJECT_NAME}
# 		${CoreMidi_LIBRARY}
# 		${CoreFoundation_LIBRARY}
# 		${CoreServices_LIBRARY}
# 		${CoreAudio_LIBRARY}
# 		)
# endif( APPLE )

#------------------------------------------------------------------------------
# Plugin Config

configure_file(
	../config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/config.h
	@ONLY
)

target_sources( ${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/config.h
	)

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR} )

#------------------------------------------------------------------------------
# Plugin Manifest

configure_file(
	../manifest.json.in
	${CMAKE_CURRENT_BINARY_DIR}/manifest.json
	@ONLY
)

target_sources( ${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/manifest.json
	)

install( FILES ${CMAKE_CURRENT_BINARY_DIR}/manifest.json DESTINATION . COMPONENT ${PROJECT_NAME} )

#------------------------------------------------------------------------------
# Install

install( TARGETS ${PROJECT_NAME}
	LIBRARY
	DESTINATION plugins
	COMPONENT ${PROJECT_NAME}
)

install( DIRECTORY include DESTINATION . COMPONENT ${PROJECT_NAME} PATTERN ".*" EXCLUDE )
install( DIRECTORY examples	DESTINATION . COMPONENT ${PROJECT_NAME} PATTERN ".*" EXCLUDE )

#------------------------------------------------------------------------------
# Qt

find_package(Qt6 COMPONENTS Core Widgets LinguistTools QUIET )

if (NOT Qt6_FOUND)
	find_package(Qt5 5.15 COMPONENTS Core Widgets LinguistTools QUIET)
endif()

target_link_libraries( ${PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets )

#------------------------------------------------------------------------------
# Translations

set( TS_FILES
	translations/translations_de.ts
	)

if( COMMAND qt_add_translations )
	qt_add_translations( ${PROJECT_NAME} TS_FILES ${TS_FILES} RESOURCE_PREFIX "/translations" )
else()
	qt5_create_translation( QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES} )

	configure_file( ${CMAKE_SOURCE_DIR}/translations.qrc ${CMAKE_CURRENT_BINARY_DIR} COPYONLY )

	target_sources( ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc ${QM_FILES} )
endif()

#------------------------------------------------------------------------------
# Unit Tests

if( FUGIO_LIBRARY_BUILD_DIR )
#add_subdirectory( tests )
endif()

set( CPACK_ARCHIVE_FUGIO-PORTMIDI_FILE_NAME "${PLUGIN_FILENAME}-${PLATFORM_NAME}" PARENT_SCOPE )

