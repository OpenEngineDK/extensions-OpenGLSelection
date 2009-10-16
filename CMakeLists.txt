SET( EXTENSION_NAME "Extensions_OpenGLSelection")

ADD_LIBRARY( ${EXTENSION_NAME}
  Utils/MouseSelection.h
  Utils/MouseSelection.cpp
  Utils/ISceneSelection.h
  Utils/GLSceneSelection.h
  Utils/GLSceneSelection.cpp
  Utils/PointingDevice.h
  Utils/ITool.h
  Utils/SelectionTool.h
  Utils/SelectionTool.cpp
  Utils/TransformationTool.h
  Utils/TransformationTool.cpp
  Utils/CameraTool.h
  Utils/CameraTool.cpp
  Utils/ToolChain.h
  Utils/ToolChain.cpp

  Utils/OSDIRenderer.h
  Utils/OSDIWidget.h
  Utils/OSDButton.h
  Utils/OSDButton.cpp
  Utils/OSDSlider.h
  Utils/OSDSlider.cpp
  Utils/OSDRenderer.h
  Utils/OSDRenderer.cpp
  Utils/OSDCollection.h
  Utils/OSDCollection.cpp
)

TARGET_LINK_LIBRARIES( ${EXTENSION_NAME}
  OpenEngine_Display
  Extensions_SDLFont
)