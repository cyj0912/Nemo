#
# Find Assimp
#
# Try to find Assimp : Open Asset Import Library.
# This module defines the following variables:
# - ASSIMP_INCLUDE_DIRS
# - ASSIMP_LIBRARIES
# - ASSIMP_FOUND
#
# The following variables can be set as arguments for the module.
# - ASSIMP_ROOT_DIR : Root library directory of Assimp
#

# Additional modules
include(FindPackageHandleStandardArgs)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ASSIMP_ARCHITECTURE "64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set(ASSIMP_ARCHITECTURE "32")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

if (WIN32)
	# Find include files
	find_path(
		ASSIMP_INCLUDE_DIR
		NAMES assimp/scene.h
		PATHS
			$ENV{PROGRAMFILES}/include
			${ASSIMP_ROOT_DIR}/include
		DOC "The directory where assimp/scene.h resides")

	if( MSVC70 OR MSVC71 )
		set(MSVC_PREFIX "vc70")
	elseif( MSVC80 )
		set(MSVC_PREFIX "vc80")
	elseif( MSVC90 )
		set(MSVC_PREFIX "vc90")
	elseif( MSVC10 )
		set(MSVC_PREFIX "vc100")
	elseif( MSVC11 )
		set(MSVC_PREFIX "vc110")
	elseif( MSVC12 )
		set(MSVC_PREFIX "vc120")
	elseif( MSVC14 )
		set(MSVC_PREFIX "vc140")
	else()
		set(MSVC_PREFIX "vc150")
	endif()

    if(MSVC_PREFIX)
		find_path(ASSIMP_LIBRARY_DIR
			NAMES
				assimp-${MSVC_PREFIX}-mt.lib
			HINTS
				${ASSIMP_ROOT_DIR}/lib${ASSIMP_ARCHITECTURE}
		)
		
		find_library(ASSIMP_LIBRARY_RELEASE assimp-${MSVC_PREFIX}-mt.lib PATHS ${ASSIMP_LIBRARY_DIR})
		find_library(ASSIMP_LIBRARY_DEBUG assimp-${MSVC_PREFIX}-mtd.lib PATHS ${ASSIMP_LIBRARY_DIR})
		
		set(ASSIMP_LIBRARY
			optimized ${ASSIMP_LIBRARY_RELEASE}
			debug ${ASSIMP_LIBRARY_DEBUG}
		)
	endif()
else()
	# Find include files
	find_path(
		ASSIMP_INCLUDE_DIR
		NAMES assimp/scene.h
		PATHS
			/usr/include
			/usr/local/include
			/sw/include
			/opt/local/include
		DOC "The directory where assimp/scene.h resides")

	# Find library files
	find_library(
		ASSIMP_LIBRARY
		NAMES assimp
		PATHS
			/usr/lib64
			/usr/lib
			/usr/local/lib64
			/usr/local/lib
			/sw/lib
			/opt/local/lib
			${ASSIMP_ROOT_DIR}/lib
		DOC "The Assimp library")
endif()

# Handle REQUIRD argument, define *_FOUND variable
find_package_handle_standard_args(Assimp DEFAULT_MSG ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)

# Define GLFW_LIBRARIES and GLFW_INCLUDE_DIRS
if (ASSIMP_FOUND)
	set(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})
	set(ASSIMP_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIR})
endif()

# Hide some variables
mark_as_advanced(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)
