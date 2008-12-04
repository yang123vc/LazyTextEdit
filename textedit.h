#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QtGui>
#include "textdocument.h"
#include "textcursor.h"
#include "syntaxhighlighter.h"

class TextEditPrivate;
class TextEdit : public QAbstractScrollArea
{
    Q_OBJECT
    Q_PROPERTY(int cursorWidth READ cursorWidth WRITE setCursorWidth)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly)
    Q_PROPERTY(bool cursorVisible READ cursorVisible WRITE setCursorVisible)
    Q_PROPERTY(QString selectedText READ selectedText)
    Q_PROPERTY(bool undoAvailable READ isUndoAvailable)
    Q_PROPERTY(bool redoAvailable READ isRedoAvailable)
    Q_PROPERTY(int maximumSizeCopy READ maximumSizeCopy WRITE setMaximumSizeCopy)
public:
    TextEdit(QWidget *parent = 0);
    ~TextEdit();

    TextDocument *document() const;
    void setDocument(TextDocument *doc);

    int cursorWidth() const;
    void setCursorWidth(int cc);

    void setSyntaxHighlighter(SyntaxHighlighter *highlighter);
    SyntaxHighlighter *syntaxHighlighter() const;

    bool load(QIODevice *device, TextDocument::DeviceMode mode = TextDocument::Sparse);
    bool load(const QString &fileName, TextDocument::DeviceMode mode = TextDocument::Sparse);
    void paintEvent(QPaintEvent *e);
    void scrollContentsBy(int dx, int dy);

    bool moveCursorPosition(TextCursor::MoveOperation op, TextCursor::MoveMode = TextCursor::MoveAnchor, int n = 1);
    void setCursorPosition(int pos, TextCursor::MoveMode mode = TextCursor::MoveAnchor);

    int viewportPosition() const;
    int cursorPosition() const;

    int textPositionAt(const QPoint &pos) const;

    bool readOnly() const;
    void setReadOnly(bool rr);

    int maximumSizeCopy() const;
    void setMaximumSizeCopy(int max);

    QRect cursorBlockRect() const;

    bool cursorVisible() const;
    void setCursorVisible(bool cc);

    QString selectedText() const;

    void save(QIODevice *device);
    bool hasSelection() const;

    void insert(int pos, const QString &text);
    void remove(int from, int size);

    TextCursor &textCursor();

    const TextCursor &textCursor() const;

    void setTextCursor(const TextCursor &textCursor);

    Section *sectionAt(const QPoint &pos) const;

    void ensureCursorVisible(const TextCursor &cursor, int linesMargin = 0);
    bool isUndoAvailable() const;
    bool isRedoAvailable() const;

    enum ActionType {
        CopyAction,
        PasteAction,
        CutAction,
        UndoAction,
        RedoAction,
        SelectAllAction
    };
    QAction *action(ActionType type) const;
public slots:
    void ensureCursorVisible();
    void append(const QString &text);
    void removeSelectedText();
    void copy(QClipboard::Mode mode = QClipboard::Clipboard);
    void paste(QClipboard::Mode mode = QClipboard::Clipboard);
    void cut();
    void undo();
    void redo();
    void selectAll();
    void clearSelection();
signals:
    void selectionChanged();
    void cursorPositionChanged(int pos);
    void sectionClicked(Section *section, const QPoint &pos);
    void undoAvailableChanged(bool on);
    void redoAvailableChanged(bool on);
protected:
    virtual void paste(const QString &ba); // int pos?
    virtual void changeEvent(QEvent *e);
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
    virtual void wheelEvent(QWheelEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void resizeEvent(QResizeEvent *e);
private:
    TextEditPrivate *d;
    friend TextLayout *qt_get_textLayout(TextEdit *edit);
    friend class TextEditPrivate;
    friend class TextCursor;
};

#endif
