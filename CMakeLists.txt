cmake_minimum_required (VERSION 2.6)

project (SFMLTest)

#Change the output directory to assets, this makes the working directory correct under
#xcode and using the command line under Linux or Windows to compile with GCC
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/assets)

MESSAGE(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/assets)
    message(${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG}})
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

if(WIN32)
	IF(MSVC)
		include_directories(lib/win/include)
		link_directories(${CMAKE_SOURCE_DIR}/lib/win/lib)
	ENDIF(MSVC)
ENDIF(WIN32)

IF(UNIX)
	include_directories(/usr/local/include)
	link_directories(/usr/local/lib)
	if(APPLE)
		set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
		set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
		set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	ENDIF(APPLE)
ENDIF(UNIX)

MACRO(BUILD_GROUPS curdir)
FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
FOREACH(child ${children})
	IF(IS_DIRECTORY ${curdir}/${child})
		FILE(GLOB files ${curdir}/${child}/*.h ${curdir}/${child}/*.cpp)
		SOURCE_GROUP(${child} FILES ${files})
	ENDIF()
ENDFOREACH()
ENDMACRO(BUILD_GROUPS)

subdirs(src)
