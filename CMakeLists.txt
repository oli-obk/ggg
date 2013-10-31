CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(GraphGame)

SET(CMAKE_CXX_COMPILER g++-4.8)
SET(CMAKE_C_COMPILER gcc-4.8)

#Projects source files
SET(SRC_FILES
	Node.cpp
	Graph.cpp
	FloydWarshall.cpp
	Betweenness.cpp
	)

#Projects headers files	
SET(INC_FILES
	)

#"Sources" and "Headers" are the group names in Visual Studio.
#They may have other uses too...
SOURCE_GROUP("Sources" FILES ${SRC_FILES})
SOURCE_GROUP("Headers" FILES ${INC_FILES})

add_library(graph ${SRC_FILES})
SET_TARGET_PROPERTIES(graph PROPERTIES COMPILE_FLAGS "-g -std=c++11 -O3 -Wall -Wextra -Wno-import -Wno-deprecated")

if(NOT APPLE)
find_package(Gosu REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIROMM REQUIRED cairomm-1.0)

INCLUDE_DIRECTORIES(${Gosu_INCLUDE_DIRS}
                    ${CAIROMM_INCLUDE_DIRS})
LINK_DIRECTORIES(${Gosu_LIBRARY_DIRS}
                 ${CAIROMM_LIBRARY_DIRS})

#Build
ADD_EXECUTABLE(GraphGame main.cpp)
set_target_properties(GraphGame PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

SET_TARGET_PROPERTIES(GraphGame PROPERTIES COMPILE_FLAGS "-g -std=c++11 -O3 -Wall -Wextra -Wno-import -Wno-deprecated")
TARGET_LINK_LIBRARIES(GraphGame ${Gosu_LIBRARIES} ${CAIROMM_LIBRARIES} graph)
endif()


#Test
ADD_EXECUTABLE(GraphTest Test.cpp)
set_target_properties(GraphTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

SET_TARGET_PROPERTIES(GraphTest PROPERTIES COMPILE_FLAGS "-g -std=c++11 -O3 -Wall -Wextra -Wno-import -Wno-deprecated")
TARGET_LINK_LIBRARIES(GraphTest ${Gosu_LIBRARIES} ${CAIROMM_LIBRARIES} graph)
