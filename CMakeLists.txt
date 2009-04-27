SET( EXTENSION_NAME "Extensions_OpenGLSelection")

ADD_LIBRARY( ${EXTENSION_NAME}
  Utils/MouseSelector.h
  Utils/MouseSelector.cpp
  Utils/TransformationSelector.h
  Utils/TransformationSelector.cpp
)

TARGET_LINK_LIBRARIES( ${EXTENSION_NAME}
  OpenEngine_Display
)