#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QPixmap>
#include <QPalette>
#include <QLinearGradient>
#include <QScrollArea>
#include <sstream>

#include "guest.hpp"

class ParticipantsDialog : public QDialog {
public:
    ParticipantsDialog(const std::vector<std::shared_ptr<Guest>>& guests, QWidget *parent = nullptr)
        : QDialog(parent) {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        setModal(true);
        resize(700, 500);

        // fundal transparent
        setStyleSheet("background-color: rgba(0, 0, 0, 150);");

        // container central alb (card)
        QWidget *card = new QWidget(this);
        card->setStyleSheet("background-color: white; border-radius: 12px;");
        card->setFixedSize(600, 420);
        card->move((width() - card->width()) / 2, (height() - card->height()) / 2);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setSpacing(10);
        cardLayout->setContentsMargins(20, 20, 20, 20);

        QLabel *title = new QLabel("Participanți înregistrați");
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet("font-weight: bold; font-size: 18px;");
        cardLayout->addWidget(title);

        // text scrollabil
        QTextEdit *text = new QTextEdit();
        text->setReadOnly(true);
        text->setStyleSheet("background-color: #ffffff; color: black; font-family: Consolas, monospace; font-size: 14px; padding: 8px; border-radius: 6px;");

        std::ostringstream oss;
        for (const auto& g : guests) {
            oss << *g << "\n";
        }
        text->setPlainText(QString::fromStdString(oss.str()));
        cardLayout->addWidget(text);

        // buton închidere
        QPushButton *backBtn = new QPushButton("Înapoi");
        backBtn->setFixedHeight(36);
        backBtn->setStyleSheet("background-color: #5A009D; color: white; font-weight: bold; border-radius: 6px;");
        QObject::connect(backBtn, &QPushButton::clicked, this, &ParticipantsDialog::accept);
        cardLayout->addWidget(backBtn);
    }
};

// Funcție accesibilă extern (pentru agentmenu)
QDialog* createParticipantsDialog(const std::vector<std::shared_ptr<Guest>>& guests, QWidget *parent = nullptr) {
    return new ParticipantsDialog(guests, parent);
}