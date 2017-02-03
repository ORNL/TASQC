#----------------------------------------------------------------------------
# Copyright (c) 2015-, UT-Battelle LLC
# All rights reserved.
#
# Authors: Jay Jay Billings, Phil Evans, Alex McCaskey
# Author Contact: Phil Evans, evanspg@ornl.gov
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#  list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#  this list of conditions and the following disclaimer in the documentation
#  and/or other materials provided with the distribution.
#
# * Neither the name of kettransclient nor the names of its
#  contributors may be used to endorse or promote products derived from
#  this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#-----------------------------------------------------------------------------*/

# Set a name for this project.
PROJECT ( KeyTransClient CXX )

# Set the minimum required version of cmake for this project.
cmake_minimum_required(VERSION 2.8)

# Configure some default options
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
message(STATUS "C++ version ${CXX_STANDARD} configured.")

# Setup flag overrides if KeyTransClient is building on its own. This works by 
# determining if a parent is available and allowing flag overrides if not.
get_directory_property(hasParent PARENT_DIRECTORY)
message(STATUS "Parent directory is ${hasParent}")
if(NOT hasParent)
  #list( APPEND CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $ENV{CXX_FLAGS}")
  list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} $ENV{CXX_FLAGS}")
  message(STATUS "Parent detected. Disabling KeyTransClient flag overrides.")
endif()

# Include directories for the main program. Note that include 
# directories must come before add subdirectory!
include_directories("${CMAKE_SOURCE_DIR}/libcurlUtil")

# Add the source directory
add_subdirectory(libcurlUtil)

# Setup the library list
set(KEYTRANS_LIBS curlUtil curl)

# ---- Tests! ----- #

# Find Boost for unit tests
FIND_PACKAGE(Boost)
# Enable testing if boost is found.
if(Boost_FOUND)
    # Turn on the testing harness
	enable_testing()
    # Let the user know that tests will be built
    message(STATUS "Boost version ${Boost_VERSION} found.")
    message(STATUS "Tests will be built since Boost was found.")
    message(STATUS "Tests can be run with the 'make test' command.")
    # Include the header directory since BOOST tests
    INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIR}")
    INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/libcurlUtil")
    INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/libcurlUtil/tests")
    # Get the test files
    file(GLOB tests ${PROJECT_SOURCE_DIR}/libcurlUtil/tests/*Test*.cpp )
    # Make executables and link libraries for testers
    foreach(test ${tests})
        message(STATUS "Making test ${test}")
        get_filename_component(testName ${test} NAME_WE)
        add_executable(${testName} ${test})
        add_test(${testName} ${testName})
        target_link_libraries(${testName} ${KEYTRANS_LIBS}) 
        #add a label so the tests can be run separately
        set_property(TEST ${testName} PROPERTY LABELS ${PACKAGE_NAME})
    endforeach(test ${tests})
endif(Boost_FOUND)
