cmake_minimum_required(VERSION 3.15)

if(NOT EMSCRIPTEN)
    if(DEFINED ENV{VCPKG_ROOT})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
    elseif(DEFINED ENV{VCPKG_INSTALLATION_ROOT})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
    else()
        message(WARNING "VCPKG_ROOT environment variable is not set. Please set it to the root of your vcpkg installation.")
    endif()

    if(DEFINED ENV{VCPKG_TARGET_TRIPLET})
        set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_TARGET_TRIPLET}"
            CACHE STRING "Vcpkg target triplet")
    elseif(NOT DEFINED VCPKG_TARGET_TRIPLET)
        set(VCPKG_TARGET_TRIPLET "x64-windows"
            CACHE STRING "Vcpkg target triplet")
    endif()

    message(STATUS "Using Vcpkg toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
    message(STATUS "Using Vcpkg target triplet: ${VCPKG_TARGET_TRIPLET}")

    set(VCPKG_FEATURE_FLAGS "manifests")
else()
    message(STATUS "Emscripten detected, skipping vcpkg configuration.")
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "${EMSCRIPTEN}upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" CACHE STRING "Emscripten toolchain file")
        message(STATUS "Using Emscripten toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
    endif()
endif()

project(PlantTournament VERSION 1.0.0 LANGUAGES CXX)

if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.c)
file(GLOB_RECURSE HEADERS head/*.hpp head/*.h)
if(ANDROID)
    add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
    set(TARGET_NAME ${PROJECT_NAME})
else()
    if(WIN32)
        add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} resources.rc)
    else()
        add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
    endif()
    set(TARGET_NAME ${PROJECT_NAME})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE head)

if(ANDROID)
    target_link_libraries(${PROJECT_NAME} PRIVATE log android EGL GLESv2 OpenSLES m)
elseif(NOT EMSCRIPTEN)
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_image CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
    find_package(SDL2_mixer CONFIG REQUIRED)
    find_package(nlohmann_json CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
    target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)
    target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        "-sUSE_SDL=2"
        "-sUSE_SDL_IMAGE=2"
        "-sUSE_SDL_TTF=2"
        "-sUSE_SDL_MIXER=2"
    )
    include(FetchContent)
    FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    )
    FetchContent_MakeAvailable(nlohmann_json)
    target_link_options(${PROJECT_NAME} PRIVATE
    "-sUSE_SDL=2"
    "-sUSE_SDL_IMAGE=2" "-sSDL2_IMAGE_FORMATS=['png','jpg']"
    "-sUSE_SDL_TTF=2"
    "-sUSE_SDL_MIXER=2" "-sSDL2_MIXER_FORMATS=['mp3','ogg','wav']"
    "-sALLOW_MEMORY_GROWTH=1"
    "-sFORCE_FILESYSTEM=1"
    "--preload-file" "${CMAKE_CURRENT_SOURCE_DIR}/res@/res"
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE
     $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
     $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
     $<$<CONFIG:Debug>:/ENTRY:mainCRTStartup>
    )
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()

if(NOT EMSCRIPTEN)
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/res" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()