TEMPLATE = app
TARGET = breeze
QT += widgets
CONFIG += c++17
lessThan(QT_MAJOR_VERSION, 6): error("Qt 6 or newer is required")

# Optionally pass NLOHMANN_JSON_INCLUDE=<include-directory> to qmake.
!isEmpty(NLOHMANN_JSON_INCLUDE): INCLUDEPATH += $$NLOHMANN_JSON_INCLUDE

INCLUDEPATH += $$PWD/src/core $$PWD/src/model $$PWD/src/service \
               $$PWD/src/validation $$PWD/src/exception $$PWD/src/gui

# These GUI implementation files are included from main.cpp transitively.
# Do not also compile them as separate translation units.
DISTFILES += src/gui/agentlogin.cpp \
    src/gui/agentmenu.cpp \
    src/gui/mainwindow.cpp \
    src/gui/partnerlogin.cpp \
    src/gui/partnermenu.cpp

SOURCES += src/core/generator.cpp \
    src/exception/exceptions.cpp \
    src/gui/addguest.cpp \
    src/gui/addpartneragent.cpp \
    src/gui/addstaff.cpp \
    src/gui/checkin.cpp \
    src/gui/consult.cpp \
    src/gui/delete.cpp \
    src/gui/deletepartneragent.cpp \
    src/gui/main.cpp \
    src/gui/participantsdialog.cpp \
    src/gui/participantsview.cpp \
    src/gui/partnerbalance.cpp \
    src/gui/payment.cpp \
    src/gui/registersale.cpp \
    src/gui/restrict.cpp \
    src/gui/ticketinfo.cpp \
    src/gui/topup.cpp \
    src/model/credentials.cpp \
    src/model/date.cpp \
    src/model/email.cpp \
    src/model/guest.cpp \
    src/model/member.cpp \
    src/model/partner.cpp \
    src/model/partner_staff.cpp \
    src/model/phone.cpp \
    src/model/staff.cpp \
    src/model/ticket.cpp \
    src/service/credentials_manager.cpp \
    src/service/credentials_operations.cpp \
    src/service/guest_manager.cpp \
    src/service/guest_operations.cpp \
    src/service/partner_manager.cpp \
    src/service/partner_operations.cpp \
    src/service/staff_manager.cpp \
    src/service/staff_operations.cpp \
    src/validation/validators.cpp

HEADERS += src/core/fee_strategy.hpp \
    src/core/generator.hpp \
    src/core/ticket_factory.hpp \
    src/exception/exceptions.hpp \
    src/model/bracelet.hpp \
    src/model/credentials.hpp \
    src/model/date.hpp \
    src/model/email.hpp \
    src/model/guest.hpp \
    src/model/member.hpp \
    src/model/partner.hpp \
    src/model/partner_staff.hpp \
    src/model/phone.hpp \
    src/model/staff.hpp \
    src/model/ticket.hpp \
    src/service/credentials_manager.hpp \
    src/service/credentials_operations.hpp \
    src/service/guest_manager.hpp \
    src/service/guest_operations.hpp \
    src/service/partner_manager.hpp \
    src/service/partner_operations.hpp \
    src/service/staff_manager.hpp \
    src/service/staff_operations.hpp \
    src/validation/validators.hpp

RESOURCES += assets/assets.qrc
