cmake_minimum_required(VERSION 3.16)

project(JsonTreeViewChallenge VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

# Copy resources to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

qt_add_executable(appJsonTreeViewChallenge
    src/main.cpp
    src/treeitem.h
    src/treeitem.cpp
    src/treemodel.h
    src/treemodel.cpp
    src/jsonmodel.h
    src/jsonmodel.cpp
    resources.qrc
)

qt_add_qml_module(appJsonTreeViewChallenge
    URI JsonTreeViewChallenge
    VERSION 1.0
    QML_FILES
        src/Main.qml
    SOURCES 
        src/treeitem.cpp src/treeitem.h 
        src/treemodel.cpp src/treemodel.h
        src/jsonmodel.h src/jsonmodel.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appJsonTreeViewChallenge PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appJsonTreeViewChallenge
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appJsonTreeViewChallenge
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appJsonTreeViewChallenge
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
