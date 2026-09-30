#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QStackedWidget>
#include <QPainter>
#include <QPalette>
#include <QLinearGradient>
#include <QPixmap>

#include "guest_manager.hpp"
#include "guest_operations.hpp"

QWidget* createRestrictAccessPage(QStackedWidget* stack, QWidget* backTargetPage) {
    QWidget *page = new QWidget();
    page->setObjectName("restrictAccessPage");

    QVBoxLayout *mainLayout = new QVBoxLayout(page);
    mainLayout->setContentsMargins(15, 15, 15, 15);

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

    // === Logo + titlu ===
    QLabel *logo = new QLabel();
    QPixmap logoPixmap(":/assets/logo.jpg");
    logoPixmap = logoPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPixmap);
    logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop);

    QLabel *title = new QLabel("<h2 style='color:white;'>Restricționează acces participant</h2>");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // === Input cod bilet ===
    QLineEdit *codeInput = new QLineEdit();
    codeInput->setPlaceholderText("Cod bilet");
    codeInput->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    mainLayout->addWidget(codeInput, 0, Qt::AlignCenter);

    QPushButton *restrictBtn = new QPushButton("Restricționează acces");
    restrictBtn->setStyleSheet("background-color: white; color:#5A009D; font-weight: bold; padding: 10px; border-radius: 8px;");
    mainLayout->addWidget(restrictBtn, 0, Qt::AlignCenter);

    QPushButton *backBtn = new QPushButton("Înapoi");
    backBtn->setStyleSheet("color:white; background: transparent; font-style: italic;");
    mainLayout->addWidget(backBtn, 0, Qt::AlignCenter);

    // === LOGICĂ ===
    QObject::connect(restrictBtn, &QPushButton::clicked, [=]() {
        QString code = codeInput->text().trimmed();
        if (code.isEmpty()) {
            QMessageBox::warning(page, "Eroare", "Introdu un cod de bilet.");
            return;
        }

        auto guests = loadGuestsFromFile();
        bool found = false;

        for (const auto& g : guests) {
            if (g->getTicket()->getTicketCode() == code.toStdString()) {
                found = true;
                break;
            }
        }

        if (found) {
            removeGuestByTicketCode(guests, code.toStdString());
            saveGuestsToFile(guests);
            QMessageBox::information(page, "Succes", "Accesul participantului a fost restricționat.");
            stack->setCurrentWidget(backTargetPage);
        } else {
            QMessageBox::critical(page, "Eroare", "Codul nu a fost găsit.");
        }
    });

    QObject::connect(backBtn, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(backTargetPage);
    });

    return page;
}