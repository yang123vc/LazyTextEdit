// Copyright 2009 Anders Bakken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef TEXTLAYOUT_P_H
#define TEXTLAYOUT_P_H

#include <QPointer>
#include <QList>
#include <QTextLayout>
#include <QRect>
#include <QString>
#include <QSize>
#include <QTextLine>
#include <QKeyEvent>
#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
#endif
#include "textdocument.h"
#include "textedit.h"
#include "syntaxhighlighter.h"

#ifndef QT_NO_DEBUG_STREAM
QDebug &operator<<(QDebug &str, const QTextLine &line);
#endif

class TextEdit;
class TextLayout
{
public:
    enum { MinimumBufferSize = 5000, LeftMargin = 3 };
    TextLayout(TextDocument *doc = 0)
        : document(doc), textEdit(0), bufferPosition(0),
          viewportPosition(0), layoutEnd(-1), viewport(-1),
          visibleLines(-1), lastVisibleCharacter(-1), lastBottomMargin(0),
          widest(-1), layoutDirty(true), sectionsDirty(true), lineBreaking(true)
    {
    }

    ~TextLayout()
    {
        qDeleteAll(textLayouts);
        qDeleteAll(unusedTextLayouts);
    }

    TextDocument *document;
    TextEdit *textEdit;
    QPointer<SyntaxHighlighter> syntaxHighlighter;
    int bufferPosition, viewportPosition, layoutEnd, viewport, visibleLines, lastVisibleCharacter, lastBottomMargin, widest;
    bool layoutDirty, sectionsDirty, lineBreaking;
    QList<QTextLayout*> textLayouts, unusedTextLayouts;
    QHash<QTextLayout*, QTextBlockFormat> blockFormats;
    QList<TextEdit::ExtraSelection> extraSelections;
    QList<QPair<int, QTextLine> > lines; // int is start position of line in document coordinates
    QRect contentRect; // contentRect means the laid out area, not just the area currently visible
    QString buffer;
    QList<TextSection*> sections; // these are all the sections in the buffer. Some might be before the current viewport
    QFont font;

    void dirty(int width);
    QList<TextSection*> relayoutCommon(); // should maybe be smarter about MinimumScreenSize. Detect it based on font and viewport size
    void relayoutByPosition(int size);
    void relayoutByGeometry(int height);

    int viewportWidth() const;

    int doLayout(int index, QList<TextSection*> *sections);

    QTextLine lineForPosition(int pos, int *offsetInLine = 0, int *lineIndex = 0) const;
    QTextLayout *layoutForPosition(int pos, int *offset = 0, int *index = 0) const;

    int textPositionAt(const QPoint &pos) const;

    enum Direction {
        Forward = 0,
        Backward = TextDocument::FindBackward
    };
    void updatePosition(int pos, Direction direction);

    inline int bufferOffset() const { return viewportPosition - bufferPosition; }
    QString bufferRead(int from, int size) const
    {
        if (from < bufferPosition || from + size > bufferPosition + buffer.size()) {
            return document->read(from, size);
        }
        return buffer.mid(from - bufferPosition, size);
    }
    inline QChar bufferReadCharacter(int index) const // document coordinates
    {
        if (index >= bufferPosition && index < bufferPosition + buffer.size()) {
            return buffer.at(index - bufferPosition);
        } else {
            Q_ASSERT(index >= 0 && index < document->documentSize()); // what if index == documentSize?
            return document->readCharacter(index);
        }
    }
};

#endif
