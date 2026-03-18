cmake_minimum_required(VERSION 3.16)

project(EN-OS-Remote-Assistant-Creator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES
    translations/assistant_ru.ts
    translations/assistant_en.ts
    translations/assistant_uk.ts
    translations/assistant_de.ts
    translations/assistant_fr.ts
    translations/assistant_zh.ts
    translations/assistant_ko.ts
    translations/assistant_ja.ts
)

set(PROJECT_SOURCES
    main.cpp
    assistant_creator.cpp
    assistant_creator.h
    assistant_creator.ui
)

set(BINARY_NAME "en-os-remote-assistant-creator")

qt_add_executable(EN-OS-Remote-Assistant-Creator
    ${PROJECT_SOURCES}
    resources.qrc
)

set_target_properties(EN-OS-Remote-Assistant-Creator PROPERTIES
    OUTPUT_NAME ${BINARY_NAME}
)

target_link_libraries(EN-OS-Remote-Assistant-Creator PRIVATE Qt6::Widgets)

qt_add_translations(EN-OS-Remote-Assistant-Creator
    TS_FILES ${TS_FILES}
    RESOURCE_PREFIX "/i18n"
)

include(GNUInstallDirs)
install(TARGETS EN-OS-Remote-Assistant-Creator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
