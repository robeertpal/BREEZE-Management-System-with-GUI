#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <QPalette>
#include <QLinearGradient>
#include <QStackedWidget>
#include <sstream>

#include "guest_manager.hpp"
#include "guest.hpp"
#include "ticket.hpp"

QWidget* createTicketInfoPage(QStackedWidget* stack, QWidget* backTargetPage) {
    QWidget *page = new QWidget();
    page->setObjectName("ticketInfoPage");

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
    logoPixmap = logoPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPixmap);
    logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop);

    // === Form centrat ===
    QWidget *center = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(center);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *title = new QLabel("<h2 style='color:white;'>Informații bilet</h2>");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QLineEdit *codeInput = new QLineEdit();
    codeInput->setPlaceholderText("Introduceți codul biletului");
    codeInput->setStyleSheet("padding: 10px; border-radius: 8px; min-width: 250px;");
    layout->addWidget(codeInput);

    QPushButton *search = new QPushButton("Afișează informații");
    search->setStyleSheet("background-color: white; color: #5A009D; font-weight: bold; padding: 10px; border-radius: 8px;");
    layout->addWidget(search);

    QTextEdit *result = new QTextEdit();
    result->setReadOnly(true);
    result->setVisible(false);
    result->setStyleSheet(R"(
        QTextEdit {
            background-color: rgba(255, 255, 255, 180);
            color: black;
            font-family: Consolas, monospace;
            font-size: 14px;
            padding: 8px;
            border-radius: 8px;
        }
        QScrollBar:vertical, QScrollBar:horizontal {
            width: 0px;
            height: 0px;
        }
    )");
    layout->addWidget(result);

    QPushButton *back = new QPushButton("Înapoi");
    back->setStyleSheet("color:white; background: transparent; font-style: italic;");
    layout->addWidget(back);

    mainLayout->addStretch();
    mainLayout->addWidget(center);
    mainLayout->addStretch();

    // === FUNCȚIONALITATE ===
    QObject::connect(search, &QPushButton::clicked, [=]() {
        QString code = codeInput->text().trimmed();
        if (code.isEmpty()) {
            result->setVisible(true);
            result->setHtml("<span style='color:red;'>Introdu un cod de bilet.</span>");
            return;
        }

        std::vector<std::shared_ptr<Guest>> guests = loadGuestsFromFile();
        bool found = false;

        for (const auto& g : guests) {
            if (g->getTicket()->getTicketCode() == code.toStdString()) {
                std::ostringstream oss;
                const auto& m = g->getMember();
                const auto& t = g->getTicket();

                oss << "<b>" << m.getFirstName() << " " << m.getLastName() << "</b><br>";
                oss << "Email: " << m.getEmail().getEmail() << "<br>";
                oss << "Telefon: " << m.getPhone().getNumber() << "<br>";
                oss << "Naționalitate: " << m.getNationality() << "<br>";
                oss << "Data nașterii: " << m.getBirthDate().toString() << "<br><br>";
                oss << "Cod bilet: " << t->getTicketCode() << "<br>";
                oss << "Sold brățară: " << t->getBracelet().getBalance() << " lei<br>";
                oss << "Check-in: " << (g->isCheckedIn() ? "✅ Da" : "❌ Nu") << "<br>";

                result->setHtml(QString::fromStdString(oss.str()));
                result->setVisible(true);
                found = true;
                break;
            }
        }

        if (!found) {
            result->setHtml("<span style='color:red;'>Biletul nu a fost găsit.</span>");
            result->setVisible(true);
        }
    });

    QObject::connect(back, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(backTargetPage);
    });

    return page;
}