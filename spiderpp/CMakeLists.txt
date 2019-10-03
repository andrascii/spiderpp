cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_SYSTEM_VERSION 7.0 CACHE STRING INTERNAL FORCE)
set(CMAKE_CXX_STANDARD 17)

project(${SPIDERPP_LIB})

set(SPIDERPP_SOURCES_DIR src)
set(SPIDERPP_HEADERS_DIR include)

# deps headers
set(GUMBO_LIB_HEADERS_DIR ../gumbo)
set(MYHTML_LIB_HEADERS_DIR ../myhtml/include)
set(COMMON_LIB_HEADERS_DIR ../common/include)

aux_source_directory(${SPIDERPP_SOURCES_DIR} SPIDERPP_SOURCES_LIST)
file(GLOB_RECURSE SPIDERPP_HEADERS_LIST "include/*.h")
list(REMOVE_ITEM SPIDERPP_SOURCES_LIST src/stdafx.cpp)

add_msvc_precompiled_header("stdafx.h" "src/stdafx.cpp" SPIDERPP_SOURCES_LIST)

source_group("Precompiled Headers" FILES include/stdafx.h src/stdafx.cpp)

configure_msvc_runtime()

add_library(
 	${SPIDERPP_LIB}
 	${SPIDERPP_SOURCES_LIST}
 	${SPIDERPP_HEADERS_LIST}
)

add_definitions(
	${QT_DEFINITIONS}
	-DQUAZIP_STATIC
	-D_WINSOCK_DEPRECATED_NO_WARNINGS
	-DCURL_STATICLIB
)

if(MSVC)
	add_definitions(
		/EHsc
		/MP
		/permissive-
		/std:c++latest
		/Zi
		/W4
		/WX
	)

	set_target_properties(
		${SPIDERPP_LIB}
		PROPERTIES COMPILE_FLAGS "/FI\"stdafx.h\""
		LINK_FLAGS /DEBUG
	)

endif()

message("CURL include headers directory: ${CURL_HEADERS_DIR}")

include_directories(
	${SPIDERPP_HEADERS_DIR}
	${CURL_HEADERS_DIR}
	${GUMBO_LIB_HEADERS_DIR}
	${MYHTML_LIB_HEADERS_DIR}
	${COMMON_LIB_HEADERS_DIR}
	${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(
	${SPIDERPP_LIB}
	${COMMON_LIB}
	${GUMBO_LIB}
	"${MyHtmlLibName}_static" # change this solution to that which is more reliable
	${Boost_LIBRARIES}
	${Boost_SYSTEM_LIBRARY}
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_ASIO_LIBRARY}
	${Boost_REGEX_LIBRARY}
	Qt5::Network
	Qt5::Xml
	Qt5::XmlPatterns
)

if(APPLE)
	target_link_libraries(
		${SPIDERPP_LIB}
		${CURL_LIBRARIES}
	)
else()
	add_dependencies(${SPIDERPP_LIB} ${CURL_LIB})

	target_link_libraries(
		${SPIDERPP_LIB}
		${CURL_LIBS}
		ws2_32.lib
		ntdll.lib
	)
endif()

if(NOT WIN32)
	set_target_properties(${SPIDERPP_LIB} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "include/stdafx.h")
	cotire(${SPIDERPP_LIB})
endif()
