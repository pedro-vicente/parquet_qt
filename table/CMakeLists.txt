cmake_minimum_required(VERSION 3.16)
project(parquet_qt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Source directory is: " ${CMAKE_SOURCE_DIR})
message(STATUS "Build directory is: " ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${CMAKE_SOURCE_DIR}/arrow-21.0.0/cpp/src)
include_directories(${CMAKE_SOURCE_DIR}/arrow-21.0.0/build/src)

add_executable(table table.cc)

target_compile_definitions(table PRIVATE
    ARROW_STATIC_DEFINE    # tells Arrow headers to use static linkage
    PARQUET_STATIC_DEFINE  # tells Parquet headers to use static linkage
    ARROW_STATIC           # additional Arrow static flag
    PARQUET_STATIC         # additional Parquet static flag
)

set(ARROW_BUILD_DIR ${CMAKE_SOURCE_DIR}/arrow-21.0.0/build)

if (MSVC)
  set(ARROW_LIB "${ARROW_BUILD_DIR}/debug/Debug/arrow_staticd.lib")
  set(PARQUET_LIB "${ARROW_BUILD_DIR}/debug/Debug/parquet_staticd.lib")
  set(THRIFT_LIB "${ARROW_BUILD_DIR}/lib/Debug/thriftmdd.lib")
  set(ZLIB_LIB "${ARROW_BUILD_DIR}/zlib_ep/src/zlib_ep-install/lib/zlibstaticd.lib")
  
  target_link_libraries(table PRIVATE
    ${PARQUET_LIB}
    ${ARROW_LIB}
    ${THRIFT_LIB}
    ${ZLIB_LIB}
    ws2_32
    crypt32
    wldap32
    bcrypt
)

  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT table)

endif()
