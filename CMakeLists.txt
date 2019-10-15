cmake_minimum_required(VERSION 3.14)


project(bgfx-playground VERSION 0.1
						DESCRIPTION "playground for playint with bgfx"
						LANGUAGES C CXX)
						


execute_process(
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/submodules/bx/tools/bin/windows/genie.exe vs2017
	#WORKING_DIRECTORY ./submodules/bgfx
)

add_executable(bgfx-application src/main.cpp)
target_include_directories(bgfx-application PUBLIC include)
target_compile_features(bgfx-application PUBLIC cxx_std_17)

add_subdirectory(submodules/assimp)
target_include_directories(bgfx-application PUBLIC submodules/assimp/inlcude)

target_link_libraries(bgfx-application PUBLIC assimp)
