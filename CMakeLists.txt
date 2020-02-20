cmake_minimum_required(VERSION 3.14)


project(bgfx-playground VERSION 0.1
						DESCRIPTION "playground for playing with bgfx"
						LANGUAGES C CXX)
						

# ========== MAIN APPLICATION ==========
add_executable(bgfx-application)
target_include_directories(bgfx-application PUBLIC include src)
target_compile_features(bgfx-application PUBLIC cxx_std_17)
target_sources(bgfx-application PUBLIC
	src/main/main.cpp
	src/main/application.h
	src/main/application.cpp
	src/class_features/class_features.hpp
	src/gfx/bgfx_helpers.h
	src/gfx/bgfx_helpers.cpp
	src/gfx/gfx_def.h
	src/gfx/gfx_instancing.h
	src/gfx/gfx_instancing.cpp
	src/gfx/gfx_objects.h
	src/gfx/gfx_objects.cpp
	src/gfx/gfx_renderer.h
	src/gfx/gfx_renderer.cpp
	src/gfx/gfx_resourcemap.h
	src/gfx/gfx_resourcemap.cpp
	src/gfx/gfx_settings.h
	src/gfx/gfx_settings.cpp
	src/gfx/gfx_vertex.h
	src/gfx/pass/gfx_pass_debugview.h
	src/gfx/pass/gfx_pass_debugview.cpp
	src/gfx/pass/gfx_pass_forward.h
	src/gfx/pass/gfx_pass_forward.cpp
	src/gfx/pass/gfx_pass_schedule.h
	src/gfx/pass/gfx_pass_shadow.h
	src/gfx/pass/gfx_pass_shadow.cpp
	src/gfx/pass/gfx_passbase.h
	src/gfx/pass/gfx_passbase.cpp
	src/input/input_system.h
	src/input/input_system.cpp
	src/input/keyboard.h
	src/input/keyboard.cpp
	src/io/io_helpers.h
	src/io/io_helpers.cpp
	src/math/math_helpers.hpp
	src/math/math_helpers.cpp
	src/math/transformable.h
	src/math/transformable.cpp
	src/math/view_provider.h
	src/math/view_provider.cpp
	src/scene/camera.h
	src/scene/camera.cpp
	src/scene/component_light.h
	src/scene/component_light.cpp
	src/scene/component_renderable.h
	src/scene/component_renderable.cpp
	src/scene/entity.h
	src/scene/entity.cpp
	src/scene/scene.h
	src/scene/scene.cpp
	src/scene/scene_importer.h
	src/scene/scene_importer.cpp
	src/system/window.h
	src/system/window.cpp
)


# ========== ASSIMP SUBMODULE ==========
option(BUILD_SHARED_LIBS "This is a CMake option that defaults ADD_LIBRARY behavour" FALSE)
add_subdirectory(submodules/assimp)
target_include_directories(bgfx-application PUBLIC submodules/assimp/inlcude)
target_link_libraries(bgfx-application PUBLIC assimp)


# ========== BGFX MODULE ==========
target_include_directories(bgfx-application PUBLIC submodules/bgfx/include)
target_include_directories(bgfx-application PUBLIC submodules/bx/include)

if(WIN32)
	add_library(bgfxDebug SHARED IMPORTED)
	add_library(bgfxRelease SHARED IMPORTED)
	
	set_target_properties(bgfxDebug PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/bgfx/bgfx-shared-libDebug.dll")
	set_target_properties(bgfxDebug PROPERTIES IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/bgfx/bgfx-shared-libDebug.lib")
	
	set_target_properties(bgfxRelease PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/bgfx/bgfx-shared-libRelease.dll")
	set_target_properties(bgfxRelease PROPERTIES IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/bgfx/bgfx-shared-libRelease.lib")
	
	target_link_libraries(bgfx-application PUBLIC debug bgfxDebug)
	target_link_libraries(bgfx-application PUBLIC optimized bgfxRelease)
	
	add_custom_command(TARGET bgfx-application POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/libs/bgfx"
		"${CMAKE_CURRENT_SOURCE_DIR}/workdir"
	)
	
	add_custom_command(TARGET bgfx-application POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/assets"
		"${CMAKE_CURRENT_SOURCE_DIR}/workdir/assets"
	)
	
	set_property(TARGET bgfx-application PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/workdir")
else()
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! FATAL_ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!
	message(FATAL_ERROR "Only Win64 platfrom is currently supported.")
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! FATAL_ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!
endif()


# =========== GLM SUBMODULE =========
set(GLM_TEST_ENABLE_CXX_17 ON CACHE BOOL "C++17 is a MUST nowadays")
set(GLM_TEST_ENABLE OFF CACHE BOOL "There are too many test projects, makes it too laggy.")
add_subdirectory(submodules/glm)
target_include_directories(bgfx-application PUBLIC submodules/glm)
target_link_libraries(bgfx-application PUBLIC glm_static)



