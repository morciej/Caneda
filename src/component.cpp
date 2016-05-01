/***************************************************************************
 * Copyright (C) 2007 by Gopala Krishna A <krishna.ggk@gmail.com>          *
 * Copyright (C) 2012-2015 by Pablo Daniel Pareja Obregon                  *
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

#include "component.h"

#include "global.h"
#include "graphicsscene.h"
#include "library.h"
#include "port.h"
#include "propertydialog.h"
#include "settings.h"
#include "xmlutilities.h"

#include <QDebug>
#include <QFile>
#include <QPainter>

namespace Caneda
{
    //! \brief Constructs default empty ComponentData.
    ComponentData::ComponentData(GraphicsScene *scene)
    {
        properties = new PropertyGroup(scene);
    }

    /*!
     * \brief Constructs a ComponentData object from a ComponentDataPtr.
     *
     * Contructs a new ComponentData object from ComponentDataPtr. Special
     * care is taken to avoid copying the properties pointer, and copying
     * properties content (PropertyMap) instead. Otherwise, one would be
     * copying the reference to the PropertyGroup (properties) and all
     * components would share only one reference, modifying only one set
     * of properties data.
     */
    ComponentData::ComponentData(const QSharedDataPointer<ComponentData> &other, GraphicsScene *scene)
    {
        // Copy all data from given ComponentDataPtr
        name = other->name;
        filename = other->filename;
        displayText = other->displayText;
        labelPrefix = other->labelPrefix;
        description = other->description;
        library = other->library;
        ports = other->ports;

        // Recreate PropertyGroup (properties) as it is a pointer
        // and only internal data must be copied.
        properties = new PropertyGroup(scene);
        properties->setPropertyMap(other->properties->propertyMap());

        models = other->models;
    }

    /*!
     * \brief Constructs and initializes a default empty component item.
     *
     * \param scene Scene where this component belong to.
     */
    Component::Component(GraphicsScene *scene) :
        GraphicsItem(0, scene)
    {
        d = new ComponentData(scene);
        init();
    }

    //! \brief Constructs a component from \a other data.
    Component::Component(const ComponentDataPtr &other, GraphicsScene *scene) :
        GraphicsItem(0, scene)
    {
        d = new ComponentData(other, scene);
        init();
    }

    //! \brief Destructor.
    Component::~Component()
    {
        qDeleteAll(m_ports);
    }

    /*!
     * \brief Intialize the component.
     *
     * This method sets component's flags, adds initial label based on
     * default prefix value and adds the component's ports.
     *
     * The symbol corresponding to this component should already be
     * registered with LibraryManager using LibraryManager::registerComponent().
     */
    void Component::init()
    {
        // Set component flags
        setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
        setFlag(ItemSendsGeometryChanges, true);
        setFlag(ItemSendsScenePositionChanges, true);

        // Add component label
        Property _label("label", labelPrefix().append('1'), tr("Label"), true);
        d->properties->addProperty("label", _label);

        // Add component ports
        const QList<PortData*> portDatas = d.constData()->ports;
        foreach(const PortData *data, portDatas) {
            Port *port = new Port(this, data->name);
            port->setPos(data->pos);
            m_ports << port;
        }

        // Update component geometry
        updateBoundingRect();

        // Update properties text position
        d->properties->setParentItem(this);
        d->properties->setTransform(transform().inverted());
        d->properties->setPos(boundingRect().bottomLeft());
    }

    /*!
     * \brief Sets the label of component.
     *
     * This method also handles label prefix and number suffix appropriately.
     * In case the label doesn't start with the correct number prefix, the
     * return value is false.
     *
     * \param newLabel The label to be set.
     * \return True on success and false on failure.
     */
    bool Component::setLabel(const QString& newLabel)
    {
        if(!newLabel.startsWith(labelPrefix())) {
            return false;
        }

        d->properties->setPropertyValue("label", newLabel);
        return true;
    }

    //! \brief Returns the label's suffix part.
    QString Component::labelSuffix() const
    {
        QString _label = label();
        return _label.mid(labelPrefix().length());
    }

    /*!
     * \brief Returns the specified model of a component.
     *
     * Models are the representation of a component in different scenarios.
     * For example, a component can have certain syntax to be used in a spice
     * circuit, and a different one in a kicad schematic. Having a way to
     * extract information from our schematic and interpret it in different
     * ways allow us to export the circuit to other softwares and simulator
     * engines.
     *
     * Models should be always strings. Gouping several models into a QMap
     * provides a convenient way of handling them all together, and filter
     * them according to the export operation being used.
     *
     * \param type The type of model to return (for example, spice).
     * \return QString with the component's model.
     *
     * \sa \ref ModelsFormat.
     */
    QString Component::model(const QString& type) const
    {
        return d->models[type];
    }

    /*!
     * \brief Convenience static method to load a component saved as xml.
     *
     * This method loads a component saved as xml. Once the component name
     * and library are retrieved, a new component is created from LibraryManager
     * and its data is filled using the loadData() method.
     *
     * \param reader The xmlreader used to read xml data.
     * \param scene GraphicsScene to which component should be parented to.
     * \return Returns new component pointer on success and null on failure.
     *
     * \sa LibraryManager::componentData(), loadData()
     */
    Component* Component::loadComponent(Caneda::XmlReader *reader, GraphicsScene *scene)
    {
        Component *retVal = 0;
        Q_ASSERT(reader->isStartElement() && reader->name() == "component");

        QString compName = reader->attributes().value("name").toString();
        QString libName = reader->attributes().value("library").toString();

        Q_ASSERT(!compName.isEmpty());

        ComponentDataPtr data = LibraryManager::instance()->componentData(compName, libName);
        if(data.constData()) {
            retVal = new Component(data, scene);
        }

        if(retVal) {
            retVal->loadData(reader);
        }
        else {
            // Read to the end of the file if not found in any of Caneda libraries.
            qWarning() << "Warning: Found unknown element" << compName << ", skipping";
            reader->readUnknownElement();
        }
        return retVal;
    }

    /*!
     * \brief Loads current component data from \a Caneda::XmlReader.
     *
     * Loads current component data (name, library, position, properties
     * and transform) from \a Caneda::XmlReader.
     *
     * \sa loadComponentData(), saveData()
     */
    void Component::loadData(Caneda::XmlReader *reader)
    {
        Q_ASSERT(reader->isStartElement() && reader->name() == "component");

        d->name = reader->attributes().value("name").toString();
        d->library = reader->attributes().value("library").toString();

        setPos(reader->readPointAttribute("pos"));
        setTransform(reader->readTransformAttribute("transform"));

        while(!reader->atEnd()) {
            reader->readNext();

            if(reader->isEndElement()) {
                break;
            }

            if(reader->isStartElement()) {

                if(reader->name() == "properties") {
                    d->properties->readProperties(reader);
                }
                else {
                    qWarning() << "Warning: Found unknown element" << reader->name().toString();
                    reader->readUnknownElement();
                }

            }
        }
    }

    /*!
     * \brief Saves current component data to \a Caneda::XmlWriter.
     *
     * Saves current component data (name, library, position, properties
     * and transform) to \a Caneda::XmlWriter.
     *
     * \sa loadComponentData(), loadData()
     */
    void Component::saveData(Caneda::XmlWriter *writer) const
    {
        writer->writeStartElement("component");
        writer->writeAttribute("name", name());
        writer->writeAttribute("library", library());

        writer->writePointAttribute(pos(), "pos");
        writer->writeTransformAttribute(sceneTransform());

        d->properties->writeProperties(writer);

        writer->writeEndElement();  //</component>
    }

    /*!
     * \brief Paints a previously registered component.
     *
     * Takes care of painting a component on a scene. The component must be
     * previously registered using LibraryManager::registerComponent(). This
     * method also takes care of setting the correct global settings pen
     * according to its selection state.
     *
     * \sa LibraryManager::registerComponent()
     */
    void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
            QWidget *w)
    {
        // Paint the component symbol
        Settings *settings = Settings::instance();
        LibraryManager *libraryManager = LibraryManager::instance();
        QPainterPath symbol = libraryManager->symbolCache(name(), library());

        // Save pen
        QPen savedPen = painter->pen();

        if(option->state & QStyle::State_Selected) {
            // If selected, the paint is performed without the pixmap cache
            painter->setPen(QPen(settings->currentValue("gui/selectionColor").value<QColor>(),
                                 settings->currentValue("gui/lineWidth").toInt()));

            painter->drawPath(symbol);  // Draw symbol
        }
        else if(painter->worldTransform().isScaling()) {
            // If zooming, the paint is performed without the pixmap cache
            painter->setPen(QPen(settings->currentValue("gui/lineColor").value<QColor>(),
                                 settings->currentValue("gui/lineWidth").toInt()));

            painter->drawPath(symbol);  // Draw symbol
        }
        else {
            // Else, a pixmap cached is used
            QPixmap pix = libraryManager->pixmapCache(name(), library());
            QRect rect =  symbol.boundingRect().toRect();
            rect.adjust(-1.0, -1.0, 1.0, 1.0);  // Adjust rect to avoid clipping when size = 1px in any dimension
            painter->drawPixmap(rect, pix);
        }

        // Restore pen
        painter->setPen(savedPen);
    }

    //! \copydoc GraphicsItem::copy()
    Component* Component::copy(GraphicsScene *scene) const
    {
        Component *component = new Component(d, scene);
        GraphicsItem::copyDataTo(component);
        return component;
    }

    //! \copydoc GraphicsItem::launchPropertiesDialog()
    int Component::launchPropertiesDialog()
    {
        return d->properties->launchPropertiesDialog();
    }

    //! \brief Returns the rect adjusted to accomodate ports too.
    QRectF Component::adjustedBoundRect(const QRectF& rect)
    {
        QRectF adjustedRect = rect;
        foreach(Port *port, m_ports) {
            adjustedRect |= portEllipse.translated(port->pos());
        }
        return adjustedRect;
    }

    //! \brief React to change of item position.
    QVariant Component::itemChange(GraphicsItemChange change,
            const QVariant &value)
    {
        if(change == ItemTransformHasChanged) {
            // Set the inverse of component's matrix to the PropertyGroup
            // (properties) so that it maintains identity when transformed.
            d->properties->setTransform(transform().inverted());
        }
        return GraphicsItem::itemChange(change, value);
    }

    //! \brief Updates the bounding rect of this item.
    void Component::updateBoundingRect()
    {
        // Get the bounding rect of the symbol
        QPainterPath symbol = LibraryManager::instance()->symbolCache(name(), library());

        // Get an adjusted rect for accomodating extra stuff like ports.
        QRectF adjustedRect = adjustedBoundRect(symbol.boundingRect());

        // Set symbol bounding rect
        setShapeAndBoundRect(QPainterPath(), adjustedRect);
    }

} // namespace Caneda
