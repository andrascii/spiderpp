cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_SYSTEM_VERSION 7.0 CACHE STRING INTERNAL FORCE)

macro(configure_msvc_runtime)
  if(MSVC)
    # Default to statically-linked runtime.
    if("${MSVC_RUNTIME}" STREQUAL "")
      set(MSVC_RUNTIME "static")
    endif()
    # Set compiler options.
    set(variables
      CMAKE_C_FLAGS_DEBUG
      CMAKE_C_FLAGS_MINSIZEREL
      CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_RELWITHDEBINFO
      CMAKE_CXX_FLAGS_DEBUG
      CMAKE_CXX_FLAGS_MINSIZEREL
      CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_RELWITHDEBINFO
    )
    if(${MSVC_RUNTIME} STREQUAL "static")
      message(STATUS
        "MSVC -> forcing use of statically-linked runtime."
      )
      foreach(variable ${variables})
        if(${variable} MATCHES "/MD")
          string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
        endif()
      endforeach()
    else()
      message(STATUS
        "MSVC -> forcing use of dynamically-linked runtime."
      )
      foreach(variable ${variables})
        if(${variable} MATCHES "/MT")
          string(REGEX REPLACE "/MT" "/MD" ${variable} "${${variable}}")
        endif()
      endforeach()
    endif()
  endif()
endmacro()

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

MACRO(add_msvc_precompiled_header PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "$(IntDir)/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(
		${PrecompiledSource}
		PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
		OBJECT_OUTPUTS "${PrecompiledBinary}"
	)
	   
    SET_SOURCE_FILES_PROPERTIES(
		${Sources}
        PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
        OBJECT_DEPENDS "${PrecompiledBinary}"
	)  
		
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(add_msvc_precompiled_header)

add_subdirectory(${CURL_LIB})
add_subdirectory(${SPIDERPP_LIB})
add_subdirectory(${GUMBO_LIB})
add_subdirectory(${MYHTML_LIB})
#add_subdirectory(${SEO_SPIDER_SERVICE_API_LIB})
#add_subdirectory(${SEO_SPIDER_SERVICE})
add_subdirectory(${COMMON_LIB})
#add_subdirectory(${QXT_SMTP_LIB})
#add_subdirectory(${ZLIB_NAME})
#add_subdirectory(${QUAZIP_LIB})