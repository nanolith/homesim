PROJECT(homesim)

cmake_minimum_required(VERSION 3.10)

INCLUDE(CheckSymbolExists)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(minunit REQUIRED)

INCLUDE_DIRECTORIES(include)
AUX_SOURCE_DIRECTORY(src/logic HOMESIM_LOGIC_SOURCES)
AUX_SOURCE_DIRECTORY(src/parser HOMESIM_PARSER_SOURCES)
AUX_SOURCE_DIRECTORY(test HOMESIM_TEST_SOURCES)

ADD_LIBRARY(homesim_logic STATIC ${HOMESIM_LOGIC_SOURCES})
ADD_LIBRARY(homesim_parser STATIC ${HOMESIM_PARSER_SOURCES})

ADD_EXECUTABLE(testhomesim
    ${HOMESIM_LOGIC_SOURCES} ${HOMESIM_PARSER_SOURCES}
    ${HOMESIM_TEST_SOURCES})
TARGET_COMPILE_OPTIONS(testhomesim PRIVATE --coverage ${MINUNIT_CFLAGS})
TARGET_LINK_LIBRARIES(testhomesim PRIVATE --coverage ${MINUNIT_LDFLAGS})

ADD_CUSTOM_COMMAND(TARGET testhomesim
    POST_BUILD
    COMMAND testhomesim)

#Build a pkg-config file
SET(HOMESIM_PC "${CMAKE_BINARY_DIR}/homesim.pc")
FILE(WRITE  ${HOMESIM_PC} "Name: homesim")
FILE(APPEND ${HOMESIM_PC} "\nDescription: homesim digital logic simulator")
FILE(APPEND ${HOMESIM_PC} "\nVersion: 0.1")
FILE(APPEND ${HOMESIM_PC} "\nURL: https://github.com/nanolith/homesim")
FILE(APPEND ${HOMESIM_PC} "\nprefix=${CMAKE_INSTALL_PREFIX}")
FILE(APPEND ${HOMESIM_PC} "\nlibdir=\${prefix}/lib")
FILE(APPEND ${HOMESIM_PC} "\nincludedir=\${prefix}/include")
FILE(APPEND ${HOMESIM_PC} "\nLibs: -L\${libdir} -lhomesim_logic -lhomesim_parser")
FILE(APPEND ${HOMESIM_PC} "\nCflags: -I\${includedir}")
INSTALL(FILES ${HOMESIM_PC} DESTINATION lib/pkgconfig)

#Install headers
FILE(GLOB HOMESIM_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include/homesim/*.h")
INSTALL(FILES ${HOMESIM_INCLUDES} DESTINATION include/homesim)

#Install library
INSTALL(TARGETS homesim_logic
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
INSTALL(TARGETS homesim_parser
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
