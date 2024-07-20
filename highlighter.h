#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class CppSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    CppSyntaxHighlighter(QTextDocument* parent = nullptr) : QSyntaxHighlighter(parent) {}

protected:
    void highlightBlock(const QString& text) override;

private:
    void applyFormat(const QString& text, const QRegularExpression& pattern, const QTextCharFormat& format);
};
