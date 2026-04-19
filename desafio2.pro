TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    utilidades.cpp \
    jugador.cpp \
    fecha.cpp \
    registro_jugador_partido.cpp

HEADERS += \
    utilidades.h \
    jugador.h \
    fecha.h \
    registro_jugador_partido.h