#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QTimer>
#include <QFontDatabase>

#include "mainwindow.cpp"  // Fără header, cum preferi

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // === Font personalizat dacă ai nevoie (exemplu) ===
    QFontDatabase::addApplicationFont(":/fonts/Montserrat-Bold.ttf");

    // === Splash screen cu logo festival ===
    QPixmap splashImage("logo.jpg");
    QSplashScreen splash(splashImage);
    splash.setFont(QFont("Montserrat", 10));
    splash.show();
    splash.showMessage("Se încarcă BREEZE Management System...", Qt::AlignBottom | Qt::AlignCenter, Qt::white);

    // === Fereastra principală ===
    MainWindow mainWin;

    // === Delay vizual frumos ===
    QTimer::singleShot(2000, &splash, &QSplashScreen::close);
    QTimer::singleShot(2000, &mainWin, [&mainWin]() {
        mainWin.show();
    });

    return app.exec();
}