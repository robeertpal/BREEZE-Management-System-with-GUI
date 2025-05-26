#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QPainter>
#include <QPalette>
#include <QLinearGradient>
#include <QStackedWidget>

#include "guest_manager.hpp"
#include "guest.hpp"

QWidget* createTopUpPage(QStackedWidget* stack, QWidget* backTargetPage) {
    QWidget *page = new QWidget();
    page->setObjectName("topUpPage");

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

    // === LOGO ===
    QLabel *logo = new QLabel();
    QPixmap logoPixmap("logo.jpg");
    logoPixmap = logoPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPixmap);
    logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop);

    // === FORM ===
    QLabel *title = new QLabel("<h2 style='color:white;'>Încarcă sold brățară</h2>");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    QLineEdit *ticketCodeInput = new QLineEdit();
    ticketCodeInput->setPlaceholderText("Cod bilet");
    ticketCodeInput->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    mainLayout->addWidget(ticketCodeInput, 0, Qt::AlignCenter);

    QLineEdit *amountInput = new QLineEdit();
    amountInput->setPlaceholderText("Suma de încărcat (lei)");
    amountInput->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    mainLayout->addWidget(amountInput, 0, Qt::AlignCenter);

    QPushButton *submit = new QPushButton("Încarcă");
    submit->setStyleSheet("background-color: white; color: #5A009D; font-weight: bold; padding: 10px; border-radius: 8px;");
    mainLayout->addWidget(submit, 0, Qt::AlignCenter);

    QPushButton *back = new QPushButton("Înapoi");
    back->setStyleSheet("color:white; background: transparent; font-style: italic;");
    mainLayout->addWidget(back, 0, Qt::AlignCenter);

    // === LOGICĂ ===
    QObject::connect(submit, &QPushButton::clicked, [=]() {
        QString code = ticketCodeInput->text().trimmed();
        QString amountStr = amountInput->text().trimmed();

        if (code.isEmpty() || amountStr.isEmpty()) {
            QMessageBox::warning(page, "Eroare", "Completează toate câmpurile.");
            return;
        }

        bool ok;
        float amount = amountStr.toFloat(&ok);
        if (!ok || amount <= 0.0f) {
            QMessageBox::warning(page, "Eroare", "Suma trebuie să fie pozitivă.");
            return;
        }

        auto guests = loadGuestsFromFile();
        for (auto& guest : guests) {
            if (guest->getTicket()->getTicketCode() == code.toStdString()) {
                guest->getTicket()->getBracelet().topUp(amount);
                saveGuestsToFile(guests);
                QMessageBox::information(page, "Succes", "Brățara a fost încărcată cu succes.");
                stack->setCurrentWidget(backTargetPage);
                return;
            }
        }

        QMessageBox::critical(page, "Eroare", "Biletul nu a fost găsit.");
    });

    QObject::connect(back, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(backTargetPage);
    });

    return page;
}