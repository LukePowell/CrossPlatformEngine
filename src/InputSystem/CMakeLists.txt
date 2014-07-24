FILE(GLOB files *.cpp *.h)
ADD_LIBRARY(Input ${files})
target_link_libraries(Input CoreEngine)