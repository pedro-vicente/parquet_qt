cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Source directory is: " ${CMAKE_SOURCE_DIR})
message(STATUS "Build directory is: " ${CMAKE_CURRENT_BINARY_DIR})

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

#//////////////////////////
# executable
#//////////////////////////

set(src ${src})
set(src ${src} display.cc)
add_executable(display ${src})

target_compile_definitions(display PRIVATE
    ARROW_STATIC_DEFINE    # tells Arrow headers to use static linkage
    PARQUET_STATIC_DEFINE  # tells Parquet headers to use static linkage
    ARROW_STATIC           # additional Arrow static flag
    PARQUET_STATIC         # additional Parquet static flag
)

#//////////////////////////
# parquet
#//////////////////////////

include_directories(${CMAKE_SOURCE_DIR}/arrow-21.0.0/cpp/src)
include_directories(${CMAKE_SOURCE_DIR}/arrow-21.0.0/build/src)

set(ARROW_BUILD_DIR ${CMAKE_SOURCE_DIR}/arrow-21.0.0/build)

if (MSVC)
  set(ARROW_LIB "${ARROW_BUILD_DIR}/debug/Debug/arrow_staticd.lib")
  set(PARQUET_LIB "${ARROW_BUILD_DIR}/debug/Debug/parquet_staticd.lib")
  set(THRIFT_LIB "${ARROW_BUILD_DIR}/lib/Debug/thriftmdd.lib")
  set(ZLIB_LIB "${ARROW_BUILD_DIR}/zlib_ep/src/zlib_ep-install/lib/zlibstaticd.lib")
  
  target_link_libraries(display PRIVATE
    ${PARQUET_LIB}
    ${ARROW_LIB}
    ${THRIFT_LIB}
    ${ZLIB_LIB}
    ws2_32
    crypt32
    wldap32
    bcrypt
)

endif()

#//////////////////////////
# Qt
# link with libraries
# lib_dep contains a cascade definition of all the libraries needed to link
#//////////////////////////

set(lib_dep ${lib_dep})
set(lib_dep ${lib_dep} Qt6::Widgets)
set(lib_dep ${lib_dep} Qt6::Core)

target_link_libraries(display PRIVATE ${lib_dep})
set_target_properties(display PROPERTIES WIN32_EXECUTABLE ON MACOSX_BUNDLE ON)
