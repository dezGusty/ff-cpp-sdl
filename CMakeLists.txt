cmake_minimum_required(VERSION 3.1)

project(ff_sdl)

set(CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED)
find_package(SDL_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS}
                    ${SDL_TTF_INCLUDE_DIR}

  )

add_executable(ff_sdl src/main.cpp src/ff2.cpp)
target_link_libraries(ff_sdl ${SDL2_LIBRARIES} ${SDL_TTF_LIBRARY})
