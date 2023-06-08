cmake_minimum_required(VERSION 3.21)
project(fileEditor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "U:/Software/Qt2/6.5.0/mingw_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        PrintSupport
        REQUIRED)

qt_standard_project_setup()
qt_add_executable(fileEditor icons.qrc main.cpp mainwindow.cpp mainwindow.h FindReplaceDialog.h)

target_link_libraries(fileEditor PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::PrintSupport
        )

#qt_add_resources(fileEditor imageresources
#        PREFIX "/"
#        FILES
#        icons/file-new.png
#        icons/file-open.png
#        icons/file-save.png
#        icons/file-saveas.png
#        icons/file-export.png
#
#        icons/style-bold.png
#        icons/style-italic.png
#        icons/style-underline.png
#        icons/style-size.png
#        icons/style-color.png
#        icons/style-font.png
#
#        icons/align-left.png
#        icons/align-center.png
#        icons/align-right.png
#
#        icons/edit-paste.png
#        icons/edit-copy.png
#        icons/edit-cut.png
#        icons/edit-image.png
#        icons/edit-undo.png
#        icons/edit-redo.png
#
#        icons/util-find.png
#        icons/util-replace.png
#        icons/util-zoomin.png
#        icons/util-zoomout.png
#        )

if (WIN32)
    set(DEBUG_SUFFIX)
#    if (CMAKE_BUILD_TYPE MATCHES "Debug")
#        set(DEBUG_SUFFIX "d")
#    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
