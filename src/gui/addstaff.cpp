#include <QWidget>
#include <QVBoxLayout>
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

#include "staff_manager.hpp"
#include "validators.hpp"
#include "exceptions.hpp"
#include "credentials.hpp"
#include "staff.hpp"
#include "credentials_manager.hpp"

QWidget* createAddStaffPage(QStackedWidget* stack, QWidget* backTargetPage) {
    QWidget *page = new QWidget();
    page->setObjectName("addStaffPage");

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

    // === Logo ===
    QLabel *logo = new QLabel();
    QPixmap logoPixmap(":/assets/logo.jpg");
    logoPixmap = logoPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo->setPixmap(logoPixmap);
    logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop);

    // === FORMULAR ===
    QWidget *form = new QWidget();
    QVBoxLayout *formLayout = new QVBoxLayout(form);
    formLayout->setAlignment(Qt::AlignCenter);

    auto createEdit = [](const QString &placeholder) {
        QLineEdit *edit = new QLineEdit();
        edit->setPlaceholderText(placeholder);
        edit->setStyleSheet("padding: 8px; border-radius: 6px; min-width: 250px;");
        return edit;
    };

    QLineEdit *firstName = createEdit("Prenume");
    QLineEdit *lastName = createEdit("Nume");
    QLineEdit *email = createEdit("Email");
    QLineEdit *phone = createEdit("Telefon");
    QDateEdit *birthDate = new QDateEdit(QDate::currentDate().addYears(-20));
    birthDate->setCalendarPopup(true);
    birthDate->setStyleSheet("padding: 8px; border-radius: 6px;");
    QLineEdit *nationality = createEdit("Naționalitate");
    QLineEdit *username = createEdit("Username");
    QLineEdit *password = createEdit("Parolă");
    password->setEchoMode(QLineEdit::Password);

    QComboBox *role = new QComboBox();
    role->addItems({"TicketAgent", "SecurityAgent", "TopUpAgent"});
    role->setStyleSheet("padding: 8px; border-radius: 6px;");

    formLayout->addWidget(firstName);
    formLayout->addWidget(lastName);
    formLayout->addWidget(email);
    formLayout->addWidget(phone);
    formLayout->addWidget(birthDate);
    formLayout->addWidget(nationality);
    formLayout->addWidget(username);
    formLayout->addWidget(password);
    formLayout->addWidget(role);

    QPushButton *submit = new QPushButton("Adaugă membru staff");
    submit->setStyleSheet("background-color: white; color: #5A009D; font-weight: bold; padding: 10px; border-radius: 8px;");
    formLayout->addWidget(submit);

    QPushButton *back = new QPushButton("Înapoi");
    back->setStyleSheet("color: white; background: transparent; font-style: italic;");
    formLayout->addWidget(back);

    mainLayout->addStretch();
    mainLayout->addWidget(form);
    mainLayout->addStretch();

    QObject::connect(submit, &QPushButton::clicked, [=]() {
        QString fn = firstName->text().trimmed();
        QString ln = lastName->text().trimmed();
        QString em = email->text().trimmed();
        QString ph = phone->text().trimmed();
        QDate bd = birthDate->date();
        QString nat = nationality->text().trimmed();
        QString un = username->text().trimmed().toLower();
        QString pw = password->text().trimmed();
        QString rl = role->currentText();

        if (fn.isEmpty() || ln.isEmpty() || em.isEmpty() || ph.isEmpty() || nat.isEmpty() || un.isEmpty() || pw.isEmpty()) {
            QMessageBox::warning(page, "Eroare", "Completează toate câmpurile.");
            return;
        }

        try {
            validateEmail(em.toStdString());
        } catch (const std::exception& e) {
            QMessageBox::warning(page, "Email invalid", e.what()); return;
        }

        try {
            validatePhoneNumber(ph.toStdString());
        } catch (const std::exception& e) {
            QMessageBox::warning(page, "Telefon invalid", e.what()); return;
        }

        try {
            validateDate(bd.day(), bd.month(), bd.year());
            validateAdult(bd.day(), bd.month(), bd.year());
        } catch (const std::exception& e) {
            QMessageBox::warning(page, "Dată invalidă", e.what()); return;
        }

        try {
            validateUsername(un.toStdString());
        } catch (const std::exception& e) {
            QMessageBox::warning(page, "Username invalid", e.what()); return;
        }

        if (pw.length() < 6) {
            QMessageBox::warning(page, "Parolă slabă", "Parola trebuie să aibă minim 6 caractere.");
            return;
        }

        try {
            Member m(fn.toStdString(), ln.toStdString(), Email(em.toStdString()), PhoneNumber(ph.toStdString()), Date(bd.day(), bd.month(), bd.year()), nat.toStdString());
            Credentials creds(un.toStdString(), pw.toStdString());

            std::shared_ptr<Staff> newStaff;
            if (rl == "TicketAgent") {
                newStaff = std::make_shared<TicketAgent>(m, creds);
            } else if (rl == "SecurityAgent") {
                newStaff = std::make_shared<SecurityAgent>(m, creds);
            } else {
                newStaff = std::make_shared<TopUpAgent>(m, creds);
            }

            auto staffList = loadStaffFromFile();
            newStaff->setStaffID(static_cast<int>(staffList.size()));
            staffList.push_back(newStaff);
            saveStaffToFile(staffList);

            auto credentials = loadCredentials();
            credentials.push_back({newStaff->getStaffID(), un.toStdString(), pw.toStdString()});
            saveCredentials(credentials);

            QMessageBox::information(page, "Succes", "Membru staff adăugat cu succes!");
            stack->setCurrentWidget(backTargetPage);
        } catch (const std::exception& e) {
            QMessageBox::critical(page, "Eroare la salvare", e.what());
        }
    });

    QObject::connect(back, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(backTargetPage);
    });

    return page;
}
