CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT (insula)

SET (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

SET (CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

IF(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
	message(FATAL_ERROR "In-source builds are not permitted. Make a separate folder for building:\nmkdir build; cd build; cmake ..\nBefore that, remove the files that cmake just created:\nrm -rf CMakeCache.txt CMakeFiles")
ENDIF()

OPTION(ENABLE_OPTIMIZATIONS "Enable -O3" ON)

#SET (CMAKE_VERBOSE_MAKEFILE ON)

SET (Boost_ADDITIONAL_VERSIONS "1.44.0" "1.44")

IF(MSVC)
	SET(Boost_USE_STATIC_LIBS ON)
ENDIF()

FIND_PACKAGE(
	Boost 1.44.0 REQUIRED COMPONENTS
	program_options
)

INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR})

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

FIND_PACKAGE(
	Fcppt REQUIRED
)

INCLUDE_DIRECTORIES(${FCPPT_INCLUDE_DIRS})

LINK_DIRECTORIES(${FCPPT_LIBRARY_DIR})

FIND_PACKAGE(
	Mizuiro REQUIRED
)

INCLUDE_DIRECTORIES(${MIZUIRO_INCLUDE_DIRS})

FIND_PACKAGE(
	SGE REQUIRED
)

INCLUDE_DIRECTORIES(${SGE_INCLUDE_DIRS})

IF (${CMAKE_COMPILER_IS_GNUCXX})
	ADD_DEFINITIONS("
		-fvisibility=hidden -std=c++0x -pedantic-errors -Wall"
		"-Wextra -Wconversion -Wfloat-equal -Wredundant-decls"
		"-Wstrict-null-sentinel -Winit-self -Woverloaded-virtual"
		"-Wold-style-cast -Wnon-virtual-dtor -Wctor-dtor-privacy -Wsign-promo"
		"-Wstrict-aliasing=1 -Wmissing-declarations"
	)

	IF (ENABLE_OPTIMIZATIONS)
		ADD_DEFINITIONS("-O3")
	ENDIF()

	ADD_DEFINITIONS(-D BOOST_HAS_GCC_TR1)
ELSEIF (MSVC)
	ADD_DEFINITIONS ("/W4 /wd4996 /EHa /D_BIND_TO_CURRENT_VCLIBS_VERSION=1")
ENDIF ()

ADD_DEFINITIONS(${SGE_CFLAGS})

# Bullet begin
FIND_PACKAGE(Bullet REQUIRED)
INCLUDE_DIRECTORIES(${BULLET_INCLUDE_DIRS})
# Bullet End

SET(media_path ${CMAKE_SOURCE_DIR}/media)

FILE(GLOB_RECURSE height_map_files height_map/*)
FILE(GLOB_RECURSE graphics_files graphics/*)
FILE(GLOB_RECURSE texture_files textures/*)
FILE(GLOB_RECURSE console_files console/*)
FILE(GLOB_RECURSE skydome_files skydome/*)
FILE(GLOB_RECURSE water_files water/*)
FILE(GLOB_RECURSE scene_files scene/*)
FILE(GLOB_RECURSE prop_files prop/*)
FILE(GLOB_RECURSE ghost_files ghost/*)
FILE(GLOB_RECURSE events_files events/*)
FILE(GLOB_RECURSE states_files states/*)
FILE(GLOB_RECURSE vehicle_files vehicle/*)
FILE(GLOB_RECURSE physics_files physics/*)
FILE(GLOB_RECURSE nugget_files nugget/*)
FILE(GLOB_RECURSE model_files model/*)
FILE(GLOB_RECURSE json_files json/*)
FILE(GLOB base_files media_path.cpp input_delegator.cpp create_path.cpp vehicle_input.cpp music_controller.cpp machine.cpp application_title.cpp sound_controller.cpp exception.cpp help_needed.cpp create_variables_map.cpp generate_nuggets.cpp random_flat_point.cpp game_cli_options.cpp random_seed.cpp scoped_machine.cpp timed_output.cpp turn_timer.cpp milliseconds_to_string.cpp model_backend.cpp model_instance.cpp static_model_instance.cpp ghost_instance.cpp parse_json_config.cpp arrow.cpp)

SET_SOURCE_FILES_PROPERTIES(
	media_path.cpp
	PROPERTIES
	COMPILE_FLAGS
	-DMEDIA_PATH="\\\"${media_path}\\\"")

ADD_LIBRARY(insula_library STATIC ${base_files} ${texture_files} ${console_files} ${skydome_files} ${water_files} ${physics_files} ${model_files} ${graphics_files} ${height_map_files} ${vehicle_files} ${states_files} ${events_files} ${json_files} ${scene_files} ${prop_files} ${ghost_files} ${nugget_files})

FUNCTION(CREATE_INSULA_EXE name)
	ADD_EXECUTABLE(${name} ${name}.cpp)
	TARGET_LINK_LIBRARIES(
		${name}
		insula_library
		${FCPPT_LIBRARIES}
		${SGE_CORE_LIBRARY}
		${Boost_LIBRARIES}
		${BULLET_LIBRARIES}
	)
ENDFUNCTION(CREATE_INSULA_EXE)

CREATE_INSULA_EXE(load_image_to_array)
CREATE_INSULA_EXE(view_model)
CREATE_INSULA_EXE(shadow_map)
CREATE_INSULA_EXE(render_height_map)
CREATE_INSULA_EXE(point_sprite_test)
#CREATE_INSULA_EXE(render_scene)
CREATE_INSULA_EXE(render_skydome)
CREATE_INSULA_EXE(generate_height_texture)
CREATE_INSULA_EXE(motor_sounds)
CREATE_INSULA_EXE(music_test)
CREATE_INSULA_EXE(sound_test)
CREATE_INSULA_EXE(insula)
CREATE_INSULA_EXE(timer_test)
CREATE_INSULA_EXE(slerp_test)
CREATE_INSULA_EXE(vertex_format_test)

ENABLE_TESTING()

ADD_SUBDIRECTORY(test)
