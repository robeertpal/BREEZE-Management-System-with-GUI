#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QPalette>
#include <QLinearGradient>
#include <QMovie>
#include <QIcon>

#include "agentlogin.cpp"
#include "partnerlogin.cpp"

class MainWindow : public QMainWindow {
public:
    MainWindow() {
        stack = new QStackedWidget(this);
        setCentralWidget(stack);

        // === PAGINA PRINCIPALĂ ===
        QWidget *mainPage = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(mainPage);
        layout->setContentsMargins(15, 15, 15, 15);

        // === Fundal gradient + overlay logo ===
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
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(background));
        mainPage->setAutoFillBackground(true);
        mainPage->setPalette(palette);

        // === Logo stânga sus ===
        QLabel *logo = new QLabel();
        QPixmap logoPixmap(":/assets/logo.jpg");
        logoPixmap = logoPixmap.scaled(160, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        logo->setPixmap(logoPixmap);
        logo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        layout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop);

        // === Titlu + Subtitlu ===
        QLabel *authLabel = new QLabel("<h1 style='color:white;'>Autentificare</h1><p style='color:white;'>Agent sau Partener</p>");
        authLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(authLabel);

        // === Butoane de alegere ===
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *agentBtn = new QPushButton("Agent");
        QPushButton *partnerBtn = new QPushButton("Partener");
        agentBtn->setMinimumSize(130, 40);
        partnerBtn->setMinimumSize(130, 40);
        agentBtn->setStyleSheet("font-weight: bold; background-color: white; color: black; border-radius: 8px;");
        partnerBtn->setStyleSheet("font-weight: bold; background-color: white; color: black; border-radius: 8px;");
        buttonLayout->addStretch();
        buttonLayout->addWidget(agentBtn);
        buttonLayout->addSpacing(20);
        buttonLayout->addWidget(partnerBtn);
        buttonLayout->addStretch();
        layout->addLayout(buttonLayout);

        // === GIF loader (opțional) ===
        QLabel *gifLabel = new QLabel();
        QMovie *movie = new QMovie(":/loader.gif");
        if (movie->isValid()) {
            gifLabel->setMovie(movie);
            movie->start();
            gifLabel->setAlignment(Qt::AlignCenter);
            layout->addWidget(gifLabel);
        }

        layout->addStretch();

        // === Footer branding ===
        QLabel *footer = new QLabel("<h2 style='color:white;'>BREEZE Management System</h2><p style='color:white;'>by Pal Robert-Attila</p>");
        footer->setAlignment(Qt::AlignCenter);
        layout->addWidget(footer);

        // === Conectăm butoanele către login ===
        QObject::connect(agentBtn, &QPushButton::clicked, this, [=]() {
            QWidget *agentLogin = createAgentLogin(this, stack);
            stack->addWidget(agentLogin);
            stack->setCurrentWidget(agentLogin);
        });

        QObject::connect(partnerBtn, &QPushButton::clicked, this, [=]() {
            QWidget *partnerLogin = createPartnerLogin(this, stack);
            stack->addWidget(partnerLogin);
            stack->setCurrentWidget(partnerLogin);
        });

        // Adăugăm și setăm pagina principală
        stack->addWidget(mainPage);
        stack->setCurrentWidget(mainPage);

        // Setări fereastră
        setFixedSize(600, 520);
        setWindowTitle("BREEZE Management System");
        setWindowIcon(QIcon(":/logo.png"));
    }

private:
    QStackedWidget *stack;
};
