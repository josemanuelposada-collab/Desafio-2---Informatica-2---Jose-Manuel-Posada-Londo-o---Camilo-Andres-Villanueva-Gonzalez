TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    equipo.cpp \
    grupo.cpp \
    main.cpp \
    utilidades.cpp \
    jugador.cpp \
    fecha.cpp \
    registro_jugador_partido.cpp

HEADERS += \
    equipo.h \
    grupo.h \
    utilidades.h \
    jugador.h \
    fecha.h \
    registro_jugador_partido.h