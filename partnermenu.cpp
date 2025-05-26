#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QPalette>
#include <QLinearGradient>
#include <QStackedWidget>

#include "partner.hpp"

QWidget* createRegisterSalePage(std::shared_ptr<Partner> partner, QStackedWidget* stack, QWidget* backTargetPage);
QWidget* createPartnerBalancePage(std::shared_ptr<Partner> partner, QStackedWidget* stack, QWidget* backTargetPage);
QWidget* createAddPartnerAgentPage(std::shared_ptr<Partner> partner, QStackedWidget* stack, QWidget* backTargetPage);
QWidget* createDeletePartnerAgentPage(std::shared_ptr<Partner> partner, QStackedWidget* stack, QWidget* backTargetPage);

QWidget* createPartnerMenu(std::shared_ptr<Partner> partner, int staffID, QStackedWidget* stack) {
    QWidget *page = new QWidget();
    page->setObjectName("partnerMenu");

    QVBoxLayout *mainLayout = new QVBoxLayout(page);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);
    mainLayout->setAlignment(Qt::AlignTop);

    QLinearGradient gradient(0, 0, 0, 520);
    gradient.setColorAt(0.0, QColor(90, 0, 157));
    gradient.setColorAt(1.0, QColor(0, 190, 255));

    QPixmap overlay("logo.jpg");
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
    QPixmap logoPixmap("logo.jpg");
    logoPixmap = logoPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPixmap);
    logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop);

    QLabel *welcome = new QLabel("<h3 style='color:white;'>Bine ai venit, reprezentant <i>" + QString::fromStdString(partner->getName()) + "</i></h3>");
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
    buttonContainer->setObjectName("partnerMenuButtonContainer");
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

    bool isAdmin = (staffID == 0);

    addButton("Înregistrare vânzare", [=]() {
        stack->addWidget(createRegisterSalePage(partner, stack, page));
        stack->setCurrentIndex(stack->count() - 1);
    });

    addButton("Consultare sold partener", [=]() {
        stack->addWidget(createPartnerBalancePage(partner, stack, page));
        stack->setCurrentIndex(stack->count() - 1);
    });

    if (isAdmin) {
        addButton("Adaugă angajat partener", [=]() {
            stack->addWidget(createAddPartnerAgentPage(partner, stack, page));
            stack->setCurrentIndex(stack->count() - 1);
        });

        addButton("Șterge angajat partener", [=]() {
            stack->addWidget(createDeletePartnerAgentPage(partner, stack, page));
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