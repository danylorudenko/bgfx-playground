cmake_minimum_required(VERSION 3.14)


project(bgfx-playground VERSION 0.1
						DESCRIPTION "playground for playint with bgfx"
						LANGUAGES C CXX)
						


add_executable(bgfx-application src/main.cpp)
target_include_directories(bgfx-application PUBLIC include)
target_compile_features(bgfx-application PUBLIC cxx_std_17)

add_subdirectory(submodules/assimp)
target_include_directories(bgfx-application PUBLIC submodules/assimp/inlcude)

target_link_libraries(bgfx-application PUBLIC assimp)


target_include_directories(bgfx-application PUBLIC submodules/bgfx/include)
target_include_directories(bgfx-application PUBLIC submodules/bx/include)
target_include_directories(bgfx-application PUBLIC submodules/bimg/include)

add_library(bgfx SHARED IMPORTED)
set_target_properties(bgfx PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/submodules/bgfx/.build/win64_vs2017/bin/bgfx-shared-libDebug.dll)
set_target_properties(bgfx PROPERTIES IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/submodules/bgfx/.build/win64_vs2017/bin/bgfx-shared-libDebug.lib)

target_link_libraries(bgfx-application PUBLIC bgfx)
