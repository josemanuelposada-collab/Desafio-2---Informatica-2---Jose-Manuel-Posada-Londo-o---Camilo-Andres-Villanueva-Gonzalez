TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    medidor_recursos.cpp \
    utilidades.cpp \
    jugador.cpp \
    equipo.cpp \
    grupo.cpp \
    fecha.cpp \
    registro_jugador_partido.cpp \
    partido.cpp \
    torneo.cpp

HEADERS += \
    medidor_recursos.h \
    utilidades.h \
    jugador.h \
    equipo.h \
    grupo.h \
    fecha.h \
    registro_jugador_partido.h \
    partido.h \
    torneo.h