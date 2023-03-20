cmake_minimum_required(VERSION 3.5)

project(Tutorial VERSION 0.1)

#======================= INCLUSION OF Qt =======================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})

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

add_subdirectory(Quiz_01_MouseEvent)
add_subdirectory(Quiz_02_Draw_Rectangle)
add_subdirectory(Quiz_03_Transformation_Animation)
add_subdirectory(Quiz_04_Robot_Joint_Rotation)
add_subdirectory(Quiz_05_Mutiple_Viewport_with_Space_Transformations)