#############################################################################
## Set minimum required version of cmake, project name and compile options ##
#############################################################################
cmake_minimum_required(VERSION 2.8.3)
project(hal_iocard)

set(CMAKE_VERBOSE_MAKEFILE ON)

#############################################################################
## Find catkin packages and libraries for catkin and system dependencies   ##
#############################################################################
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

################################################
## Declare ROS messages, services and actions ##
################################################

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

###################################
## catkin specific configuration ##
###################################
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES can
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

#############################################################################
## Build                                                                   ##
#############################################################################
include_directories(
 include
 ${catkin_INCLUDE_DIRS}
)
link_directories("${PROJECT_SOURCE_DIR}")
add_executable(hal_iocard src/IoCard.cpp src/iocard_node.cpp)
target_link_libraries(hal_iocard ${catkin_LIBRARIES} -lbiodaq)
add_dependencies(hal_iocard ${catkin_EXPORTED_TARGETS})

#############
## Install ##
#############

install(TARGETS
  hal_iocard
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

#############
## Testing ##
#############
