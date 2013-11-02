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
	Game.cpp
	Player.cpp
    ForceDirected.cpp
	)

#Projects headers files	
SET(INC_FILES
    Node.hpp
    Graph.hpp
    Edge.hpp
    optional.hpp
    unmanaged_ptr.hpp
    Game.hpp
    Player.hpp
	)

#"Sources" and "Headers" are the group names in Visual Studio.
#They may have other uses too...
SOURCE_GROUP("Sources" FILES ${SRC_FILES})
SOURCE_GROUP("Headers" FILES ${INC_FILES})

set(GGG_COMPILE_FLAGS "-g -std=c++11 -O3 -Wall -Wextra -Wno-deprecated")

add_library(graph ${SRC_FILES})
SET_TARGET_PROPERTIES(graph PROPERTIES COMPILE_FLAGS ${GGG_COMPILE_FLAGS})


find_package(Gosu REQUIRED)

find_package(PkgConfig REQUIRED)

INCLUDE_DIRECTORIES(${Gosu_INCLUDE_DIRS})
LINK_DIRECTORIES(${Gosu_LIBRARY_DIRS})

#Build
ADD_EXECUTABLE(GraphGame main.cpp)
set_target_properties(GraphGame PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

SET_TARGET_PROPERTIES(GraphGame PROPERTIES COMPILE_FLAGS ${GGG_COMPILE_FLAGS})
if(APPLE)
    set_target_properties(GraphGame PROPERTIES LINK_FLAGS ${Gosu_APPLE_FRAMEWORKS})
endif()
TARGET_LINK_LIBRARIES(GraphGame ${Gosu_LIBRARIES} graph)


#Test
ADD_EXECUTABLE(GraphTest Test.cpp)
set_target_properties(GraphTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

SET_TARGET_PROPERTIES(GraphTest PROPERTIES COMPILE_FLAGS ${GGG_COMPILE_FLAGS})
TARGET_LINK_LIBRARIES(GraphTest ${Gosu_LIBRARIES} graph)
