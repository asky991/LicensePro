cmake_minimum_required(VERSION 2.8.0)
project(FQLisence)

set(CMAKE_VERBOSE_MAKEFILE ON)


############# VERSION ###################
set(MY_PRODUCT_NUMBER 1)
set(MY_PRODUCT_VERSION 0)
set(MY_BUILD_NUMBER 1)
set(MY_INTERNAL_BUILD_NUMBER 3)

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
	SET(PLATFORM win2015)
elseif (UNIX)
	SET(PLATFORM linux)
endif()


set(COMMON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../include)
set(COMMON_LIB_DIR ${PROJECT_SOURCE_DIR}/../lib/${PLATFORM}) 

############# INCLUDE DIRECTORIES #############
include_directories(${COMMON_INCLUDE_DIR})
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
			${COMMON_INCLUDE_DIR}/LicDefine.h
			${COMMON_INCLUDE_DIR}/license.h
					
)

################################


############# MAIN #############
add_executable(${PROJECT_NAME} ${SOURCES_UTIL}  ${SOURCES}  ${VERSION_FILE})
target_link_libraries(${PROJECT_NAME} ${WIN_LIB} ${UNIX_LIB} ${LICENSE_LIB})
   