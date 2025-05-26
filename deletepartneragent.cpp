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

#include "partner.hpp"
#include "partner_operations.hpp"

QWidget* createDeletePartnerAgentPage(std::shared_ptr<Partner> partner, QStackedWidget* stack, QWidget* backTargetPage) {
    QWidget *page = new QWidget();
    page->setObjectName("deletePartnerAgentPage");

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

    // === LOGO + TITLU ===
    QLabel *logo = new QLabel();
    QPixmap logoPixmap("logo.jpg");
    logoPixmap = logoPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPixmap);
    logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(logo);

    QLabel *title = new QLabel("<h2 style='color:white;'>Ștergere angajat partener</h2>");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    QLineEdit *idInput = new QLineEdit();
    idInput->setPlaceholderText("ID angajat de șters");
    idInput->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    mainLayout->addWidget(idInput, 0, Qt::AlignCenter);

    QPushButton *submitBtn = new QPushButton("Șterge angajat");
    submitBtn->setStyleSheet("background-color: white; color: #5A009D; font-weight: bold; padding: 10px; border-radius: 8px;");
    mainLayout->addWidget(submitBtn, 0, Qt::AlignCenter);

    QPushButton *backBtn = new QPushButton("Înapoi");
    backBtn->setStyleSheet("color:white; background: transparent; font-style: italic;");
    mainLayout->addWidget(backBtn, 0, Qt::AlignCenter);

    QObject::connect(submitBtn, &QPushButton::clicked, [=]() {
        QString idStr = idInput->text().trimmed();
        bool ok;
        int id = idStr.toInt(&ok);
        if (!ok || id < 0) {
            QMessageBox::warning(page, "Eroare", "Introduce un ID valid.");
            return;
        }

        auto creds = loadPartnerCredentials();
        auto it = std::remove_if(creds.begin(), creds.end(), [&](const PartnerCredential& c) {
            return c.partnerID == partner->getID() && c.staffID == id;
        });

        if (it == creds.end()) {
            QMessageBox::warning(page, "Eroare", "Angajatul nu a fost găsit sau nu aparține partenerului curent.");
            return;
        }

        creds.erase(it, creds.end());
        savePartnerCredentials(creds);

        QMessageBox::information(page, "Succes", "Angajatul a fost șters cu succes.");
        stack->setCurrentWidget(backTargetPage);
    });

    QObject::connect(backBtn, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(backTargetPage);
    });

    return page;
}