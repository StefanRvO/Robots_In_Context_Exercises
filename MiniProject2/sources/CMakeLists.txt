#####################################################
# Template for building RobWork dependent console application
#
# You should edit directly in this file to add extra source 
# files and when configuring your project.  
#####################################################

#
# Test CMake version
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)



# And now we add any targets that we want
add_executable(WavefrontPlanner main.cpp PPMLoader.cpp Image.cpp Planner.cpp point.cpp vector2D.cpp)



