#include "syntaxhighlighter.h"
#include "textedit.h"
#include "textdocument.h"
#include "textlayout_p.h"
#include "textdocument_p.h"

SyntaxHighlighter::SyntaxHighlighter(QObject *parent)
    : QObject(parent), d(new Private)
{
}

SyntaxHighlighter::SyntaxHighlighter(TextEdit *parent)
    : QObject(parent), d(new Private)
{
    if (parent) {
        parent->setSyntaxHighlighter(this);
    }
}

SyntaxHighlighter::~SyntaxHighlighter()
{
    delete d;
}

void SyntaxHighlighter::setTextEdit(TextEdit *doc)
{
    if (d->textEdit)
        d->textEdit->setSyntaxHighlighter(0);
    d->textEdit = doc;
    if (d->textEdit)
        d->textEdit->setSyntaxHighlighter(this);
}
TextEdit *SyntaxHighlighter::textEdit() const
{
    return d->textEdit;
}


TextDocument * SyntaxHighlighter::document() const
{
    return d->textEdit ? d->textEdit->document() : 0;
}

void SyntaxHighlighter::rehighlight()
{
    if (d->textEdit) {
        Q_ASSERT(d->textLayout);
        d->textLayout->dirty(d->textEdit->viewport()->width());
    }
}

void SyntaxHighlighter::setFormat(int start, int count, const QTextCharFormat &format)
{
    ASSUME(d->textEdit);
    Q_ASSERT(start >= 0);
    Q_ASSERT(start + count <= d->currentBlock.size());
    d->formatRanges.append(QTextLayout::FormatRange());
    QTextLayout::FormatRange &range = d->formatRanges.last();
    range.start = start;
    range.length = count;
    range.format = format;
}

void SyntaxHighlighter::setFormat(int start, int count, const QColor &color)
{
    QTextCharFormat format;
    format.setForeground(color);
    setFormat(start, count, format);
}

void SyntaxHighlighter::setFormat(int start, int count, const QFont &font)
{
    QTextCharFormat format;
    format.setFont(font);
    setFormat(start, count, format);
}

void SyntaxHighlighter::setForeground(int start, int count, const QBrush &brush)
{
    QTextCharFormat format;
    format.setForeground(brush);
    setFormat(start, count, format);
}

void SyntaxHighlighter::setBackground(int start, int count, const QBrush &brush)
{
    QTextCharFormat format;
    format.setBackground(brush);
    setFormat(start, count, format);
}

QTextCharFormat SyntaxHighlighter::format(int pos) const
{
    QTextCharFormat ret;
    foreach(const QTextLayout::FormatRange &range, d->formatRanges) {
        if (range.start <= pos && range.start + range.length > pos) {
            ret.merge(range.format);
        } else if (range.start > pos) {
            break;
        }
    }
    return ret;
}


int SyntaxHighlighter::previousBlockState() const
{
    return d->previousBlockState;
}

int SyntaxHighlighter::currentBlockState() const
{
    return d->currentBlockState;
}

void SyntaxHighlighter::setCurrentBlockState(int s)
{
    d->previousBlockState = d->currentBlockState;
    d->currentBlockState = s; // ### These don't entirely follow QSyntaxHighlighter's behavior
}

int SyntaxHighlighter::currentBlockPosition() const
{
    return d->currentBlockPosition;
}

void SyntaxHighlighter::setBlockFormat(const QTextBlockFormat &format)
{
    d->blockFormat = format;
}
