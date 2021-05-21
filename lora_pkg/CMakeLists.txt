cmake_minimum_required(VERSION 3.0.2)
project(lora_pkg)
add_compile_options(-std=c++11)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

catkin_package(
INCLUDE_DIRS include
LIBRARIES ros_tutorials_topic
CATKIN_DEPENDS roscpp std_msgs
DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(lora_node src/topic_subscriber.cpp)
add_dependencies(lora_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(lora_node ${catkin_LIBRARIES})


