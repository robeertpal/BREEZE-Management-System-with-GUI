QT += widgets

# Dacă folosești nlohmann/json instalat cu Homebrew:
INCLUDEPATH += /opt/homebrew/Cellar/nlohmann-json/3.12.0/include

SOURCES += \
    main.cpp \
    guest_operations.cpp \
    staff_operations.cpp \
    credentials_operations.cpp \
    partner_operations.cpp \
    partner_manager.cpp \
    guest_manager.cpp \
    staff_manager.cpp \
    validators.cpp \
    date.cpp \
    exceptions.cpp \
    member.cpp \
    email.cpp \
    phone.cpp \
    ticket.cpp \
    guest.cpp \
    staff.cpp \
    credentials.cpp \
    generator.cpp \
    credentials_manager.cpp \
    partner.cpp \
    agentlogin.cpp \
    partnerlogin.cpp \
    agentmenu.cpp \
    participantsdialog.cpp \
    addguest.cpp \
    mainwindow.cpp \ 
    partnermenu.cpp \
    checkin.cpp \
    participantsview.cpp \
    ticketinfo.cpp \
    payment.cpp \ 
    topup.cpp \
    addstaff.cpp \ 
    restrict.cpp \
    delete.cpp \
    consult.cpp \ 
    partnerbalance.cpp \
    registersale.cpp \ 
    addpartneragent.cpp \
    deletepartneragent.cpp

HEADERS += \
    guest_operations.hpp \
    staff_operations.hpp \
    generator.hpp \
    credentials_operations.hpp \
    partner_operations.hpp \
    partner_manager.hpp \
    guest_manager.hpp \
    staff_manager.hpp \
    validators.hpp \
    date.hpp \
    exceptions.hpp \
    fee_strategy.hpp \
    member.hpp \
    email.hpp \
    phone.hpp \
    bracelet.hpp \
    ticket.hpp \
    guest.hpp \
    staff.hpp \
    credentials.hpp \
    credentials_manager.hpp \
    partner.hpp 

# Dacă adaugi fișiere UI:
# FORMS +=

# Dacă ai fișiere de resurse:
# RESOURCES +=

# Alte setări, dacă ai nevoie:
# QMAKE_CXXFLAGS += -Wall -Wextra

# Pentru debug:
# CONFIG += debug

# Pentru release:
# CONFIG += release
