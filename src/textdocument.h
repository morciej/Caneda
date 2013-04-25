/***************************************************************************
 * Copyright (C) 2010 by Gopala Krishna A <krishna.ggk@gmail.com>          *
 * Copyright (C) 2010-2013 by Pablo Daniel Pareja Obregon                  *
 *                                                                         *
 * This is free software; you can redistribute it and/or modify            *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2, or (at your option)     *
 * any later version.                                                      *
 *                                                                         *
 * This software is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this package; see the file COPYING.  If not, write to        *
 * the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,   *
 * Boston, MA 02110-1301, USA.                                             *
 ***************************************************************************/

#ifndef TEXTDOCUMENT_H
#define TEXTDOCUMENT_H

#include "idocument.h"

// Forward declarations.
class QTextDocument;

namespace Caneda
{
    // Forward declarations.
    class TextEdit;

    /*!
     * \brief This class represents the text document interface
     * implementation.
     *
     * This class represents the actual document interface
     * (scene), in a manner similar to Qt's Graphics View Architecture.
     *
     * This class manages document specific methods like saving and
     * loading, as well as containing the actual document. The document
     * itself is included as a pointer to QTextDocument, that contains all the
     * document specific methods.
     *
     * \sa IContext, IDocument, IView, \ref DocumentViewFramework
     * \sa TextContext, TextView
     */
    class TextDocument : public IDocument
    {
        Q_OBJECT

    public:
        TextDocument();
        virtual ~TextDocument();

        // IDocument interface methods
        virtual IContext* context();

        virtual bool isModified() const;

        virtual bool canUndo() const;
        virtual bool canRedo() const;

        virtual void undo();
        virtual void redo();

        virtual QUndoStack* undoStack();

        virtual bool canCut() const { return true; }
        virtual bool canCopy() const { return true; }
        virtual bool canPaste() const { return true; }

        virtual void cut();
        virtual void copy();
        virtual void paste();

        virtual void selectAll();

        virtual void intoHierarchy();
        virtual void popHierarchy();

        virtual void alignTop() {}
        virtual void alignBottom() {}
        virtual void alignLeft() {}
        virtual void alignRight() {}
        virtual void distributeHorizontal() {}
        virtual void distributeVertical() {}
        virtual void centerHorizontal() {}
        virtual void centerVertical() {}

        virtual void simulate();

        virtual bool printSupportsFitInPage() const { return false; }
        virtual void print(QPrinter *printer, bool fitInView);
        virtual void exportImage(QPaintDevice &device, qreal width = -1, qreal height = -1,
                                 Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio) {}
        virtual QSizeF documentSize();

        virtual bool load(QString *errorMessage = 0);
        virtual bool save(QString *errorMessage = 0);

        virtual IView* createView();

        virtual void launchPropertiesDialog() {}
        // End of IDocument interface methods

        QTextDocument* textDocument() const { return m_textDocument; }

        void pasteTemplate(const QString& text);

    private Q_SLOTS:
        void onContentsChanged();

    private:
        TextEdit* activeTextEdit();
        QTextDocument *m_textDocument;
    };

} // namespace Caneda

#endif //TEXTDOCUMENT_H
