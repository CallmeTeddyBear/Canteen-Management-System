QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addbalance.cpp \
    addstaff.cpp \
    addstudent.cpp \
    adminaccountsettings.cpp \
    admincustomerlogin.cpp \
    admintodaysspecial.cpp \
    adminwindow.cpp \
    checkout.cpp \
    customeraccountsettings.cpp \
    customerwindow.cpp \
    deletecustomer.cpp \
    discountoffer.cpp \
    logout.cpp \
    main.cpp \
    mainwindow.cpp \
    showbalance.cpp \
    showcustomerdataoption.cpp \
    showfoodmenu.cpp \
    studentstaffoption.cpp \
    todaysspecialpopup.cpp

HEADERS += \
    addbalance.h \
    addstaff.h \
    addstudent.h \
    adminaccountsettings.h \
    admincustomerlogin.h \
    admintodaysspecial.h \
    adminwindow.h \
    checkout.h \
    customeraccountsettings.h \
    customerwindow.h \
    deletecustomer.h \
    discountoffer.h \
    logout.h \
    mainwindow.h \
    showbalance.h \
    showcustomerdataoption.h \
    showfoodmenu.h \
    studentstaffoption.h \
    todaysspecialpopup.h

FORMS += \
    addbalance.ui \
    addstaff.ui \
    addstudent.ui \
    adminaccountsettings.ui \
    admincustomerlogin.ui \
    admintodaysspecial.ui \
    adminwindow.ui \
    checkout.ui \
    customeraccountsettings.ui \
    customerwindow.ui \
    deletecustomer.ui \
    discountoffer.ui \
    logout.ui \
    mainwindow.ui \
    showbalance.ui \
    showcustomerdataoption.ui \
    showfoodmenu.ui \
    studentstaffoption.ui \
    todaysspecialpopup.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    admin_resources/image1.png \
    canteen_database.db \
    customer _resources/toast.jpg \
    settings.png

win32:RC_ICONS += icon.ico
