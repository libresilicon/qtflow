#ifndef VERILOG_H
#define VERILOG_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>

class VerilogHighlight : public QSyntaxHighlighter
{
		Q_OBJECT

public:
		VerilogHighlight(QTextDocument *parent = 0);

protected:
		void highlightBlock(const QString &text) override;

private:
		struct HighlightingRule
		{
				QRegularExpression pattern;
				QTextCharFormat format;
		};
		QVector<HighlightingRule> rules;

		QRegularExpression commentStartExpression;
		QRegularExpression commentEndExpression;

		QStringList keywordPatterns;

		QTextCharFormat keywordFormat;
		QTextCharFormat singleLineCommentFormat;
		QTextCharFormat multiLineCommentFormat;
};

#endif // VERILOG_H
