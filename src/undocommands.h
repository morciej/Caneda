/***************************************************************************
 * Copyright (C) 2006 by Gopala Krishna A <krishna.ggk@gmail.com>          *
 * Copyright (C) 2010-2016 by Pablo Daniel Pareja Obregon                  *
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

#ifndef UNDO_COMMANDS_H
#define UNDO_COMMANDS_H

#include "global.h"
#include "property.h"

#include <QPair>
#include <QUndoCommand>

namespace Caneda
{
    // Forward declarations.
    class CGraphicsItem;
    class GraphicText;
    class Painting;
    class Port;
    class Wire;

    class MoveCmd : public QUndoCommand
    {
    public:
        MoveCmd(CGraphicsItem *item, const QPointF &init, const QPointF &final,
                QUndoCommand *parent = 0);

        void undo();
        void redo();

    private:
        CGraphicsItem *m_item;
        QPointF m_initialPos;
        QPointF m_finalPos;
    };

    class DisconnectCmd : public QUndoCommand
    {
    public:
        DisconnectCmd(Port *p1, Port *p2, QUndoCommand *parent = 0);

        void undo();
        void redo();

    private:
        Port * const m_port1;
        Port * const m_port2;
    };

    class AddWireCmd : public QUndoCommand
    {
    public:
        AddWireCmd(Wire *wire, CGraphicsScene *scene, QUndoCommand *parent = 0);

        void undo();
        void redo();

    private:
        Wire *m_wire;
        CGraphicsScene *m_scene;
    };

    class InsertItemCmd : public QUndoCommand
    {
    public:
        InsertItemCmd(CGraphicsItem *const item, CGraphicsScene *scene,
                QPointF pos, QUndoCommand *parent = 0);

        void undo();
        void redo();

    protected:
        CGraphicsItem *const m_item;
        CGraphicsScene *const m_scene;
        QPointF m_pos;
    };

    class RemoveItemsCmd : public QUndoCommand
    {
    public:
        typedef QPair<CGraphicsItem*, QPointF> ItemPointPair;

        RemoveItemsCmd(const QList<CGraphicsItem*> &items, CGraphicsScene *scene,
                QUndoCommand *parent = 0);

        void undo();
        void redo();

    protected:
        QList<ItemPointPair> m_itemPointPairs;
        CGraphicsScene *const m_scene;
    };

    class RotateItemsCmd : public QUndoCommand
    {
    public:
        RotateItemsCmd(const QList<CGraphicsItem*> &items, const  Caneda::AngleDirection,
                       CGraphicsScene *scene, QUndoCommand *parent = 0);

        void undo();
        void redo();

    protected:
        QList<CGraphicsItem*> m_items;
        Caneda::AngleDirection m_dir;
        CGraphicsScene *const m_scene;
    };

    class MirrorItemsCmd : public QUndoCommand
    {
    public:
        MirrorItemsCmd(const QList<CGraphicsItem*> items, const Qt::Axis axis,
                       CGraphicsScene *scene, QUndoCommand *parent = 0);

        void undo();
        void redo();

    protected:
        QList<CGraphicsItem*> m_items;
        Qt::Axis m_axis;
        CGraphicsScene *const m_scene;
    };

    class PaintingRectChangeCmd : public QUndoCommand
    {
    public:
        PaintingRectChangeCmd(Painting *paintng, QRectF oldRect, QRectF newRect,
                QUndoCommand *parent = 0);

        void undo();
        void redo();

    protected:
        Painting *const m_painting;
        QRectF m_oldRect;
        QRectF m_newRect;
    };

    class PaintingPropertyChangeCmd : public QUndoCommand
    {
    public:
        PaintingPropertyChangeCmd(Painting *painting, QString oldText,
                QUndoCommand *parent = 0);

        void undo();
        void redo();

    protected:
        Painting *const m_painting;
        QString m_oldPropertyText;
        QString m_newPropertyText;
    };

    class GraphicTextChangeCmd : public QUndoCommand
    {
    public:
        GraphicTextChangeCmd(GraphicText *text, QString oldText, QString newText,
                QUndoCommand *parent = 0);

        void undo();
        void redo();

    protected:
        GraphicText *const m_graphicText;
        QString m_oldText;
        QString m_newText;
    };

    class PropertyMapCmd : public QUndoCommand
    {
    public:
        PropertyMapCmd(PropertyGroup *propGroup, const PropertyMap& old, const PropertyMap& newMap,
                QUndoCommand *parent = 0);

        void undo();
        void redo();

    private:
        PropertyGroup *m_propertyGroup;
        PropertyMap m_oldMap;
        PropertyMap m_newMap;
    };

} // namespace Caneda

#endif //UNDO_COMMANDS_H
