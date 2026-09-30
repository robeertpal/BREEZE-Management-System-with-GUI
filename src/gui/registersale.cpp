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
#include <sstream>

#include "partner.hpp"
#include "guest_manager.hpp"
#include "partner_operations.hpp"
#include "ticket.hpp"
#include "fee_strategy.hpp"

QWidget* createRegisterSalePage(std::shared_ptr<Partner> partner, QStackedWidget* stack, QWidget* backTargetPage) {
    QWidget *page = new QWidget();
    page->setObjectName("registerSalePage");

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

    QLabel *title = new QLabel("<h2 style='color:white;'>Înregistrare vânzare</h2>");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    QLineEdit *ticketCodeInput = new QLineEdit();
    ticketCodeInput->setPlaceholderText("Cod bilet participant");
    ticketCodeInput->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    mainLayout->addWidget(ticketCodeInput, 0, Qt::AlignCenter);

    QLineEdit *amountInput = new QLineEdit();
    amountInput->setPlaceholderText("Suma în lei");
    amountInput->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    mainLayout->addWidget(amountInput, 0, Qt::AlignCenter);

    QPushButton *registerBtn = new QPushButton("Înregistrează vânzarea");
    registerBtn->setStyleSheet("background-color: white; color: #5A009D; font-weight: bold; padding: 10px; border-radius: 8px;");
    mainLayout->addWidget(registerBtn, 0, Qt::AlignCenter);

    QPushButton *backBtn = new QPushButton("Înapoi");
    backBtn->setStyleSheet("color:white; background: transparent; font-style: italic;");
    mainLayout->addWidget(backBtn, 0, Qt::AlignCenter);

    // === LOGICĂ ===
    QObject::connect(registerBtn, &QPushButton::clicked, [=]() {
        QString code = ticketCodeInput->text().trimmed();
        QString amountStr = amountInput->text().trimmed();

        if (code.isEmpty() || amountStr.isEmpty()) {
            QMessageBox::warning(page, "Eroare", "Completează toate câmpurile.");
            return;
        }

        bool ok;
        float amount = amountStr.toFloat(&ok);
        if (!ok || amount <= 0.0f) {
            QMessageBox::warning(page, "Eroare", "Introdu o sumă validă (> 0).");
            return;
        }

        auto guests = loadGuestsFromFile();
        auto itGuest = std::find_if(guests.begin(), guests.end(), [&](const std::shared_ptr<Guest>& g) {
            return g->getTicket()->getTicketCode() == code.toStdString();
        });

        if (itGuest == guests.end()) {
            QMessageBox::critical(page, "Eroare", "Codul biletului nu a fost găsit.");
            return;
        }

        try {
            float commissionRate = 0.10f;
            float commission = amount * commissionRate;
            float total = amount + commission;

            QString msg = QString("Comision partener: %1 lei (%2%)\nTotal retras de pe brățară: %3 lei.\nContinui?")
                            .arg(commission, 0, 'f', 2)
                            .arg(commissionRate * 100, 0, 'f', 0)
                            .arg(total, 0, 'f', 2);

            QMessageBox::StandardButton confirm = QMessageBox::question(
                page, "Confirmare vânzare", msg,
                QMessageBox::Yes | QMessageBox::No
            );

            if (confirm != QMessageBox::Yes) return;

            (*itGuest)->getTicket()->getBracelet().pay(total);

            auto partners = loadPartnersFromFile();
            for (auto& p : partners) {
    if (p.getID() == partner->getID()) {
        p.receivePayment(amount);
        partner->receivePayment(amount);  // sincronizare și cu partenerul logat
        break;
    }
}

            saveGuestsToFile(guests);
            savePartnersToFile(partners);

            QMessageBox::information(page, "Succes", "Vânzare înregistrată cu succes.");
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
