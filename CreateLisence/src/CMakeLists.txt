cmake_minimum_required(VERSION 2.8.0)
project(FQLisence)

set(CMAKE_VERBOSE_MAKEFILE ON)


############# VERSION ###################
set(MY_PRODUCT_NUMBER 1)
set(MY_PRODUCT_VERSION 0)
set(MY_BUILD_NUMBER 3)
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
 
 ADD_DEFINITIONS(-DLIBLICENSE_STATIC)

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


set(COMMON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../../include)
set(COMMON_LIB_DIR ${PROJECT_SOURCE_DIR}/../../lib/${PLATFORM}) 

############# INCLUDE DIRECTORIES #############
include_directories(${COMMON_INCLUDE_DIR})
include_directories(${COMMON_INCLUDE_DIR}/lisence)
include_directories(${COMMON_LIB_DIR})
include_directories(${PROJECT_SOURCE_DIR})


############# LINK DIRECTORIES #############
	if (MSVC)
		set(LICENSE_LIB
		optimized ${COMMON_LIB_DIR}/Release/libsodium.lib
		optimized ${COMMON_LIB_DIR}/Release/LIBLicense.lib
        debug  ${COMMON_LIB_DIR}/Debug/libsodium.lib
		debug  ${COMMON_LIB_DIR}/Debug/LIBLicense.lib
    )
	elseif (UNIX)

	 set(LICENSE_LIB 
		${COMMON_LIB_DIR}/license/${PLATFORM}/libsodium.a
		${COMMON_LIB_DIR}/license/${PLATFORM}/liblicense.a
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


############# MAIN #############
add_executable(${PROJECT_NAME} ${SOURCES_UTIL}  ${SOURCES}  ${VERSION_FILE})
target_link_libraries(${PROJECT_NAME} ${WIN_LIB} ${UNIX_LIB} ${LICENSE_LIB})
 
 if (UNIX)
	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES 
	#OUTPUT_NAME LIBLicense
	VERSION ${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER}
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin
	)
elseif (MSVC)
   	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES 
	#OUTPUT_NAME LIBLicense
	VERSION ${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin/
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin/
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin/
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin/Debug
	LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin/Debug
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin/Debug
	)
endif()
 