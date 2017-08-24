#include "editor.h"

#include <QPainter>
#include <QTextBlock>
#include <QTextStream>

Editor::Editor(QWidget *parent) :
    QPlainTextEdit(parent),
    IEditor(),
    lineNumberArea(new LineNumberArea(this)),
    highlight(NULL),
	filePath(QString())
{
    setWordWrapMode(QTextOption::NoWrap);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

Editor::~Editor()
{
    delete highlight;
}

void Editor::loadFile(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        filePath = path;
        QByteArray contents = file.readAll();
        file.close();
        insertPlainText(contents);
    }
}

void Editor::saveFile(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream stream(&file);
        stream << toPlainText();
        file.close();
    }
}

void Editor::saveFile()
{
    saveFile(filePath);
}

QString Editor::getFilePath()
{
    return filePath;
}

void Editor::setSyntax(QSyntaxHighlighter *syntax)
{
    highlight = syntax;
}

bool Editor::changes()
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray contents = file.readAll();
        return contents != toPlainText();
    }
    return false;
}

int Editor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }
    int space = 10 + fontMetrics().width(QLatin1Char('9')) * digits;
    return space;
}

void Editor::updateLineNumberAreaWidth(int)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void Editor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void Editor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void Editor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

void Editor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
