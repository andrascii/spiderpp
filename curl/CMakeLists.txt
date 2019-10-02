cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_SYSTEM_VERSION 7.0 CACHE STRING INTERNAL FORCE)

include(ExternalProject)

externalproject_add(curl
	URL ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/curl-7.62.0.tar.gz
	PREFIX curl
	CMAKE_ARGS -DCURL_STATICLIB=ON -DCMAKE_DEBUG_POSTFIX= -DBUILD_SHARED_LIBS=OFF -DBUILD_CURL_EXE=OFF -DBUILD_TESTING=OFF -DCMAKE_USE_OPENSSL=ON -DCURL_ZLIB=OFF -DHTTP_ONLY=ON
	INSTALL_COMMAND ""
	UPDATE_COMMAND ""
)

externalproject_get_property(curl Source_Dir Binary_Dir)

set(CURL_HEADERS_DIR "${Source_Dir}/include" CACHE INTERNAL "")
include_directories(${CURL_HEADERS_DIR})

list(APPEND CURL_LIBRARIES_LIST wsock32 ws2_32)
set(CURL_DEPENDENCY curl CACHE INTERNAL "")

set(CURL_DEBUG_DIR ${Binary_Dir}/lib/debug)
set(CURL_RELEASE_DIR ${Binary_Dir}/lib/release)
message(STATUS "CURL Debug directory ${CURL_DEBUG_DIR}")
message(STATUS "CURL Release directory ${CURL_RELEASE_DIR}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# in Debug mode
	set(CURL_LIBS ${CURL_LIBRARIES_LIST} ${CURL_DEBUG_DIR}/libcurl.lib CACHE INTERNAL "")
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# in Release mode
	set(CURL_LIBS ${CURL_LIBRARIES_LIST} ${CURL_RELEASE_DIR}/libcurl.lib CACHE INTERNAL "")
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

add_definitions(-DCURL_STATICLIB)