cmake_minimum_required(VERSION 3.0.2)
project(life_best)

find_package(catkin REQUIRED COMPONENTS
  life_msgs
  roscpp
  sensor_msgs
  std_msgs
  nav_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES life_best life_msgs
  CATKIN_DEPENDS roscpp sensor_msgs std_msgs nav_msgs
  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  /usr/include/eigen3 
)

add_executable(robot_core src/robot_main.cpp)

add_dependencies(robot_core ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(robot_core
  ${catkin_LIBRARIES}
)
