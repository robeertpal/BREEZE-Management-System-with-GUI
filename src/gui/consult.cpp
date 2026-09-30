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

#include "guest_manager.hpp"

QWidget* createConsultBalancePage(QStackedWidget* stack, QWidget* backTargetPage) {
    QWidget *page = new QWidget();
    page->setObjectName("consultBalancePage");

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

    QLabel *title = new QLabel("<h2 style='color:white;'>Consultare sold brățară</h2>");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    QLineEdit *codeInput = new QLineEdit();
    codeInput->setPlaceholderText("Cod bilet");
    codeInput->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    mainLayout->addWidget(codeInput, 0, Qt::AlignCenter);

    QPushButton *checkBtn = new QPushButton("Afișează soldul");
    checkBtn->setStyleSheet("background-color: white; color:#5A009D; font-weight: bold; padding: 10px; border-radius: 8px;");
    mainLayout->addWidget(checkBtn, 0, Qt::AlignCenter);

    QPushButton *backBtn = new QPushButton("Înapoi");
    backBtn->setStyleSheet("color:white; background: transparent; font-style: italic;");
    mainLayout->addWidget(backBtn, 0, Qt::AlignCenter);

    // === Funcționalitate ===
    QObject::connect(checkBtn, &QPushButton::clicked, [=]() {
        QString code = codeInput->text().trimmed();
        if (code.isEmpty()) {
            QMessageBox::warning(page, "Eroare", "Introdu codul biletului.");
            return;
        }

        auto guests = loadGuestsFromFile();
        for (const auto& g : guests) {
            if (g->getTicket()->getTicketCode() == code.toStdString()) {
                float balance = g->getTicket()->getBracelet().getBalance();
                QString message = QString("Soldul disponibil este: <b>%1 lei</b>").arg(balance, 0, 'f', 2);
                QMessageBox::information(page, "Sold brățară", message);
                return;
            }
        }

        QMessageBox::critical(page, "Eroare", "Codul de bilet nu a fost găsit.");
    });

    QObject::connect(backBtn, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(backTargetPage);
    });

    return page;
}