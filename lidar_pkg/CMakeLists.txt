cmake_minimum_required(VERSION 3.0.2)
project(lidar_pkg)

find_package(catkin REQUIRED COMPONENTS
  life_msgs
  roscpp
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES lidar_pkg life_msgs
  CATKIN_DEPENDS roscpp std_msgs 
)
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(lidar_node src/main.cpp)
add_dependencies(lidar_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(lidar_node
   ${catkin_LIBRARIES}
)

