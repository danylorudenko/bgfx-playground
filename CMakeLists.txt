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


#bgfx and it's dependecies
add_library(bgfx-interface INTERFACE)

add_library(bgfx STATIC IMPORTED)
set_target_properties(bgfx PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/submodules/bgfx/.build/win64_vs2017/bin/bgfxDebug.lib)
target_link_libraries(bgfx-interface INTERFACE bgfx)

add_library(bx STATIC IMPORTED)
set_target_properties(bx PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/submodules/bgfx/.build/win64_vs2017/bin/bxDebug.lib)
target_link_libraries(bgfx-interface INTERFACE bx)

add_library(bimg STATIC IMPORTED)
set_target_properties(bimg PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/submodules/bgfx/.build/win64_vs2017/bin/bimgDebug.lib)
target_link_libraries(bgfx-interface INTERFACE bimg)

add_library(bimg_decode STATIC IMPORTED)
set_target_properties(bimg_decode PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/submodules/bgfx/.build/win64_vs2017/bin/bimg_decodeDebug.lib)
target_link_libraries(bgfx-interface INTERFACE bimg_decode)

target_link_libraries(bgfx-application PUBLIC bgfx-interface)
