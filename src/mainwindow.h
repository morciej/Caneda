/***************************************************************************
 * Copyright (C) 2006 by Gopala Krishna A <krishna.ggk@gmail.com>          *
 * Copyright (C) 2009-2012 by Pablo Daniel Pareja Obregon                  *
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

#ifndef CANEDA_MAINWINDOW_H
#define CANEDA_MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

// Forward declarations
class QLabel;
class QUndoGroup;
class QUndoView;

namespace Caneda
{
    // Forward declarations
    class Action;
    class SidebarBrowser;
    class FolderBrowser;
    class Project;
    class TabWidget;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        ~MainWindow();

        static MainWindow* instance();

        TabWidget* tabWidget() const;
        QDockWidget* sidebarDockWidget() const;

        void saveSettings();
        void setNormalAction();
        Action* action(const QString &name);
        QMenu* menubarMenu(const QString &name, bool createOnAbsence = true);
        QUndoGroup *m_undoGroup;

    public Q_SLOTS:
        void slotFileNew();
        void slotFileOpen(QString fileName = QString());
        void slotFileSave();
        void slotFileSaveAs();
        bool slotFileSaveAll();
        void slotFileClose();
        void slotFilePrint();
        void slotExportImage();
        void slotAppSettings();

        void slotEditUndo();
        void slotEditRedo();
        void slotEditCut();
        void slotEditCopy();
        void slotEditPaste();
        void slotEditFind();
        void slotSelectAll();

        void openLayout();
        void openSchematic();
        void openSymbol();
        void openFileFormat(const QString &suffix);

        void slotZoomIn();
        void slotZoomOut();
        void slotZoomBestFit();
        void slotZoomOriginal();
        void slotSetZoom(int percentage);
        void slotSplitHorizontal();
        void slotSplitVertical();
        void slotCloseSplit();
        void slotViewToolBar(bool);
        void slotViewStatusBar(bool);

        void slotNewProject();
        void slotOpenProject(QString fileName = QString());
        void slotAddToProject();
        void slotRemoveFromProject();
        void slotCloseProject();
        void slotBackupAndHistory();

        void slotCallFilter();
        void slotCallLine();
        void slotCallMatch();
        void slotCallAtt();
        void slotCallLibrary();
        void slotImportData();

        void slotShowLastMsg();
        void slotShowLastNetlist();

        void slotHelpIndex();
        void slotHelpAbout();
        void slotHelpAboutQt();

        void setDocumentTitle(const QString& title);
        void updateTitle();
        void slotUpdateSettingsChanges();
        void slotStatusBarMessage(const QString& newPos);

    protected:
        void closeEvent(QCloseEvent *closeEvent);

    private Q_SLOTS:
        void initFile();
        void loadSettings();

    private:
        MainWindow(QWidget *w=0);
        void addAsDockWidget(QWidget *w, const QString &title = QString(),
                Qt::DockWidgetArea area = Qt::LeftDockWidgetArea);

        void initActions();
        void initMouseActions();
        void initMenus();
        void initToolBars();
        void initStatusBar();

        void createUndoView();
        void createFolderView();
        void setupSidebar();
        void setupProjectsSidebar();

        // menus contain the items of their menubar
        QMenu *fileMenu, *editMenu, *insMenu, *projMenu, *simMenu, *viewMenu,
              *docksMenu, *helpMenu, *alignMenu, *toolMenu;

        QLabel *m_statusLabel;
        QToolBar *fileToolbar, *editToolbar, *viewToolbar, *workToolbar;
        QDockWidget *m_sidebarDockWidget, *m_projectDockWidget,
                    *m_browserDockWidget,*m_undoDockWidget;
        Project *m_project;
        FolderBrowser *m_folderBrowser;
        QUndoView *undoView;
        TabWidget *m_tabWidget;
        QString titleText;
    };

} // namespace Caneda

#endif //CANEDA_MAINWINDOW_H
