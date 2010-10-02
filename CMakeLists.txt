CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT (insula)

SET (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

SET (CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

IF(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
	message(FATAL_ERROR "In-source builds are not permitted. Make a separate folder for building:\nmkdir build; cd build; cmake ..\nBefore that, remove the files that cmake just created:\nrm -rf CMakeCache.txt CMakeFiles")
ENDIF()

OPTION(ENABLE_OPTIMIZATIONS "Enable -O3" ON)
SET(STATECHART_DIR CACHE FILEPATH "Location of the statechart library")
INCLUDE_DIRECTORIES(SYSTEM ${STATECHART_DIR})

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

SET(height_map_files height_map/random_point.cpp
height_map/image_to_array.cpp
height_map/cli_options.cpp
height_map/cli_factory.cpp
height_map/object.cpp
height_map/array_to_image.cpp
height_map/height_for_point.cpp)
SET(graphics_files graphics/shader/sampler.cpp
graphics/shader/object.cpp
graphics/shader/scoped.cpp
graphics/shader/variable.cpp
graphics/stats.cpp
graphics/cli_options.cpp
graphics/camera/pan.cpp
graphics/camera/cli_options.cpp
graphics/camera/cli_factory.cpp
graphics/camera/object.cpp
graphics/camera/scoped.cpp)
SET(texture_files textures/interpolators/bernstein_polynomial.cpp
textures/weights.cpp
textures/blend.cpp) 
SET(console_files console/streambuf.cpp
console/object.cpp)
SET(skydome_files skydome/cli_options.cpp
skydome/cli_factory.cpp
skydome/object.cpp
skydome/sphere_point.cpp
skydome/color_to_vec3.cpp
skydome/vec3_to_color.cpp)
SET(water_files water/cli_options.cpp
water/cli_factory.cpp
water/object.cpp)
SET(scene_files scene/transparent_instance.cpp
scene/instance.cpp
scene/backend.cpp
scene/manager.cpp
scene/scoped_backend.cpp)
SET(prop_files prop/instance.cpp
prop/manager.cpp)
SET(ghost_files ghost/instance.cpp
ghost/manager.cpp)
SET(events_files events/tick.cpp
events/key_repeat.cpp
events/render.cpp
events/key.cpp)
SET(states_files states/camera_move.cpp
states/game_inner.cpp
states/freelook.cpp
states/gameover.cpp
states/running.cpp
states/game_outer.cpp
states/finished.cpp
states/pregame.cpp)
SET(player_files player/object.cpp)
SET(physics_files physics/transform_from_vec3.cpp
physics/shape_from_approximation.cpp
physics/bullet_to_mat3.cpp
physics/scoped_constraint.cpp
physics/dim3_to_bullet.cpp
physics/character_controller.cpp
physics/height_map.cpp
physics/bullet_to_vec3.cpp
physics/vec3_to_bullet.cpp
physics/world.cpp
physics/model_approximation.cpp
physics/broadphase/manager.cpp
physics/broadphase/culler.cpp
physics/scoped_action.cpp
physics/shape_from_model.cpp
physics/ghost.cpp
physics/filter_callback.cpp
physics/debug_drawer.cpp
physics/cli_options.cpp
physics/object.cpp
physics/triangle_mesh/shape.cpp
physics/triangle_mesh/shape_from_model.cpp
physics/transform_to_gizmo.cpp
physics/rigid/object.cpp
physics/motion_state.cpp
physics/bullet_to_mat4.cpp
physics/mat3_to_bullet.cpp
physics/upright_constraint.cpp
physics/transform_to_mat4.cpp
physics/scoped_body.cpp)
SET(projectile_files projectile/manager.cpp)
SET(nugget_files nugget/manager.cpp)
SET(model_files model/instance.cpp
model/backend.cpp
model/object.cpp
model/scoped.cpp)
SET(json_files json/parse_config.cpp
json/parse_font.cpp)
SET(base_files media_path.cpp input_delegator.cpp create_path.cpp music_controller.cpp machine.cpp application_title.cpp sound_controller.cpp exception.cpp help_needed.cpp create_variables_map.cpp game_cli_options.cpp random_seed.cpp scoped_machine.cpp timed_output.cpp turn_timer.cpp milliseconds_to_string.cpp rigid_model.cpp arrow.cpp)

SET_SOURCE_FILES_PROPERTIES(
	media_path.cpp
	PROPERTIES
	COMPILE_FLAGS
	-DMEDIA_PATH="\\\"${media_path}\\\"")

ADD_LIBRARY(insula_library STATIC ${base_files} ${texture_files} ${console_files} ${skydome_files} ${water_files} ${physics_files} ${model_files} ${graphics_files} ${height_map_files} ${states_files} ${events_files} ${json_files} ${scene_files} ${prop_files} ${ghost_files} ${player_files} ${projectile_files} ${nugget_files})

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
