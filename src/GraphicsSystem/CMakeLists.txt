FILE(GLOB files *.cpp *.h)
ADD_LIBRARY(Graphics ${files})

IF(MSVC)
	target_link_libraries(Graphics CoreEngine glew32.lib)
ELSE()
	target_link_libraries(Graphics CoreEngine)
ENDIF(MSVC)
