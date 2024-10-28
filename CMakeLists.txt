cmake_minimum_required(VERSION 3.29.6)

project(rawtcp VERSION 1.0.0 DESCRIPTION "Cross-platform Raw TCP Library")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (NOT LIB_TYPE)
    set(LIB_TYPE STATIC)
endif()

set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "BSD$")
    set(IS_BSD TRUE)
endif()

if (IS_BSD)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PCAP REQUIRED libpcap)
    set(LIB_INCLUDES ${PCAP_INCLUDE_DIRS})
    set(LIBS ${PCAP_LIBRARIES})
elseif (WIN32)
    set(LIBS ws2_32)
endif()

set(SOURCES
    src/bsd/rawsock.c src/nux/rawsock.c src/win/rawsock.c
    src/bsd/network.c src/nux/network.c src/win/network.c src/uni/network.c
    src/nux/thread.c src/win/thread.c
    src/eth.c
    src/arp.c
    src/ip.c
    src/tcp.c
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_executable(${PROJECT_NAME}
        ${SOURCES}
        src/test.c
    )
else()
    add_library(${PROJECT_NAME} ${LIB_TYPE}
        ${SOURCES}
    )
endif()

target_link_libraries(${PROJECT_NAME}
    ${LIBS}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE include
    PUBLIC ${LIB_INCLUDES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/rawtcp.h)