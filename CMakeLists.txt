CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(GraphGame)

#Projects source files
SET(SRC_FILES
	main.cpp
	)

#Projects headers files	
SET(INC_FILES
	)

#"Sources" and "Headers" are the group names in Visual Studio.
#They may have other uses too...
SOURCE_GROUP("Sources" FILES ${SRC_FILES})
SOURCE_GROUP("Headers" FILES ${INC_FILES})

find_package(Gosu REQUIRED)

INCLUDE_DIRECTORIES(${Gosu_INCLUDE_DIRS})
LINK_DIRECTORIES(${Gosu_LIBRARY_DIRS})

#Build
ADD_EXECUTABLE(GraphGame ${SRC_FILES})
set_target_properties(GraphGame PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

SET_TARGET_PROPERTIES(GraphGame PROPERTIES COMPILE_FLAGS "-g -std=c++0x -O3")
TARGET_LINK_LIBRARIES(GraphGame ${Gosu_LIBRARIES})
