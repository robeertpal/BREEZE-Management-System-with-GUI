#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <QPalette>
#include <QLinearGradient>
#include <QStackedWidget>
#include <sstream>

#include "guest.hpp"
#include "guest_manager.hpp"
#include "ticket.hpp"

QWidget* createParticipantsPage(QStackedWidget* stack, QWidget* backTargetPage) {
    QWidget *page = new QWidget();
    page->setObjectName("participantsPage");

    QVBoxLayout *mainLayout = new QVBoxLayout(page);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(0);

    // === BACKGROUND: gradient + overlay ===
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

    // === Titlu ===
    QLabel *title = new QLabel("<h2 style='color:white;'>Participanți înregistrați</h2>");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // === Text box ===
    QTextEdit *text = new QTextEdit();
    text->setReadOnly(true);
    text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    text->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    text->setStyleSheet(R"(
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

    std::vector<std::shared_ptr<Guest>> guests = loadGuestsFromFile();
    std::ostringstream oss;
    for (const auto& g : guests) {
        const auto& m = g->getMember();
        oss << "<b>" << m.getFirstName() << " " << m.getLastName() << "</b><br>";
        oss << "Email: " << m.getEmail().getEmail() << "<br>";
        oss << "Telefon: " << m.getPhone().getNumber() << "<br>";
        oss << "Naționalitate: " << m.getNationality() << "<br>";
        oss << "Data nașterii: " << m.getBirthDate().toString() << "<br>";

        const auto& ticket = g->getTicket();
        oss << "Bilet: " << ticket->getTicketCode() << "<br>";

        // === Tip bilet fără getTypeName() ===
        if (dynamic_cast<GeneralAccess*>(ticket.get()))
            oss << "Tip bilet: General Access<br>";
        else if (dynamic_cast<GeneralVIP*>(ticket.get()))
            oss << "Tip bilet: General VIP<br>";
        else if (dynamic_cast<VIPMeetAndGreet*>(ticket.get()))
            oss << "Tip bilet: VIP Meet & Greet<br>";
        else
            oss << "Tip bilet: Necunoscut<br>";

        oss << "Sold brățară: " << ticket->getBracelet().getBalance() << " lei<br>";
        oss << "Check-in: " << (g->isCheckedIn() ? "Da" : "Nu") << "<br><br>";
    }
    text->setHtml(QString::fromStdString(oss.str()));
    mainLayout->addWidget(text);

    // === Buton Înapoi ===
    QPushButton *backBtn = new QPushButton("Înapoi");
    backBtn->setMinimumHeight(36);
    backBtn->setStyleSheet("background-color: white; color: #5A009D; font-weight: bold; border-radius: 6px;");
    QObject::connect(backBtn, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(backTargetPage);
    });
    mainLayout->addWidget(backBtn);

    return page;
}