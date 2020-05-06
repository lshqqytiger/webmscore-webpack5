set(_components
    Core
    Gui
    #Designer
    # Network
    # Test
    # Qml
    # Quick
    # QuickControls2
    # QuickTemplates2
    # QuickWidgets
    Xml
    XmlPatterns
    Svg
    # Sql
    # Widgets
    # PrintSupport
    # Concurrent
    # OpenGL
    # LinguistTools
    # Help
  )
foreach(_component ${_components})
  find_package(Qt5${_component})
  list(APPEND QT_LIBRARIES ${Qt5${_component}_LIBRARIES})
  list(APPEND QT_INCLUDES ${Qt5${_component}_INCLUDE_DIRS})
  add_definitions(${Qt5${_component}_DEFINITIONS})
endforeach()

include_directories(${QT_INCLUDES})
