cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_SYSTEM_VERSION 7.0 CACHE STRING INTERNAL FORCE)

set(COMMON_SOURCES_DIR src)
set(COMMON_HEADERS_DIR include)

# deps headers
aux_source_directory(${COMMON_SOURCES_DIR} COMMON_SOURCES_LIST)
file(GLOB_RECURSE COMMON_HEADERS_LIST "include/*.h")

list(REMOVE_ITEM COMMON_SOURCES_LIST src/stdafx.cpp)

add_msvc_precompiled_header("stdafx.h" "src/stdafx.cpp" COMMON_SOURCES_LIST)
source_group("Precompiled Headers" FILES include/stdafx.h src/stdafx.cpp)

configure_msvc_runtime()
add_library(
	${COMMON_LIB}
	${COMMON_SOURCES_LIST}
	${COMMON_HEADERS_LIST}
)

add_definitions(
	${QT_DEFINITIONS}
	-DQXT_STATIC
	-DQUAZIP_STATIC
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
		${COMMON_LIB}
		PROPERTIES COMPILE_FLAGS "/FI\"stdafx.h\""
	)
	
	set_target_properties(
		${COMMON_LIB} 
		PROPERTIES WIN32_EXECUTABLE YES
		LINK_FLAGS "/DEBUG /ENTRY:mainCRTStartup"
	)
endif()

include_directories(
	${COMMON_HEADERS_DIR}
)

target_link_libraries(
	${COMMON_LIB}
	Qt5::Network
)