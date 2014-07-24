FILE(GLOB_RECURSE files *.cpp *.h)
BUILD_GROUPS(./)
ADD_LIBRARY(CoreEngine ${files})
target_link_libraries(CoreEngine XML)