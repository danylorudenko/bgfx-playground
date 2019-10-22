cmake_minimum_required(VERSION 3.14)


project(bgfx-playground VERSION 0.1
						DESCRIPTION "playground for playing with bgfx"
						LANGUAGES C CXX)
						

# ========== MAIN APPLICATION ==========
add_executable(bgfx-application)
target_include_directories(bgfx-application PUBLIC include src)
target_compile_features(bgfx-application PUBLIC cxx_std_17)
target_sources(bgfx-application PUBLIC
	src/main.cpp
	src/globals.h
	src/globals.cpp
	src/entity.h
	src/entity.cpp
	src/io_helpers.h
	src/io_helpers.cpp
	src/struct_helpers.h
	src/struct_helpers.cpp
	src/windows/window.h
	src/windows/window.cpp
)


# ========== ASSIMP SUBMODULE ==========
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
	
	set_property(TARGET bgfx-application PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/workdir")
else()
	message(FATAL_ERROR "Only Win64 platfrom is currently supported.")
endif()


# =========== GLM SUBMODULE =========
set(GLM_TEST_ENABLE_CXX_17 ON CACHE BOOL "C++17 is a MUST nowadays")
set(GLM_TEST_ENABLE OFF CACHE BOOL "There are too many test projects, makes it too laggy.")
add_subdirectory(submodules/glm)
target_include_directories(bgfx-application PUBLIC submodules/glm)
target_link_libraries(bgfx-application PUBLIC glm_static)



