#include "codeeditor.h"
#include "linenumberarea.h"
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    installEventFilter(this);

    lineNumberArea = new LineNumberArea(this);

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);   //HighLight the curr line if u want :)

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

bool CodeEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        QTextCursor cursor = textCursor();
        switch (keyEvent->key()) {
        case Qt::Key_ParenLeft:
            insertPlainText("()");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);     //Moves the cursor one character left cuz itz convenient
            setTextCursor(cursor);
            return true;
        case Qt::Key_BracketLeft:
            insertPlainText("[]");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            setTextCursor(cursor);
            return true;
        case Qt::Key_BraceLeft:
            insertPlainText("{}");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            setTextCursor(cursor);
            return true;
        case Qt::Key_QuoteDbl:
            insertPlainText("\"\"");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            setTextCursor(cursor);
            return true;
        case Qt::Key_Apostrophe:
            insertPlainText("''");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            setTextCursor(cursor);
            return true;
        default:
            break;
        }
    }
    return QPlainTextEdit::eventFilter(obj, event);
}
int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 5 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}


void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}


void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor(Qt::darkGray);
        lineColor.setAlpha(20);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    QColor color(Qt::black);
    color.setAlpha(60);

    painter.fillRect(event->rect(), color);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            if (blockNumber == textCursor().blockNumber())      // Check if it's the current line
                painter.setPen(Qt::yellow);                  // Set color to yellow for the current line
            else
                painter.setPen(Qt::white);              // Set color to light gray for other lines :D


            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}


void CodeEditor::formatCode()
{
    QTextCursor cursor = textCursor();  //Take position of the cursor
    QString inputCode = toPlainText();
    QString formattedCode = runClangFormat(inputCode);
    setPlainText(formattedCode);
    setTextCursor(cursor);  //set the cursor to initial position after formatiing :)
}

QString CodeEditor::runClangFormat(const QString &code)
{
    QProcess process;
    process.start("clang-format", QStringList() << "-style=Google");    //Format using clang-format..... :)

    if (!process.waitForStarted()) {
        qWarning() << "Failed to start clang-format process";
        return code;
    }

    process.write(code.toUtf8());
    process.closeWriteChannel();

    if (!process.waitForFinished()) {
        qWarning() << "Clang-format process failed";
        return code;
    }

    QString formattedCode = process.readAllStandardOutput();
    return formattedCode;
}
