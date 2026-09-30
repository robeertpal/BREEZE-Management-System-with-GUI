#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>
#include <QPalette>
#include <QLinearGradient>
#include <QInputDialog>
#include <QStackedWidget>
#include <QSpacerItem>
#include <QGraphicsBlurEffect>

#include "staff.hpp"
#include "guest_operations.hpp"
#include "staff_operations.hpp"
#include "guest_manager.hpp"

// === Pagini externe ===
QWidget* createAddGuestPage(QStackedWidget* stack, QWidget* backTargetPage);
QWidget* createCheckInPage(QStackedWidget* stack, QWidget* backTargetPage);
QWidget* createParticipantsPage(QStackedWidget* stack, QWidget* backTargetPage);
QWidget* createTicketInfoPage(QStackedWidget* stack, QWidget* backTargetPage);
QWidget* createPaymentPage(QStackedWidget* stack, QWidget* backTargetPage);
QWidget* createTopUpPage(QStackedWidget* stack, QWidget* backTargetPage);
QWidget* createAddStaffPage(QStackedWidget* stack, QWidget* backTargetPage);
QWidget* createDeleteStaffPage(QStackedWidget* stack, QWidget* backTargetPage);
QWidget* createRestrictAccessPage(QStackedWidget* stack, QWidget* backTargetPage);
QWidget* createConsultBalancePage(QStackedWidget* stack, QWidget* backTargetPage);

QWidget* createAgentMenu(std::shared_ptr<Staff> staffCurent, QStackedWidget* stack) {
    QWidget *page = new QWidget();
    page->setObjectName("agentMenu");

    QVBoxLayout *mainLayout = new QVBoxLayout(page);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);
    mainLayout->setAlignment(Qt::AlignTop);

    // === BACKGROUND ===
    QLinearGradient gradient(0, 0, 0, 520);
    gradient.setColorAt(0.0, QColor(90, 0, 157));
    gradient.setColorAt(1.0, QColor(0, 190, 255));

    QPixmap overlay(":/assets/logo.jpg");
    overlay = overlay.scaled(600, 520, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPainter painter(&overlay);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.fillRect(overlay.rect(), QColor(255, 255, 255, 100));
    painter.end();

    QPixmap background(600, 520);
    background.fill(Qt::transparent);
    QPainter combo(&background);
    combo.fillRect(background.rect(), QBrush(gradient));
    combo.drawPixmap(0, 0, overlay);
    combo.end();

    QPalette pal;
    pal.setBrush(QPalette::Window, QBrush(background));
    page->setAutoFillBackground(true);
    page->setPalette(pal);

    QLabel *logo = new QLabel();
    QPixmap logoPixmap(":/assets/logo.jpg");
    logoPixmap = logoPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPixmap);
    logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop);

    QLabel *welcome = new QLabel("<h3 style='color:white;'>Bine ai venit, <i>" + QString::fromStdString(staffCurent->getCredentials().getUsername()) + "</i></h3>");
    welcome->setStyleSheet("color:white;");
    welcome->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(welcome);

    QLabel *subtext = new QLabel("Alege una dintre următoarele opțiuni:");
    subtext->setStyleSheet("color:white; font-size:14px;");
    subtext->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(subtext);

    QVBoxLayout *contentLayout = new QVBoxLayout();
    QScrollArea *scroll = new QScrollArea();
    QWidget *buttonContainer = new QWidget();
    buttonContainer->setObjectName("agentMenuButtonContainer");
    buttonContainer->setStyleSheet("background: transparent;");
    QVBoxLayout *buttonLayout = new QVBoxLayout(buttonContainer);
    buttonLayout->setSpacing(6);

    auto addButton = [&](const QString &text, std::function<void()> func) {
        QPushButton *btn = new QPushButton(text);
        btn->setMinimumHeight(36);
        btn->setStyleSheet("background-color: white; color: #5A009D; font-weight: bold; border-radius: 6px;");
        buttonLayout->addWidget(btn);
        QObject::connect(btn, &QPushButton::clicked, func);
    };

    auto guests = loadGuestsFromFile();

    bool isAdmin = (staffCurent->getStaffID() == 0);
    bool isTicket = dynamic_cast<TicketAgent*>(staffCurent.get()) != nullptr;
    bool isSecurity = dynamic_cast<SecurityAgent*>(staffCurent.get()) != nullptr;
    bool isTopUp = dynamic_cast<TopUpAgent*>(staffCurent.get()) != nullptr;

    if (isAdmin || isTicket) {
        addButton("Afișează toți participanții", [stack, page]() {
            stack->addWidget(createParticipantsPage(stack, page));
            stack->setCurrentIndex(stack->count() - 1);
        });

        addButton("Adaugă participant nou", [stack, page]() {
            stack->addWidget(createAddGuestPage(stack, page));
            stack->setCurrentIndex(stack->count() - 1);
        });

        addButton("Check-in participant", [stack, page]() {
            stack->addWidget(createCheckInPage(stack, page));
            stack->setCurrentIndex(stack->count() - 1);
        });

        addButton("Afișează informații bilet", [stack, page]() {
            stack->addWidget(createTicketInfoPage(stack, page));
            stack->setCurrentIndex(stack->count() - 1);
        });

        addButton("Plată cu brățară", [stack, page]() {
            stack->addWidget(createPaymentPage(stack, page));
            stack->setCurrentIndex(stack->count() - 1);
        });

        if (isAdmin) {
            addButton("Adaugă membru staff", [stack, page]() {
                stack->addWidget(createAddStaffPage(stack, page));
                stack->setCurrentIndex(stack->count() - 1);
            });

            addButton("Șterge membru staff", [stack, page]() {
                stack->addWidget(createDeleteStaffPage(stack, page));
                stack->setCurrentIndex(stack->count() - 1);
            });
        }
    }

    if (isAdmin || isSecurity) {
        addButton("Afișează informații bilet", [stack, page]() {
            stack->addWidget(createTicketInfoPage(stack, page));
            stack->setCurrentIndex(stack->count() - 1);
        });

        addButton("Restricționează acces", [stack, page]() {
            stack->addWidget(createRestrictAccessPage(stack, page));
            stack->setCurrentIndex(stack->count() - 1);
        });
    }

    if (isAdmin || isTopUp) {
        addButton("Încarcă sold brățară", [stack, page]() {
            stack->addWidget(createTopUpPage(stack, page));
            stack->setCurrentIndex(stack->count() - 1);
        });

        addButton("Consultă sold brățară", [stack, page]() {
            stack->addWidget(createConsultBalancePage(stack, page));
            stack->setCurrentIndex(stack->count() - 1);
        });

        addButton("Plată cu brățară", [stack, page]() {
            stack->addWidget(createPaymentPage(stack, page));
            stack->setCurrentIndex(stack->count() - 1);
        });
    }

    buttonContainer->setLayout(buttonLayout);
    scroll->setWidget(buttonContainer);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet(R"(
        QScrollArea {
            background: transparent;
            border: none;
        }
        QScrollBar:vertical, QScrollBar:horizontal {
            width: 0px;
            height: 0px;
        }
    )");
    contentLayout->addWidget(scroll);

    QPushButton *logoutBtn = new QPushButton("Deconectare");
    logoutBtn->setMinimumHeight(36);
    logoutBtn->setStyleSheet("background-color: white; color: #5A009D; font-weight: bold; border-radius: 6px;");
    QObject::connect(logoutBtn, &QPushButton::clicked, [stack]() {
        stack->setCurrentIndex(0);
    });
    contentLayout->addWidget(logoutBtn);

    mainLayout->addLayout(contentLayout);

    return page;
}