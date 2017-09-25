cmake_minimum_required(VERSION 2.8.0)
project(LIBLicense)

set(CMAKE_VERBOSE_MAKEFILE ON)


############# VERSION ###################
set(MY_PRODUCT_NUMBER 1)
set(MY_PRODUCT_VERSION 0)
set(MY_BUILD_NUMBER 1)
set(MY_INTERNAL_BUILD_NUMBER 1)

configure_file(
  ${PROJECT_SOURCE_DIR}/version.h.in
  ${PROJECT_SOURCE_DIR}/version.h
  @ONLY)

IF (MSVC)
 	set(VERSION_FILE ${PROJECT_SOURCE_DIR}/version.h ${PROJECT_SOURCE_DIR}/version.rc)
ELSEIF (UNIX)
 	set(VERSION_FILE ${PROJECT_SOURCE_DIR}/version.h)
ENDIF()


############# BASIC SETTINGS #############

if (UNIX)
    add_definitions(-std=c++11)
	add_definitions(-Wwrite-strings)
	set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
	set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
 
 ##### 看情况
 ADD_DEFINITIONS(-DLIBLICENSE_STATIC)
 ADD_DEFINITIONS(-DSODIUM_STATIC)


############# BASIC DIRECTORIES #############


	
IF (MSVC)
	#SET(PLATFORM win2015)
	
	IF (MSVC_VERSION EQUAL 1900)
		set(PLATFORM win2015)
	ELSE()
		SET(PLATFORM win)
	ENDIF()

elseif (UNIX)
	SET(PLATFORM linux)
endif()


set(COMMON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../../../include)
set(COMMON_LIB_DIR ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}) 

set(LISENCE_INCLUDE_DIR ${COMMON_INCLUDE_DIR}/lisence)

############# INCLUDE DIRECTORIES #############
include_directories(${COMMON_INCLUDE_DIR})
include_directories(${COMMON_INCLUDE_DIR}/libsodium)
include_directories(${COMMON_LIB_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${LISENCE_INCLUDE_DIR})

############# LINK DIRECTORIES #############
	if (MSVC)
		set(LICENSE_LIB
		optimized ${COMMON_LIB_DIR}/Release/libsodium.lib
        debug  ${COMMON_LIB_DIR}/Debug/libsodium.lib
    )
	elseif (UNIX)

	 set(LICENSE_LIB 
		${COMMON_LIB_DIR}/license/${PLATFORM}/libsodium.a
		)
	endif()
	
	
if (UNIX)
	set(UNIX_LIB pthread rt crypto ssl)
elseif (MSVC)
    set(WIN_LIB ws2_32 dbghelp)
endif()

############# BOOST SETTINGS #############


############# ADD SOURCES #############

file(GLOB SOURCES
    "*.h"
    "*.cpp"
)
set(SOURCES_UTIL 
			${COMMON_INCLUDE_DIR}/lisence/LicDefine.h
			${COMMON_INCLUDE_DIR}/lisence/license.h
					
)

################################


############# MAIN LIBLicense #############
#add_executable(${PROJECT_NAME} ${SOURCES_UTIL}  ${SOURCES}  ${VERSION_FILE})

#ADD_LIBRARY(${PROJECT_NAME} STATIC ${SOURCES_UTIL}  ${SOURCES}  ${VERSION_FILE})
#target_link_libraries(${PROJECT_NAME} ${WIN_LIB} ${UNIX_LIB} ${LICENSE_LIB})

  
ADD_LIBRARY(LIBLicense_static STATIC ${SOURCES_UTIL}  ${SOURCES}  ${VERSION_FILE})
target_link_libraries(LIBLicense_static ${WIN_LIB} ${UNIX_LIB} ${LICENSE_LIB})

if (UNIX)
	SET_TARGET_PROPERTIES(LIBLicense_static PROPERTIES 
	OUTPUT_NAME LIBLicense
	VERSION ${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER}
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}
	)
elseif (MSVC)
	SET_TARGET_PROPERTIES(LIBLicense_static PROPERTIES 
	OUTPUT_NAME LIBLicense
	VERSION ${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/Release
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/Release
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/Release
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/Debug
	LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/Debug
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/Debug
	)
endif()


  
ADD_LIBRARY(LIBLicense_dynamic SHARED ${SOURCES_UTIL}  ${SOURCES}  ${VERSION_FILE})
target_link_libraries(LIBLicense_dynamic ${WIN_LIB} ${UNIX_LIB} ${LICENSE_LIB})

if (UNIX)
	SET_TARGET_PROPERTIES(LIBLicense_dynamic PROPERTIES 
	OUTPUT_NAME LIBLicense
	VERSION ${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER}
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/dynamic
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/dynamic
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/dynamic
	)
elseif (MSVC)
   	SET_TARGET_PROPERTIES(LIBLicense_dynamic PROPERTIES 
	OUTPUT_NAME LIBLicense
	VERSION ${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/Release/dynamic
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/Release/dynamic
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/Release/dynamic
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/Debug/dynamic
	LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/Debug/dynamic
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../../lib/${PLATFORM}/Debug/dynamic
	)
endif()



   