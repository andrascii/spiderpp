cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_SYSTEM_VERSION 7.0 CACHE STRING INTERNAL FORCE)

include(cmake/configure_msvc_runtime.cmake)
include(cmake/add_msvc_precompiled_header.cmake)
include(cmake/cotire.cmake)

set(SPIDERPP_LIB spiderpp)
set(CURL_LIB curl)
set(COMMON_LIB common)
set(QXT_SMTP_LIB qxtsmtp)
set(QUAZIP_LIB quazip)
set(ZLIB_NAME zlib)
set(GUMBO_LIB gumbo)
set(MYHTML_LIB myhtml)
set(SEO_SPIDER_SERVICE_API_LIB seospiderserviceapi)
set(SEO_SPIDER_SERVICE seospiderservice)

project(${SPIDERPP_LIB})

set(CMAKE_INSTALL_PREFIX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED Widgets Network Xml XmlPatterns COMPONENTS)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED system date_time program_options filesystem COMPONENTS)

if(APPLE)
	find_package(OpenSSL)
	find_package(curl)
else()
	add_subdirectory(${CURL_LIB})
endif()

add_subdirectory(${SPIDERPP_LIB})
add_subdirectory(${GUMBO_LIB})
add_subdirectory(${MYHTML_LIB})
add_subdirectory(${COMMON_LIB})
