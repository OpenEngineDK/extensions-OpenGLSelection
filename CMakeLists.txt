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
  Utils/WidgetTool.h
  Utils/WidgetTool.cpp
  Utils/ToolChain.h
  Utils/ToolChain.cpp

  Widgets/IWidgetRenderer.h
  Widgets/IWidget.h
  Widgets/Button.h
  Widgets/Button.cpp
  Widgets/Slider.h
  Widgets/Slider.cpp
  Widgets/CircularSlider.h
#  Widgets/CircularSlider.cpp
  Widgets/WidgetRenderer.h
  Widgets/WidgetRenderer.cpp
  Widgets/Collection.h
  Widgets/Collection.cpp
)

TARGET_LINK_LIBRARIES( ${EXTENSION_NAME}
  OpenEngine_Display
  Extensions_SDLFont
)