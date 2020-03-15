#The following sources are external code (not written by us)
set(SSLCLIENT_SOURCES
        sslClient/robocup_ssl_client.cpp
        sslClient/netraw.cpp
        )
add_library(net
        ${SSLCLIENT_SOURCES}
        )
target_link_libraries(net
        PUBLIC proto
        )

target_include_directories(net
        INTERFACE include
        PRIVATE include/net
        )