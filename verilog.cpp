#include "verilog.h"

VerilogHighlight::VerilogHighlight(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkGreen);
    keywordFormat.setFontWeight(QFont::Bold);
    keywordPatterns
            << "\\balways\\b" << "\\bassign\\b" << "\\bautomatic\\b"
            << "\\bbegin\\b" << "\\bend\\b"
            << "\\bcase\\b" << "\\bendcase\\b" << "\\bcasex\\b" << "\\bcasez\\b" << "\\bcell\\b" << "\\bconfig\\b"
            << "\\bdeassign\\b" << "\\bdefault\\b" << "\\bdefparam\\b" << "\\bdesign\\b" << "\\bdisable\\b"
            << "\\bedge\\b" << "\\bwait\\b"
            << "\\bif\\b" << "\\belse\\b"
            << "\\bfor\\b" << "\\bwhile\\b" << "\\brepeat\\b" << "\\bforever\\b"
            << "\\breg\\b"
            << "\\binput\\b" << "\\binout\\b" << "\\boutput\\b" << "\\bclock\\b"
            << "\\bvectored\\b" << "\\buse\\b"
            << "\\bwire\\b" << "\\bunsigned\\b"
            << "\\bttable\\b" << "\\btask\\b"
            << "\\bmodule\\b" << "\\bendmodule\\b";
    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        rules.append(rule);
    }

    singleLineCommentFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    rules.append(rule);

    multiLineCommentFormat.setForeground(Qt::blue);
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void VerilogHighlight::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, rules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0)
    {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
