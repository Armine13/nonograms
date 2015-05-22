#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "nonogram.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void newFile();
    void open();
    void save();
    void saveProgress();
    void load();
    void about();
    void closeNonogram();
    void settings();
    void changeStyle(const bool);
    void updatePreview(const QString str);

private:
    void createActions();
    void createMenus();

    QString filename;
    QString defaultImage;
    QSize defaultWindowSize;
    QString backgroundImage;
    QString path;
    Nonogram* nonogram;
    QLabel* imagePreviewLabel;
    QHBoxLayout *layout;
    QWidget *window;
    QMenu *fileMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveProgressAct;
    QAction *loadAct;
    QAction *closeAct;
    QAction *exitAct;
    QAction *settingsAct;
    QAction *aboutAct;

};


#endif // MAINWINDOW_H
