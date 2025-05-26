#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QStackedWidget>
#include <QPixmap>
#include <QPainter>
#include <QPalette>
#include <QLinearGradient>

#include "staff_manager.hpp"
#include "credentials_manager.hpp"

QWidget* createDeleteStaffPage(QStackedWidget* stack, QWidget* backTargetPage) {
    QWidget *page = new QWidget();
    page->setObjectName("deleteStaffPage");

    QVBoxLayout *mainLayout = new QVBoxLayout(page);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // === BACKGROUND ===
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

    // === Logo și titlu ===
    QLabel *logo = new QLabel();
    QPixmap logoPixmap("logo.jpg");
    logoPixmap = logoPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPixmap);
    logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop);

    QLabel *title = new QLabel("<h2 style='color:white;'>Șterge membru staff</h2>");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    QLineEdit *usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Introdu username-ul");
    usernameInput->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    mainLayout->addWidget(usernameInput, 0, Qt::AlignCenter);

    QPushButton *deleteBtn = new QPushButton("Șterge");
    deleteBtn->setStyleSheet("background-color: white; color:#5A009D; font-weight: bold; padding: 10px; border-radius: 8px;");
    mainLayout->addWidget(deleteBtn, 0, Qt::AlignCenter);

    QPushButton *backBtn = new QPushButton("Înapoi");
    backBtn->setStyleSheet("color:white; background: transparent; font-style: italic;");
    mainLayout->addWidget(backBtn, 0, Qt::AlignCenter);

    // === Funcție ștergere ===
    QObject::connect(deleteBtn, &QPushButton::clicked, [=]() {
        QString username = usernameInput->text().trimmed().toLower();
        if (username.isEmpty()) {
            QMessageBox::warning(page, "Eroare", "Introdu un username.");
            return;
        }

        try {
            auto staffList = loadStaffFromFile();
            auto credentials = loadCredentials();

            auto credIt = std::find_if(credentials.begin(), credentials.end(),
                [&](const Credential& c) { return c.username == username.toStdString(); });

            if (credIt == credentials.end()) {
                QMessageBox::critical(page, "Eroare", "Username-ul nu a fost găsit.");
                return;
            }

            int staffIdToRemove = credIt->id;
            credentials.erase(credIt);
            saveCredentials(credentials);

            auto staffIt = std::remove_if(staffList.begin(), staffList.end(),
                [&](const std::shared_ptr<Staff>& s) {
                    return s->getStaffID() == staffIdToRemove;
                });

            if (staffIt != staffList.end()) {
                staffList.erase(staffIt, staffList.end());
                saveStaffToFile(staffList);
                QMessageBox::information(page, "Succes", "Membrul staff a fost șters cu succes.");
                stack->setCurrentWidget(backTargetPage);
            } else {
                QMessageBox::warning(page, "Avertisment", "Staff-ul nu a fost găsit în lista activă.");
            }
        } catch (const std::exception& e) {
            QMessageBox::critical(page, "Eroare", e.what());
        }
    });

    QObject::connect(backBtn, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(backTargetPage);
    });

    return page;
}