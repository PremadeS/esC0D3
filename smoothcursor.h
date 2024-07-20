#ifndef SMOOTHCURSOR_H
#define SMOOTHCURSOR_H
#include <QApplication>
#include <QPlainTextEdit>
#include <QTimer>
#include <QPainter>
#include <QVBoxLayout>
#include <QWidget>

class SmoothBlinkingCursorPlainTextEdit : public QPlainTextEdit {
    Q_OBJECT

public:
    SmoothBlinkingCursorPlainTextEdit(QWidget *parent = nullptr)
        : QPlainTextEdit(parent), cursorOpacity(1.0), fadeDirection(-0.05) {
        // Set up the timer for smooth cursor blinking
        blinkTimer = new QTimer(this);
        connect(blinkTimer, &QTimer::timeout, this, &SmoothBlinkingCursorPlainTextEdit::updateCursorOpacity);
        blinkTimer->start(30); // Adjust the interval for blinking smoothness
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPlainTextEdit::paintEvent(event);

        // Custom cursor painting with varying opacity
        if (cursorOpacity > 0) {
            QPainter painter(viewport());
            painter.setOpacity(cursorOpacity);

            QTextCursor cursor = textCursor();
            QRect cursorRect = cursorRect(cursor);
            cursorRect.setWidth(2); // Set cursor width
            painter.fillRect(cursorRect, palette().text().color());
        }
    }

private slots:
    void updateCursorOpacity() {
        cursorOpacity += fadeDirection;
        if (cursorOpacity <= 0.0 || cursorOpacity >= 1.0) {
            fadeDirection = -fadeDirection;
        }
        viewport()->update();
    }

private:
    QTimer *blinkTimer;
    qreal cursorOpacity;
    qreal fadeDirection;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Smooth Fading Cursor QPlainTextEdit");

    QVBoxLayout *layout = new QVBoxLayout(&window);
    SmoothBlinkingCursorPlainTextEdit *plainTextEdit = new SmoothBlinkingCursorPlainTextEdit();
    plainTextEdit->setPlainText("This is a long text...\n" + QString(100, '\n') + "End of text.");

    layout->addWidget(plainTextEdit);
    window.resize(400, 300);
    window.show();

    return app.exec();
}


#endif // SMOOTHCURSOR_H
