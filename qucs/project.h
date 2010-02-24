/***************************************************************************
 * Copyright 2010 Pablo Daniel Pareja Obregon                              *
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

#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>

// Forward declarations
class ComponentsSidebar;
class Library;
class QToolButton;

class Project : public QWidget
{
    Q_OBJECT;

public:
    Project(QWidget *parent = 0);
    ~Project() {}

public Q_SLOTS:
    void slotNewProject();
    void slotOpenProject();
    void slotAddToProject();
    void slotRemoveFromProject();
    void slotCloseProject();

signals:
    void itemDoubleClicked(const QString& filename);

private Q_SLOTS:
    void slotOnDoubleClicked();

private:
    ComponentsSidebar *m_projectsSidebar;
    Library *projectLibrary;

    QToolButton *buttonBack, *buttonForward;
};

#endif //PROJECT_H
