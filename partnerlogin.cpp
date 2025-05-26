#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include <QPalette>
#include <QLinearGradient>
#include <QMessageBox>
#include <QStackedWidget>
#include <sstream>
#include <iostream>

#include "partner_operations.hpp"
#include "partner_manager.hpp"
#include "partner.hpp"
#include "partnermenu.cpp"

QWidget* createPartnerLogin(QWidget *parent, QStackedWidget *stack) {
    QWidget *page = new QWidget(parent);
    page->setObjectName("partnerLogin");

    QVBoxLayout *mainLayout = new QVBoxLayout(page);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // BACKGROUND
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

    // LOGO
    QLabel *logo = new QLabel();
    QPixmap logoPixmap("logo.jpg");
    logoPixmap = logoPixmap.scaled(160, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPixmap);
    logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop);

    // FORMULAR LOGIN
    QWidget *center = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(center);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *title = new QLabel("<h2 style='color:white;'>Autentificare Partener</h2>");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    layout->addSpacing(10);

    QLineEdit *username = new QLineEdit();
    username->setPlaceholderText("Username");
    username->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    layout->addWidget(username);

    QLineEdit *password = new QLineEdit();
    password->setPlaceholderText("Parola");
    password->setEchoMode(QLineEdit::Password);
    password->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    layout->addWidget(password);

    layout->addSpacing(10);

    QPushButton *login = new QPushButton("Autentificare");
    login->setStyleSheet("font-weight: bold; background-color: white; color:#5A009D; border-radius: 8px; padding: 10px;");
    layout->addWidget(login);

    QPushButton *back = new QPushButton("Înapoi");
    back->setStyleSheet("color:white; background: transparent; font-style: italic;");
    layout->addWidget(back);

    mainLayout->addStretch();
    mainLayout->addWidget(center);
    mainLayout->addStretch();

    // LOGIN LOGIC
    QObject::connect(login, &QPushButton::clicked, [=]() {
        QString user = username->text().trimmed();
        QString pass = password->text().trimmed();

        if (user.isEmpty() || pass.isEmpty()) {
            QMessageBox::warning(page, "Eroare", "Completează toate câmpurile!");
            return;
        }

        std::streambuf* originalCin = std::cin.rdbuf();
        std::istringstream input(user.toStdString() + "\n" + pass.toStdString() + "\n");
        std::cin.rdbuf(input.rdbuf());

        std::vector<Partner> partners = loadPartnersFromFile();
        std::shared_ptr<Partner> partner = loginPartnerStaff(partners);

        std::cin.rdbuf(originalCin);

        if (partner) {
            QMessageBox::information(page, "Succes", "Autentificare reușită!");

            // Determină staffID asociat
            int staffID = 0;
            auto creds = loadPartnerCredentials();
            for (const auto& c : creds) {
                if (c.partnerID == partner->getID() && c.username == user.toStdString()) {
                    staffID = c.staffID;
                    break;
                }
            }

            QWidget *menuPage = createPartnerMenu(partner, staffID, stack);
            stack->addWidget(menuPage);
            stack->setCurrentWidget(menuPage);
        } else {
            QMessageBox::critical(page, "Eroare", "Credentiale incorecte!");
        }
    });

    // ÎNAPOI
    QObject::connect(back, &QPushButton::clicked, [=]() {
        stack->setCurrentIndex(0);
    });

    return page;
}