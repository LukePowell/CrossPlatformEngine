# Install script for directory: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solutionMac/src/SFMLTest/cmake_install.cmake")
  include("/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solutionMac/src/Engine/cmake_install.cmake")
  include("/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solutionMac/src/WindowingSystem/cmake_install.cmake")
  include("/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solutionMac/src/TinyXML2/cmake_install.cmake")
  include("/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solutionMac/src/GraphicsSystem/cmake_install.cmake")
  include("/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solutionMac/src/PhysicsSystem/cmake_install.cmake")
  include("/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solutionMac/src/Maths/cmake_install.cmake")
  include("/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solutionMac/src/InputSystem/cmake_install.cmake")
  include("/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solutionMac/src/SDLWindow/cmake_install.cmake")

endif()

