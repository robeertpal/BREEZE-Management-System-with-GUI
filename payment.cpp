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
#include <memory>
#include <sstream>

#include "guest_manager.hpp"
#include "guest.hpp"
#include "ticket.hpp"
#include "fee_strategy.hpp" // include strategia de taxare externă

QWidget* createPaymentPage(QStackedWidget* stack, QWidget* backTargetPage) {
    QWidget *page = new QWidget();
    page->setObjectName("paymentPage");

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

    // === Logo + titlu ===
    QLabel *logo = new QLabel();
    QPixmap logoPixmap("logo.jpg");
    logoPixmap = logoPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPixmap);
    logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop);

    QLabel *title = new QLabel("<h2 style='color:white;'>Plată cu brățară</h2>");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // === Formular ===
    QLineEdit *codeInput = new QLineEdit();
    codeInput->setPlaceholderText("Cod bilet");
    codeInput->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    mainLayout->addWidget(codeInput, 0, Qt::AlignCenter);

    QLineEdit *amountInput = new QLineEdit();
    amountInput->setPlaceholderText("Suma (lei)");
    amountInput->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    mainLayout->addWidget(amountInput, 0, Qt::AlignCenter);

    QPushButton *payBtn = new QPushButton("Plătește");
    payBtn->setStyleSheet("font-weight: bold; background-color: white; color:#5A009D; border-radius: 8px; padding: 10px;");
    mainLayout->addWidget(payBtn, 0, Qt::AlignCenter);

    QPushButton *back = new QPushButton("Înapoi");
    back->setStyleSheet("color:white; background: transparent; font-style: italic;");
    mainLayout->addWidget(back, 0, Qt::AlignCenter);

    // === Funcție plată ===
    QObject::connect(payBtn, &QPushButton::clicked, [=]() {
        QString code = codeInput->text().trimmed();
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
        for (auto& guest : guests) {
            auto ticket = guest->getTicket();
            if (ticket->getTicketCode() == code.toStdString()) {
                std::unique_ptr<FeeStrategy<float>> feeStrategy;

                if (dynamic_cast<VIPMeetAndGreet*>(ticket.get()))
                    feeStrategy = std::make_unique<MGFee<float>>();
                else
                    feeStrategy = std::make_unique<StandardFee<float>>();

                float fee = feeStrategy->calculateFee(amount);
                float total = amount + fee;

                QString warning = QString("Taxă de serviciu: %1 lei (%2%)\nTotal de plată: %3 lei.")
                                  .arg(fee, 0, 'f', 2)
                                  .arg((fee / amount) * 100, 0, 'f', 0)
                                  .arg(total, 0, 'f', 2);

                QMessageBox::StandardButton reply = QMessageBox::question(
                    page, "Confirmare plată", warning + "\nContinui?",
                    QMessageBox::Yes | QMessageBox::No
                );

                if (reply == QMessageBox::Yes) {
                    try {
                        ticket->getBracelet().pay(total);
                        saveGuestsToFile(guests);
                        QMessageBox::information(page, "Succes", "Plata a fost procesată cu succes.");
                        stack->setCurrentWidget(backTargetPage);
                    } catch (const InsufficientFunds& e) {
                        QMessageBox::critical(page, "Eroare", e.what());
                    }
                }
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