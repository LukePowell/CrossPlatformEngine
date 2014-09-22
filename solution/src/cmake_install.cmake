# Install script for directory: C:/Users/Victus/Documents/GitHub/CrossPlatformEngine/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/SFMLTest")
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
  include("C:/Users/Victus/Documents/GitHub/CrossPlatformEngine/solution/src/SFMLTest/cmake_install.cmake")
  include("C:/Users/Victus/Documents/GitHub/CrossPlatformEngine/solution/src/Engine/cmake_install.cmake")
  include("C:/Users/Victus/Documents/GitHub/CrossPlatformEngine/solution/src/WindowingSystem/cmake_install.cmake")
  include("C:/Users/Victus/Documents/GitHub/CrossPlatformEngine/solution/src/TinyXML2/cmake_install.cmake")
  include("C:/Users/Victus/Documents/GitHub/CrossPlatformEngine/solution/src/GraphicsSystem/cmake_install.cmake")
  include("C:/Users/Victus/Documents/GitHub/CrossPlatformEngine/solution/src/PhysicsSystem/cmake_install.cmake")
  include("C:/Users/Victus/Documents/GitHub/CrossPlatformEngine/solution/src/Maths/cmake_install.cmake")
  include("C:/Users/Victus/Documents/GitHub/CrossPlatformEngine/solution/src/InputSystem/cmake_install.cmake")
  include("C:/Users/Victus/Documents/GitHub/CrossPlatformEngine/solution/src/SDLWindow/cmake_install.cmake")

endif()

