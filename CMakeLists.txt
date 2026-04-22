cmake_minimum_required(VERSION 3.16)
project(PML VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# cppsocket
FetchContent_Declare(
  cppsocket
  GIT_REPOSITORY https://github.com/DharioNewbery/cppsocket.git
  GIT_TAG v0.0.1-alpha.2
)

FetchContent_MakeAvailable(cppsocket)

# Project files
set(CLIENTSOURCES src/client.cpp)
set(CLIENTHEADERS)

set(SERVERSOURCES src/server.cpp)
set(SERVERHEADERS)


add_executable(pml_client ${CLIENTSOURCES} ${CLIENTHEADERS})
add_executable(pml_server ${SERVERSOURCES} ${SERVERHEADERS})

target_include_directories(pml_client PRIVATE include)
target_include_directories(pml_server PRIVATE include)

target_include_directories(pml_client PRIVATE 
    ${cppsocket_SOURCE_DIR}/include 
)
target_include_directories(pml_server PRIVATE 
    ${cppsocket_SOURCE_DIR}/include 
)

if(WIN32)
    target_link_libraries(pml_client PRIVATE ws2_32)
    target_link_libraries(pml_server PRIVATE ws2_32)
endif()