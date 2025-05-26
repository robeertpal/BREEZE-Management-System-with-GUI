#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>
#include <QPalette>
#include <QLinearGradient>
#include <memory>

#include "partner.hpp"

QWidget* createPartnerBalancePage(std::shared_ptr<Partner> partner, QStackedWidget* stack, QWidget* backTargetPage) {
    QWidget *page = new QWidget();
    page->setObjectName("partnerBalancePage");

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

    QLabel *title = new QLabel("<h2 style='color:white;'>Consultare sold partener</h2>");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    float balance = partner->getBalance();
    QLabel *balanceLabel = new QLabel(QString("<p style='color:white; font-size: 18px;'>Sold actual: <b>%1 lei</b></p>").arg(balance, 0, 'f', 2));
    balanceLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(balanceLabel);

    QPushButton *backBtn = new QPushButton("Înapoi");
    backBtn->setStyleSheet("color:white; background: transparent; font-style: italic;");
    mainLayout->addWidget(backBtn, 0, Qt::AlignCenter);

    QObject::connect(backBtn, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(backTargetPage);
    });

    return page;
}