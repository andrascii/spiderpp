cmake_minimum_required(VERSION 2.8.11)

project(${GUMBO_LIB})

aux_source_directory(./ GUMBO_SOURCES)
file(GLOB_RECURSE GUMBO_HEADERS "./*.h")

configure_msvc_runtime()
add_library(
	${GUMBO_LIB} STATIC 
	${GUMBO_SOURCES} 
	${GUMBO_HEADERS}
)

if(MSVC)

	include_directories(include/)
	
	add_definitions(
		-Dinline=__inline
		/MP
		/permissive-
		/Zi
	)
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Oi /GL")
	set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS} /LTCG")
	
	set_target_properties(
		${GUMBO_LIB}
		PROPERTIES LINK_FLAGS /DEBUG
	)
	
endif()