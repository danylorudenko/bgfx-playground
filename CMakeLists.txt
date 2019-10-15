cmake_minimum_required(VERSION 3.14)

project(bgfx-playground VERSION 0.1
						DESCRIPTION "playground for playint with bgfx"
						LANGUAGES C CXX)
						

add_executable(bgfx-application src/main.cpp)
target_include_directories(bgfx-application PUBLIC include)
target_compile_features(bgfx-application PUBLIC cxx_std_17)
