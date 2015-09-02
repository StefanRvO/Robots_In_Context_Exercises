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


#Wanted to add more error flags, but Image.cpp throw warnings with -pedantic and -Wunused-variable
SET( CMAKE_CXX_FLAGS  "	-std=c++14 		\
			-g			\
		 	-Wall 			\
			-Wextra 		\
			-Wshadow 		\
			-Wpointer-arith 	\
			-ffunction-sections 	\
			-fdata-sections		\
			-Wcast-qual		\
			-Werror			\
			-Wno-unused-variable 	\
			-Ofast			\
" )

# And now we add any targets that we want
add_executable(readpgmtest readpgmtest.cpp PPMLoader.cpp Image.cpp PathFinder.cpp point.cpp vector2D.cpp)



