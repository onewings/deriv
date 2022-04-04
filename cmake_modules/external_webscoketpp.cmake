include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

ExternalProject_Add(project_websocket
  GIT_REPOSITORY     https://github.com/zaphoyd/websocketpp.git
  GIT_TAG            1b11fd301531e6df35a6107c1e8665b1e77a2d8e
  PREFIX             ${CMAKE_CURRENT_SOURCE_DIR}/libraries/websocketpp
  SOURCE_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/libraries/websocketpp
  DOWNLOAD_DIR       ${CMAKE_CURRENT_SOURCE_DIR}/libraries/websocketpp
  STAMP_DIR          ${CMAKE_BINARY_DIR}/libraries/websocketpp/stamp
  TMP_DIR            ${CMAKE_BINARY_DIR}/libraries/websocketpp/tmp
  BINARY_DIR         ${CMAKE_BINARY_DIR}/libraries/websocketpp
  INSTALL_COMMAND    echo "Fake benchmark install" # could simply install it to ${CMAKE_BINARY_DIR}/libraries/ by setting INSTALL_DIR
  CMAKE_ARGS ""
  UPDATE_COMMAND ""
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(project_websocket install_dir) 
