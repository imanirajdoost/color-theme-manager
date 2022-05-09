#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // For drag and drops
    setAcceptDrops(true);
    // For adding scrolls for themes
    themeList = new QGridLayout;
//    QList<Theme> listOfThemes();
    listOfThemes = new QList<Theme*>();
    QWidget* scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(themeList);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable (true);
    scrollArea->setWidget (scrollAreaContent);

    QHBoxLayout* themes_container = ui->themeLayout;
    themes_container->addLayout(themeList);
    themes_container->addWidget(scrollArea);
}

void MainWindow::update_themes()
{
    for(int i =0; i < listOfThemes->size(); i++)
    {
        ThemeWidget* themeWidget = new ThemeWidget(this);
        themeWidget->setThemeReference(listOfThemes->at(i));
//        std::cout << "Theme ref  = " << (listOfThemes.at(i)).themeName.toStdString() << std::endl;
//        QList<ColorPair> l = listOfThemes.at(i).getColorPair();
//        for(int j=0; j < l.count(); j++) {
//            std::cout << "COL: " << l.at(j).getId().toStdString() << std::endl;
//        }
        themeList->addWidget(themeWidget);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        openFile(fileName);
    }
}

void MainWindow::on_button_import_theme_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"), "", tr("XML files (*.xml)"));
    openFile(fileName);
}

void MainWindow::openFile(QString fileName)
{
    if(!fileName.isEmpty()) {
        XMLReader reader;
        QFile file;
        file.setFileName(fileName);
        reader.read(file,listOfThemes);

        update_themes();
    }
    else {
        add_error("No file or wrong extension was loaded");
        //std::cout << "ERROR: No file was found" << std::endl;
    }
}

void MainWindow::add_error(QString m)
{
    ui->text_error->appendPlainText("[" + QTime::currentTime().toString() + "] " + m);
}

