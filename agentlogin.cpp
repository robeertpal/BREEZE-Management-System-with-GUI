#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include <QPalette>
#include <QLinearGradient>
#include <QMessageBox>
#include <QStackedWidget>
#include <memory>
#include <sstream>
#include <iostream>

#include "staff_operations.hpp"
#include "staff.hpp"
#include "agentmenu.cpp"

QWidget* createAgentLogin(QWidget *parent, QStackedWidget *stack) {
    QWidget *page = new QWidget(parent);
    page->setObjectName("agentLogin");

    QVBoxLayout *mainLayout = new QVBoxLayout(page);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // === BACKGROUND: Gradient + Logo overlay ===
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

    // === LOGO SUS STÂNGA ===
    QLabel *logo = new QLabel();
    QPixmap logoPixmap("logo.jpg");
    logoPixmap = logoPixmap.scaled(160, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPixmap);
    logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop);

    // === FORMULAR AUTENTIFICARE CENTRAT ===
    QWidget *center = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(center);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *avatar = new QLabel();
    QPixmap avatarPixmap("man.jpg");
    avatarPixmap = avatarPixmap.scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    avatar->setPixmap(avatarPixmap);
    avatar->setAlignment(Qt::AlignCenter);
    layout->addWidget(avatar);

    QLabel *title = new QLabel("<h2 style='color:white;'>Autentificare Agent</h2>");
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

    // === FUNCȚIE LOGIN ===
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

        std::vector<std::shared_ptr<Staff>> staffList = loadStaffFromFile();
        std::shared_ptr<Staff> staffCurent = loginStaff(staffList);

        std::cin.rdbuf(originalCin);  // restaurăm stdin

        if (staffCurent) {
            QMessageBox::information(page, "Succes", "Autentificare reușită!");
            QWidget *menuPage = createAgentMenu(staffCurent, stack);
            stack->addWidget(menuPage);
            stack->setCurrentWidget(menuPage);
        } else {
            QMessageBox::critical(page, "Eroare", "Credentiale greșite!");
        }
    });

    // === FUNCȚIE ÎNAPOI ===
    QObject::connect(back, &QPushButton::clicked, [=]() {
        stack->setCurrentIndex(0); // pagina principală
    });

    return page;
}