#include "mainwindow.h"
#include "nonogram.h"

MainWindow::MainWindow()
{
    path = tr("../nonograms/nonograms");
    defaultImage = "Seikaiha";
    defaultWindowSize = QSize(640, 480);
    backgroundImage = defaultImage;
    setWindowTitle(QString("Color Nonograms"));
    setWindowIcon(QIcon(tr(":/new/prefix1/images/win_icon.png")));
    filename = QString();
    nonogram = new Nonogram(this);

    createActions();
    createMenus();
    setStyleSheet(QString(".MainWindow {border-image: url(:/new/prefix1/images/%1.png) 20% repeat;}").arg(defaultImage));
    resize(defaultWindowSize);
    closeAct->setEnabled(false);
    saveAct->setEnabled(false);
    saveProgressAct->setEnabled(false);
    layout = new QHBoxLayout;
    layout->addWidget(nonogram);
    window = new QWidget(this);
    window->setLayout(layout);
    setCentralWidget(window);
}

void MainWindow::newFile()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Create New Nonogram");
    QFormLayout form(&dialog);

    QSpinBox* widthField = new QSpinBox(&dialog);
    widthField->setMinimum(10);
    widthField->setMaximum(35);
    widthField->setValue(15);
    QString widthLabel = QString("width");

    QSpinBox* heightField = new QSpinBox(&dialog);
    heightField->setMinimum(10);
    heightField->setMaximum(35);
    heightField->setValue(15);
    QString heightLabel = QString("height");

    QSpinBox* colorsField = new QSpinBox(&dialog);
    colorsField->setMinimum(3);
    colorsField->setMaximum(10);
    colorsField->setValue(5);
    QString colorsLabel = QString("colors");

    form.addRow(heightLabel, heightField);
    form.addRow(widthLabel, widthField);
    form.addRow(colorsLabel, colorsField);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if (dialog.exec() == QDialog::Accepted) {
        nonogram->~Nonogram();
        resize(defaultWindowSize);
        nonogram = new Nonogram(heightField->value(),widthField->value(),colorsField->value(), this);
        layout->addWidget(nonogram);

        saveAct->setEnabled(true);
        saveProgressAct->setEnabled(false);
        closeAct->setEnabled(true);
        setWindowTitle(QString("Color Nonograms - New Nonogram"));
    }
}

void MainWindow::open()
{
    filename = QFileDialog::getOpenFileName(this,tr("Open Nonogram"), path, tr("Pixmap Files (*xpm)"));
    if(!filename.isEmpty())
    {
        nonogram->~Nonogram();
        resize(defaultWindowSize);
        nonogram = new Nonogram(filename,this);
        layout->addWidget(nonogram);
        saveAct->setEnabled(false);
        saveProgressAct->setEnabled(true);
        closeAct->setEnabled(true);
        setWindowTitle(QString("Color Nonograms - "+filename));
    }
}

void MainWindow::save()
{
    QString str = QFileDialog::getSaveFileName(this,
                                            tr("Save Nonogram"), path,
                                            tr("X Pixmap Files (*.xpm)"));
    if(!str.isEmpty())
    {
        filename = str;
        nonogram->savePicture(filename);
        setWindowTitle(QString("Color Nonograms - "+filename));
    }

}
void MainWindow::saveProgress()
{
    nonogram->saveProgress(filename);
}
void MainWindow::load()
{
    QString str,str1;
    str = QFileDialog::getOpenFileName(this,tr("Load Nonogram"), path, tr("Portable Pixmap Files (*ppm)"));

    if(!str.isEmpty())
    {
        nonogram->~Nonogram();
        resize(defaultWindowSize);
        str1 = str.left(str.length() - 4)+".xpm";
        nonogram = new Nonogram(str1,this);
        nonogram->loadNonogram(str);
        layout->addWidget(nonogram);
        saveAct->setEnabled(false);
        saveProgressAct->setEnabled(true);
        closeAct->setEnabled(true);
        filename = str;
        setWindowTitle(QString("Color Nonograms - "+filename));
    }
}
void MainWindow::about()
{
    QMessageBox::about(this, tr("About"),
                       tr("<b>Color Nonograms</b> (2015)\nAuthor: Armine Vardazaryan."));
}
void MainWindow::closeNonogram()
{
    nonogram->~Nonogram();
    nonogram = new Nonogram(this);

    setWindowTitle(QString("Color Nonograms"));
    saveAct->setEnabled(false);
    saveProgressAct->setEnabled(false);
    closeAct->setEnabled(false);
}
void MainWindow::settings()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Settings"));
    QGroupBox* backgroundGroup = new QGroupBox(tr("Background"), &dialog);
    QGridLayout* groupLayout = new QGridLayout(backgroundGroup);

    QRadioButton* noBackgoundRadio = new QRadioButton(tr("No Background"),backgroundGroup);
    QRadioButton* backgoundRadio = new QRadioButton(tr("Use Background image"),backgroundGroup);
    QButtonGroup* buttonGroup = new QButtonGroup(backgroundGroup);
    backgoundRadio->setChecked(true);
    buttonGroup->addButton(noBackgoundRadio);
    buttonGroup->addButton(backgoundRadio);


    imagePreviewLabel = new QLabel(backgroundGroup);

    imagePreviewLabel->setBackgroundRole(QPalette::Base);
    imagePreviewLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imagePreviewLabel->setScaledContents(true);
    backgroundImage = defaultImage;
    QPixmap image(QString(":/new/prefix1/images/%1.png").arg(defaultImage));

    imagePreviewLabel->setPixmap(image);
    imagePreviewLabel->setFixedSize(150,150);

    QComboBox* imageComboBox = new QComboBox(backgoundRadio);
    imageComboBox->addItem("Seikaiha");
    imageComboBox->addItem("Chidori");
    imageComboBox->addItem("Hana");
    imageComboBox->addItem("Hana2");
    imageComboBox->addItem("Hana3");
    imageComboBox->addItem("Kikkou");
    imageComboBox->addItem("Shima");
    connect(imageComboBox, SIGNAL(activated(QString)),
            this, SLOT(updatePreview(QString)));

    connect(backgoundRadio, SIGNAL(clicked(bool)), imageComboBox, SLOT(setEnabled(bool)) );
    connect(noBackgoundRadio, SIGNAL(clicked(bool)), imageComboBox, SLOT(setDisabled(bool)) );

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                       Qt::Horizontal, &dialog);

    groupLayout->addWidget(noBackgoundRadio,0,0);
    groupLayout->addWidget(backgoundRadio, 0, 1);
    groupLayout->addWidget(imageComboBox,1, 1);
    groupLayout->addWidget(imagePreviewLabel, 2, 1);
    groupLayout->setRowMinimumHeight(2, 160);
    groupLayout->setColumnMinimumWidth(1, 160);
    groupLayout->addWidget(buttonBox, 3, 0,1,2);
    groupLayout->setAlignment(buttonBox, Qt::AlignCenter);
    groupLayout->setVerticalSpacing(20);

    connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    backgroundGroup->adjustSize();

    if (dialog.exec() == QDialog::Accepted)
    {
        changeStyle(backgoundRadio->isChecked());
    }

}
void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save Nonogram"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveProgressAct = new QAction(tr("Save &Progress"), this);
    connect(saveProgressAct, SIGNAL(triggered()), this, SLOT(saveProgress()));

    loadAct = new QAction(tr("&Load Progress"), this);
    connect(loadAct, SIGNAL(triggered()), this, SLOT(load()));

    closeAct = new QAction(tr("&Close"),this);
    connect(closeAct, SIGNAL(triggered()),this, SLOT(closeNonogram()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    settingsAct = new QAction(tr("&Settings"),this);
    connect(settingsAct, SIGNAL(triggered()), this, SLOT(settings()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveProgressAct);
    fileMenu->addAction(loadAct);
    fileMenu->addAction(closeAct);
    fileMenu->addAction(exitAct);

    formatMenu = menuBar()->addMenu(tr("F&ormat"));
    formatMenu->addAction(settingsAct);
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}
void MainWindow::changeStyle(const bool a)
{
    QString qss = "";
    if (a)
        setStyleSheet(QString(".MainWindow {border-image: url(:/new/prefix1/images/%1.png) 20% repeat;}").arg(backgroundImage));
    else
        setStyleSheet(QString(".MainWindow {border-image: ;}"));

}
void MainWindow::updatePreview(const QString str)
{
    QString imPath = QString(":/new/prefix1/images/%1.png").arg(str);
    QPixmap image(imPath);

    imagePreviewLabel->setPixmap(image);
    imagePreviewLabel->setFixedSize(150,150);
    backgroundImage = str;
}
