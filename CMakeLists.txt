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
	src/Windows/Window.h
	src/Windows/Window.cpp
)

# ========== ASSIMP SUBMODULE ==========
add_subdirectory(submodules/assimp)
target_include_directories(bgfx-application PUBLIC submodules/assimp/inlcude)
target_link_libraries(bgfx-application PUBLIC assimp)

# # ========== BGFX MODULE ==========
target_include_directories(bgfx-application PUBLIC submodules/bgfx/include)
target_include_directories(bgfx-application PUBLIC submodules/bx/include)
target_include_directories(bgfx-application PUBLIC submodules/bimg/include)

if(WIN32)
	add_library(bgfx SHARED IMPORTED)
	set_target_properties(bgfx PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/submodules/bgfx/.build/win64_vs2017/bin/bgfx-shared-libDebug.dll)
	set_target_properties(bgfx PROPERTIES IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/submodules/bgfx/.build/win64_vs2017/bin/bgfx-shared-libDebug.lib)
	target_link_libraries(bgfx-application PUBLIC bgfx)
else()
	message(FATAL_ERROR "Only Win64 platfrom is currently supported.")
endif()
