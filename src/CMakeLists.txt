SUBDIRS(SFMLTest Engine WindowingSystem SFMLWindow TinyXML2 GraphicsSystem PhysicsSystem Maths InputSystem SDLWindow)

INCLUDE_DIRECTORIES(./ ./Engine)

IF(UNIX)
	ADD_DEFINITIONS(-DC11_UNIX)
ENDIF(UNIX)