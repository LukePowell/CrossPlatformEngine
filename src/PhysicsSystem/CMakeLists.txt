FILE(GLOB files *.cpp *.h)
ADD_LIBRARY(Physics ${files})
target_link_libraries(Physics CoreEngine Maths)