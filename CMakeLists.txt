CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

PROJECT (load_image_to_array)

SET (CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

IF(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
	message(FATAL_ERROR "In-source builds are not permitted. Make a separate folder for building:\nmkdir build; cd build; cmake ..\nBefore that, remove the files that cmake just created:\nrm -rf CMakeCache.txt CMakeFiles")
ENDIF()

INCLUDE (FindPkgConfig)

OPTION(ENABLE_OPTIMIZATIONS "Enable -O3" OFF)

#SET (CMAKE_VERBOSE_MAKEFILE ON)

SET (Boost_ADDITIONAL_VERSIONS "1.41" "1.41.0" "1.42" "1.42.0")

IF(MSVC)
	SET(Boost_USE_STATIC_LIBS ON)
ENDIF()

FIND_PACKAGE(
	Boost 1.41.0 REQUIRED COMPONENTS
	iostreams
	program_options
	system
	thread
)

INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR})

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

pkg_check_modules (SGE spacegameengine)

LINK_DIRECTORIES(${SGE_LIBRARY_DIRS})

IF (${CMAKE_COMPILER_IS_GNUCXX})
	ADD_DEFINITIONS("-fvisibility=hidden -ansi -pedantic-errors -Wall"
	"-Wextra -Wconversion -Wfloat-equal -Wredundant-decls"
	"-Wstrict-null-sentinel -Winit-self -Woverloaded-virtual"
	"-Wold-style-cast -Wnon-virtual-dtor -Wctor-dtor-privacy -Wsign-promo"
	"-Wstrict-aliasing=1 -Wmissing-declarations -Wno-long-long"
	"-ggdb"
	"-std=c++0x")

	IF (ENABLE_OPTIMIZATIONS)
		ADD_DEFINITIONS("-O3")
	ENDIF()
	# we don't use long long but boost does :(

	ADD_DEFINITIONS(-D BOOST_FILESYSTEM_NO_DEPRECATED)
	ADD_DEFINITIONS(-D BOOST_HAS_GCC_TR1)
ELSEIF (MSVC)
	ADD_DEFINITIONS ("/W3 /wd4512 /wd4355 /wd4800 /wd4996 /wd4714 /EHa /D_BIND_TO_CURRENT_VCLIBS_VERSION=1")
ENDIF ()

ADD_DEFINITIONS(${SGE_CFLAGS})

SET(media_path ${CMAKE_SOURCE_DIR}/media)

FILE(GLOB_RECURSE height_map_files height_map/*)
FILE(GLOB_RECURSE graphics_files graphics/*)
FILE(GLOB_RECURSE texture_files textures/*)
FILE(GLOB_RECURSE console_files console/*)
FILE(GLOB_RECURSE skydome_files skydome/*)
FILE(GLOB_RECURSE water_files water/*)
FILE(GLOB base_files media_path.cpp)

set_source_files_properties(
	media_path.cpp
	PROPERTIES
	COMPILE_FLAGS
	-DMEDIA_PATH="\\\"${media_path}\\\""
)

ADD_EXECUTABLE(load_image_to_array load_image_to_array.cpp media_path.cpp
	${height_map_files} ${graphics_files} ${console_files})
#
#ADD_EXECUTABLE(render_height_map media_path.cpp render_height_map.cpp
#	${texture_files}
#	${graphics_files} 
#	${height_map_files}
#	${console_files})
ADD_EXECUTABLE(render_scene media_path.cpp render_scene.cpp
	${texture_files}
	${graphics_files} 
	${height_map_files}
	${console_files}
	${skydome_files}
	${water_files}
	)
ADD_EXECUTABLE(generate_height_texture media_path.cpp generate_height_texture.cpp 
	${height_map_files} ${texture_files} ${graphics_files} ${console_files})
#ADD_EXECUTABLE(render_skydome media_path.cpp render_skydome.cpp 
#	${texture_files} ${graphics_files} ${console_files} ${skydome_files})

TARGET_LINK_LIBRARIES(load_image_to_array fcppt sgecore)
#TARGET_LINK_LIBRARIES(render_height_map fcppt sgecore)
TARGET_LINK_LIBRARIES(render_scene fcppt sgecore)
TARGET_LINK_LIBRARIES(generate_height_texture fcppt sgecore)
#TARGET_LINK_LIBRARIES(render_skydome fcppt sgecore)

IF(NOT MSVC)
	TARGET_LINK_LIBRARIES(load_image_to_array ${Boost_LIBRARIES})
	#TARGET_LINK_LIBRARIES(render_height_map ${Boost_LIBRARIES})
	TARGET_LINK_LIBRARIES(generate_height_texture ${Boost_LIBRARIES})
#	TARGET_LINK_LIBRARIES(render_skydome ${Boost_LIBRARIES})
	TARGET_LINK_LIBRARIES(render_scene ${Boost_LIBRARIES})
ENDIF()

ENABLE_TESTING()

ADD_SUBDIRECTORY(test)
