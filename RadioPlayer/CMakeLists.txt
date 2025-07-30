cmake_minimum_required(VERSION 3.16)

project(RadioPlayer VERSION 0.1 LANGUAGES CXX)

# Qt ayarları
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt modüllerini bul
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

# Proje kaynakları
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    icystreamfetcher.cpp
    icystreamfetcher.h
    resources.qrc
)

# Uygulama hedefini oluştur
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RadioPlayer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(RadioPlayer SHARED ${PROJECT_SOURCES})
    else()
        add_executable(RadioPlayer ${PROJECT_SOURCES})
    endif()
endif()

# Qt kütüphaneleri bağla
target_link_libraries(RadioPlayer PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
)

# macOS ayarları
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.RadioPlayer)
endif()

set_target_properties(RadioPlayer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Yükleme ayarları
include(GNUInstallDirs)
install(TARGETS RadioPlayer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Qt6 finalize işlemi
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RadioPlayer)
endif()
