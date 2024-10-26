cmake_minimum_required(VERSION 3.29.6)

project(rawsock)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "BSD$")
    set(IS_BSD TRUE)
endif()

if (IS_BSD)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PCAP REQUIRED libpcap)
endif()


add_executable(${PROJECT_NAME}
    src/bsd/rawsock.c src/nux/rawsock.c src/win/rawsock.c
    src/bsd/network.c src/nux/network.c src/win/network.c src/uni/network.c
    src/eth.c
    src/arp.c
    src/ip.c
    src/tcp.c
    src/main.c
)

target_link_libraries(${PROJECT_NAME}
    ${PCAP_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE include
    PUBLIC ${PCAP_INCLUDE_DIRS}
)