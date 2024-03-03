cmake_minimum_required( VERSION 3.14 )

project( fugio-portmidi-api LANGUAGES CXX )

install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-portmidi/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )

set( CPACK_ARCHIVE_FUGIO-PORTMIDI-API_FILE_NAME "fugio-portmidi-api" PARENT_SCOPE )

