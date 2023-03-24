cmake_minimum_required(VERSION 3.5)

project(07_Ex02_LoadExternalModel VERSION 0.1)

#======================= INCLUSION OF Qt =======================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})

#======================= INCLUSION OF glm =======================#
SET(_glm_HEADER_SEARCH_DIRS
"/usr/include"
"/usr/local/include"
"${CMAKE_SOURCE_DIR}/includes"
"E:/glm" )
# locate header
FIND_PATH(GLM_INCLUDE_DIR "glm/glm.hpp"
PATHS ${_glm_HEADER_SEARCH_DIRS})
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLM DEFAULT_MSG
GLM_INCLUDE_DIR)
IF(GLM_FOUND)
	SET(GLM_INCLUDE_DIRS "${GLM_INCLUDE_DIR}")
	MESSAGE(STATUS "GLM_INCLUDE_DIR = ${GLM_INCLUDE_DIR}")
ENDIF(GLM_FOUND)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(OpenGL)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Widgets REQUIRED)

#======================= INCLUSION OF Our Code ======================#
set(FORMS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/forms")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

include_directories(${FORMS_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})
include_directories(${GLM_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES
    "${FORMS_DIR}/*.ui"
    "${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
)

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

# Add the executable
if (WIN32) 
    add_executable(07_Ex02_LoadExternalModel WIN32 ${SOURCES})
elseif(UNIX)
    add_executable(07_Ex02_LoadExternalModel ${SOURCES})
endif()

# Add the target includes
target_include_directories(07_Ex02_LoadExternalModel PRIVATE ${FORMS_DIR})
target_include_directories(07_Ex02_LoadExternalModel PRIVATE ${INCLUDE_DIR})
target_include_directories(07_Ex02_LoadExternalModel PRIVATE ${SOURCE_DIR})

#===================== LINKING LIBRARIES =======================#
target_link_libraries( 07_Ex02_LoadExternalModel Qt5::OpenGL)
target_link_libraries( 07_Ex02_LoadExternalModel Qt5::Xml)
target_link_libraries( 07_Ex02_LoadExternalModel Qt5::Widgets)
target_link_libraries( 07_Ex02_LoadExternalModel ${QT_LIBRARIES} ${OPENGL_LIBRARIES})