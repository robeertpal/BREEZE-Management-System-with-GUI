#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>
#include <QPalette>
#include <QLinearGradient>
#include <QStackedWidget>

#include "guest_manager.hpp"
#include "guest_operations.hpp"

QWidget* createCheckInPage(QStackedWidget* stack, QWidget* backTargetPage) {
    QWidget *page = new QWidget();
    page->setObjectName("checkInPage");

    QVBoxLayout *mainLayout = new QVBoxLayout(page);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(0);

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

    // === Logo ===
    QLabel *logo = new QLabel();
    QPixmap logoPixmap(":/assets/logo.jpg");
    logoPixmap = logoPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPixmap);
    logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop);

    // === Formul check-in ===
    QWidget *center = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(center);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *title = new QLabel("<h2 style='color:white;'>Check-in participant</h2>");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    layout->addSpacing(10);

    QLineEdit *ticketCodeInput = new QLineEdit();
    ticketCodeInput->setPlaceholderText("Cod bilet");
    ticketCodeInput->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    layout->addWidget(ticketCodeInput);

    QPushButton *checkinButton = new QPushButton("Realizează check-in");
    checkinButton->setStyleSheet("font-weight: bold; background-color: white; color:#5A009D; border-radius: 8px; padding: 10px;");
    layout->addWidget(checkinButton);

    QPushButton *back = new QPushButton("Înapoi");
    back->setStyleSheet("color:white; background: transparent; font-style: italic;");
    layout->addWidget(back);

    mainLayout->addStretch();
    mainLayout->addWidget(center);
    mainLayout->addStretch();

    // === Acțiune check-in ===
    QObject::connect(checkinButton, &QPushButton::clicked, [=]() {
        QString code = ticketCodeInput->text().trimmed();
        if (code.isEmpty()) {
            QMessageBox::warning(page, "Eroare", "Introdu codul biletului.");
            return;
        }

        auto guests = loadGuestsFromFile();
        bool found = false;

        for (auto& guest : guests) {
            if (guest->getTicket()->getTicketCode() == code.toStdString()) {
                if (guest->isCheckedIn()) {
                    QMessageBox::information(page, "Check-in", "Participantul este deja check-in!");
                } else {
                    guest->checkIn();
                    saveGuestsToFile(guests);
                    QMessageBox::information(page, "Succes", "Check-in realizat cu succes!");
                }
                found = true;
                break;
            }
        }

        if (!found) {
            QMessageBox::critical(page, "Eroare", "Biletul nu a fost găsit!");
        }
    });

    // === Înapoi ===
    QObject::connect(back, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(backTargetPage);
    });

    return page;
}