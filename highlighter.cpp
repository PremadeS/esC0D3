#include "highlighter.h"
void CppSyntaxHighlighter::highlightBlock(const QString& text)
{
    // Define highlighting formats
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor("#9cdcfe"));

    QTextCharFormat commentFormat;
    commentFormat.setForeground(Qt::darkGreen);

    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor("#ce9178"));

    QTextCharFormat controlFormat;
    controlFormat.setForeground(QColor("#c586c0"));

    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor("#a0bb97"));

    QTextCharFormat loopFormat;
    loopFormat.setForeground(QColor("#c586c0"));

    QTextCharFormat dataTypeFormat;
    dataTypeFormat.setForeground(QColor("#569cd6"));

    QTextCharFormat extraDataTypeFormat;
    extraDataTypeFormat.setForeground(QColor("#4ebc7d"));

    QTextCharFormat bracketFormat;
    bracketFormat.setForeground(QColor("#ccaa00"));

    QTextCharFormat operatorsFormat;
    operatorsFormat.setForeground(QColor("#569cd6"));


    // Define regular expressions for various parts of the syntax

    QRegularExpression bracketPattern("[\\{\\}\\[\\]\\(\\)]");

    QRegularExpression keywordPattern("\\b(cout|cin|class|struct|namespace||static|extern|virtual|override|public|private|protected|)\\b");

    QRegularExpression controlPattern("\\b(using|if|else|for|while|do|switch|case|default|return|break|continue|typedef|template|typename|sizeof)\\b|#include\\b");



    QRegularExpression commentPattern("//[^\n]*|/\\*.*\\*/");

    QRegularExpression stringPattern("\"(?:\\\\.|[^\"])*\"");

    QRegularExpression numberPattern("\\b\\d+\\.?\\d*\\b");

    QRegularExpression operatorsPattern("\\+\\+|--|\\+=|-=|\\*=|/=|%=");
    // Loop keywords
    QRegularExpression loopPattern("\\b(for|while|do)\\b");

    // Data types
    QRegularExpression dataTypePattern("\\b(int|float|double|char|void|auto|bool|true|false)\\b");

    QRegularExpression extraDataTypePattern("\\b(endl|std|iostream|ifstream|ofstream|fstream|ostream|istream|string|wstring|u16string|u32string|vector|list|deque|forward_list|set|multiset|unordered_set|unordered_multiset|map|multimap|unordered_map|unordered_multimap)\\b");

    // Apply formatting based on patterns
    applyFormat(text, keywordPattern, keywordFormat);
    applyFormat(text, commentPattern, commentFormat);
    applyFormat(text, stringPattern, stringFormat);
    applyFormat(text, numberPattern, numberFormat);
    applyFormat(text, loopPattern, loopFormat);
    applyFormat(text, dataTypePattern, dataTypeFormat);
    applyFormat(text, bracketPattern, bracketFormat);
    applyFormat(text, controlPattern, controlFormat);
    applyFormat(text, extraDataTypePattern, extraDataTypeFormat);
    applyFormat(text, operatorsPattern, operatorsFormat);
}
void CppSyntaxHighlighter::applyFormat(const QString& text, const QRegularExpression& pattern, const QTextCharFormat& format)
{
    QRegularExpressionMatchIterator matchIterator = pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), format);
    }
}
