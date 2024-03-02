QT       += core gui network networkauth openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GUI/CoverViewer.cpp \
    GUI/CustomSlider.cpp \
    GUI/mainwindow.cpp \
    SpotifyWrapper/SpotifyWrapper.cpp \
    main.cpp

HEADERS += \
    GUI/CoverViewer.h \
    GUI/CustomSlider.h \
    GUI/mainwindow.h \
    SpotifyWrapper/SpotifyWrapper.h

FORMS += \
    GUI/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/rec.qrc
