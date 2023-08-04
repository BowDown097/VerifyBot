cmake_minimum_required(VERSION 3.12)
project(VerifyBot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath='$ORIGIN'")

add_executable(VerifyBot
    main.cpp
    botcommandhandler.h botcommandhandler.cpp
    botconfig.h botconfig.cpp
    commands.h commands.cpp
    constants.h
    verification.h verification.cpp
)

include(FetchContent)

# use system CURL if it exists
find_package(CURL COMPONENTS HTTP HTTPS QUIET)
if(CURL_FOUND)
    option(CPR_USE_SYSTEM_CURL "" ON)
endif()

# CPR
find_package(cpr QUIET)
if(NOT cpr_FOUND)
    set(CURL_ZLIB OFF CACHE STRING "" FORCE) # we aren't using zlib so we can turn it off
    FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 2553fc41450301cd09a9271c8d2c3e0cf3546b73)
    FetchContent_MakeAvailable(cpr)
endif()
target_link_libraries(VerifyBot cpr::cpr)

# DPP
find_package(DPP QUIET)
if(DPP_FOUND)
    target_link_libraries(VerifyBot dpp::dpp)
else()
    # get version 10.0.24 (latest as of 7/1/2023)
    option(BUILD_VOICE_SUPPORT "" OFF)
    option(DPP_BUILD_TEST "" OFF)
    FetchContent_Declare(libdpp GIT_REPOSITORY https://github.com/brainboxdotcc/DPP.git GIT_TAG 63ac3fed8bdd92e3122c6079b944f70fc9546dc2)

    FetchContent_GetProperties(libdpp)
    if(NOT libdpp_POPULATED)
        FetchContent_Populate(libdpp)
        target_include_directories(VerifyBot PUBLIC ${libdpp_SOURCE_DIR}/include)
        add_subdirectory(${libdpp_SOURCE_DIR} ${libdpp_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()

    target_link_libraries(VerifyBot dpp)
endif()

install(TARGETS VerifyBot
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
