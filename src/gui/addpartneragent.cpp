#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QStackedWidget>
#include <QPixmap>
#include <QPainter>
#include <QPalette>
#include <QLinearGradient>
#include <memory>
#include <algorithm>

#include "partner.hpp"
#include "partner_operations.hpp"
#include "partner_manager.hpp"

QWidget* createAddPartnerAgentPage(std::shared_ptr<Partner> partner, QStackedWidget* stack, QWidget* backTargetPage) {
    QWidget *page = new QWidget();
    page->setObjectName("addPartnerAgentPage");

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

    // === LOGO + TITLU ===
    QLabel *logo = new QLabel();
    QPixmap logoPixmap(":/assets/logo.jpg");
    logoPixmap = logoPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPixmap);
    logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(logo);

    QLabel *title = new QLabel("<h2 style='color:white;'>Adaugă angajat partener</h2>");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    QLineEdit *usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Username nou");
    usernameInput->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    mainLayout->addWidget(usernameInput, 0, Qt::AlignCenter);

    QLineEdit *passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Parolă nouă");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    mainLayout->addWidget(passwordInput, 0, Qt::AlignCenter);

    QPushButton *submitBtn = new QPushButton("Adaugă angajat");
    submitBtn->setStyleSheet("background-color: white; color: #5A009D; font-weight: bold; padding: 10px; border-radius: 8px;");
    mainLayout->addWidget(submitBtn, 0, Qt::AlignCenter);

    QPushButton *backBtn = new QPushButton("Înapoi");
    backBtn->setStyleSheet("color:white; background: transparent; font-style: italic;");
    mainLayout->addWidget(backBtn, 0, Qt::AlignCenter);

    // === FUNCȚIONALITATE ===
    QObject::connect(submitBtn, &QPushButton::clicked, [=]() {
        QString user = usernameInput->text().trimmed();
        QString pass = passwordInput->text().trimmed();

        if (user.isEmpty() || pass.isEmpty()) {
            QMessageBox::warning(page, "Eroare", "Completează toate câmpurile.");
            return;
        }

        try {
            auto creds = loadPartnerCredentials();
            for (const auto& c : creds) {
                if (c.username == user.toStdString()) {
                    QMessageBox::critical(page, "Eroare", "Acest username există deja.");
                    return;
                }
            }

            int maxID = -1;
            for (const auto& c : creds) {
                if (c.partnerID == partner->getID()) {
                    maxID = std::max(maxID, c.staffID);
                }
            }

            PartnerCredential newCred;
            newCred.partnerID = partner->getID();
            newCred.staffID = maxID + 1;
            newCred.username = user.toStdString();
            newCred.password = pass.toStdString();
            creds.push_back(newCred);

            savePartnerCredentials(creds);

            QMessageBox::information(page, "Succes", "Angajat partener adăugat cu succes!");
            stack->setCurrentWidget(backTargetPage);
        } catch (const std::exception& e) {
            QMessageBox::critical(page, "Eroare", e.what());
        }
    });

    QObject::connect(backBtn, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(backTargetPage);
    });

    return page;
}
