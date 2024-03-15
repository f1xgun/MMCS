cmake_minimum_required(VERSION 3.5)

project(Lab_1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        accountrecord.cpp
        createrecorddialog.cpp
        homewindow.cpp

        recordsmanager.cpp
        recordwidget.cpp
        accountrecord.h
        createrecorddialog.h
        homewindow.h

        recordsmanager.h
        recordwidget.h
        createrecorddialog.ui
        homewindow.ui
        recordwidget.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Lab_1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Lab_1 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Lab_1 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Lab_1
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Lab_1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(
    Lab_1
    PRIVATE
    D:/Qt/Tools/OpenSSLv3/Win_x64/include)
target_link_libraries(
    Lab_1 # к какой цели подключать
    PRIVATE # в каком режиме подключать
    D:/Qt/Tools/OpenSSLv3/Win_x64/lib/libcrypto.lib) # что именно подключать
target_link_libraries(
    Lab_1 # к какой цели подключать
    PRIVATE # в каком режиме подключать
    D:/Qt/Tools/OpenSSLv3/Win_x64/lib/libssl.lib)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Lab_1)
endif()
set_target_properties(Lab_1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Lab_1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Lab_1)
endif()

configure_file (
    records.json
    records.json
)

configure_file (copy_icon.svg copy_icon.svg)
configure_file (add_icon.svg add_icon.svg)
configure_file (delete_icon.svg delete_icon.svg)
configure_file (search_icon.svg search_icon.svg)
configure_file (records_enc.txt records_enc.txt)
