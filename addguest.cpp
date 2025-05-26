#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QMessageBox>
#include <QStackedWidget>
#include <QPixmap>
#include <QPainter>
#include <QPalette>
#include <QLinearGradient>

#include "guest_operations.hpp"
#include "guest_manager.hpp"
#include "generator.hpp"
#include "exceptions.hpp"
#include "ticket_factory.hpp"

QWidget* createAddGuestPage(QStackedWidget* stack, QWidget* backTargetPage) {
    QWidget *page = new QWidget();
    page->setObjectName("addGuestPage");

    QVBoxLayout *mainLayout = new QVBoxLayout(page);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(0);

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

    // === Logo sus ===
    QLabel *logo = new QLabel();
    QPixmap logoPixmap("logo.jpg");
    logoPixmap = logoPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPixmap);
    logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop);

    // === FORMULAR ===
    QWidget *form = new QWidget();
    QVBoxLayout *formLayout = new QVBoxLayout(form);
    formLayout->setAlignment(Qt::AlignCenter);

    auto createLineEdit = [](const QString &placeholder) {
        QLineEdit *edit = new QLineEdit();
        edit->setPlaceholderText(placeholder);
        edit->setStyleSheet("padding: 8px; border-radius: 6px; min-width: 250px;");
        return edit;
    };

    QLineEdit *firstName = createLineEdit("Prenume");
    QLineEdit *lastName = createLineEdit("Nume");
    QLineEdit *email = createLineEdit("Email");
    QLineEdit *phone = createLineEdit("Telefon");
    QDateEdit *birthDate = new QDateEdit(QDate::currentDate().addYears(-18));
    birthDate->setCalendarPopup(true);
    birthDate->setStyleSheet("padding: 8px; border-radius: 6px;");
    QLineEdit *nationality = createLineEdit("Naționalitate");

    QComboBox *ticketType = new QComboBox();
    ticketType->addItems({ "General Access (GA)", "General VIP (GV)", "VIP Meet & Greet (MG)" });
    ticketType->setStyleSheet("padding: 8px; border-radius: 6px;");

    formLayout->addWidget(firstName);
    formLayout->addWidget(lastName);
    formLayout->addWidget(email);
    formLayout->addWidget(phone);
    formLayout->addWidget(birthDate);
    formLayout->addWidget(nationality);
    formLayout->addWidget(ticketType);

    QPushButton *submit = new QPushButton("Adaugă participant");
    submit->setStyleSheet("background-color: white; color: #5A009D; font-weight: bold; padding: 10px; border-radius: 8px;");
    formLayout->addWidget(submit);

    QPushButton *back = new QPushButton("Înapoi");
    back->setStyleSheet("color: white; background: transparent; font-style: italic;");
    formLayout->addWidget(back);

    mainLayout->addStretch();
    mainLayout->addWidget(form);
    mainLayout->addStretch();

    // === SUBMIT ===
    QObject::connect(submit, &QPushButton::clicked, [=]() {
        QString fn = firstName->text().trimmed();
        QString ln = lastName->text().trimmed();
        QString em = email->text().trimmed();
        QString ph = phone->text().trimmed();
        QDate bd = birthDate->date();
        QString nat = nationality->text().trimmed();
        QString ttRaw = ticketType->currentText();

        if (fn.isEmpty() || ln.isEmpty() || em.isEmpty() || ph.isEmpty() || nat.isEmpty()) {
            QMessageBox::warning(page, "Eroare", "Completează toate câmpurile.");
            return;
        }

        std::string typeCode;
        if (ttRaw.startsWith("General Access")) typeCode = "GA";
        else if (ttRaw.startsWith("General VIP")) typeCode = "GV";
        else if (ttRaw.startsWith("VIP Meet"))    typeCode = "MG";

        try {
            validateEmail(em.toStdString());
            validatePhoneNumber(ph.toStdString());
            validateDate(bd.day(), bd.month(), bd.year());
            validateAdult(bd.day(), bd.month(), bd.year());

            std::string code = generateTicketCode(
                fn.toStdString(), ln.toStdString(),
                bd.day(), bd.month(), bd.year(),
                typeCode
            );

            Member m(
                fn.toStdString(),
                ln.toStdString(),
                Email(em.toStdString()),
                PhoneNumber(ph.toStdString()),
                Date(bd.day(), bd.month(), bd.year()),
                nat.toStdString()
            );

            auto ticket = TicketFactory::createTicket(typeCode, code);
            auto guest = std::make_shared<Guest>(m, ticket);

            auto guests = loadGuestsFromFile();
            guests.push_back(guest);
            saveGuestsToFile(guests);

            QMessageBox::information(page, "Succes", "Participant adăugat cu succes!");
            stack->setCurrentWidget(backTargetPage);

        } catch (const std::exception& e) {
            QMessageBox::critical(page, "Eroare", e.what());
        }
    });

    // === ÎNAPOI ===
    QObject::connect(back, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(backTargetPage);
    });

    return page;
}