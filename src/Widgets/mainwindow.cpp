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
    listOfThemes = new QList<Theme*>();
    QWidget* scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(themeList);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable (true);
    scrollArea->setWidget (scrollAreaContent);
    scrollArea->setAlignment(Qt::AlignTop);

    QHBoxLayout* themes_container = ui->themeLayout;
    themes_container->addLayout(themeList);
    themes_container->addWidget(scrollArea);

    ui->progressBar->setVisible(false);

    //Load the last session
    XMLReader xmlManager;
    stat = xmlManager.getLastStat();
    if(stat != NULL && stat->linksToFiles->count() > 0)
    {
        add_message("Loading session...","black");
        for(int i=0; i < stat->linksToFiles->count(); i++)
            openFile(stat->linksToFiles->at(i),false);
    }
}

void MainWindow::update_themes()
{

    while(QLayoutItem* item = themeList->layout()->takeAt(0))
    {
        Q_ASSERT( ! item->layout() ); // otherwise the layout will leak
        delete item->widget();
        delete item;
    }
    for(int i =0; i < listOfThemes->count(); i++)
    {
        ThemeWidget* themeWidget = new ThemeWidget(this);
        themeWidget->setThemeReference(listOfThemes->at(i));
        //        themeWidget->setMainWindowRef(this);
        //        connect(this,SLOT(updateSingleTheme(Theme*)),themeWidget,SIGNAL(updateTheme(Theme*)));
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
        openFile(fileName,true);
    }
}

void MainWindow::on_button_import_theme_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"), "", tr("XML files (*.xml)"));
    openFile(fileName,true);
}

void MainWindow::openFile(QString fileName, bool shouldWrite)
{
    if(!fileName.isEmpty()) {
        XMLReader reader;
        QFile file;
        file.setFileName(fileName);
        reader.read(file,listOfThemes);
        //        if(shouldWrite)
        //        {
        //            stat->linksToFiles->push_back(fileName);
        //            stat->linksToIcons->push_back("");
        //            reader.write(stat);
        //        }
        update_themes();
    }
    else {
        add_message("Error in loading file: No file or wrong extension was chosen","red");
        //std::cout << "ERROR: No file was found" << std::endl;
    }
}

void MainWindow::updateSingleTheme(Theme* _theme)
{
    add_message("Theme Updated: " + _theme->themeName,"black");
    update_themes();
}

void MainWindow::add_message(QString m, QString color)
{
    QString errorHtml = "<font color=\"" + color + "\">";
    QString endHtml = "</font>";
    ui->text_error->appendHtml("[" + QTime::currentTime().toString() + "] "+ errorHtml + m + endHtml);
    //    ui->text_error->appendPlainText("[" + QTime::currentTime().toString() + "] "+ errorHtml + m + endHtml);
}


void MainWindow::on_button_create_theme_clicked()
{
    ThemeWidget* themeWidget = new ThemeWidget(this);
    Theme* newTheme = new Theme();
    newTheme->setName("New Theme");
    ColorPair* newColorPair = new ColorPair("Color 0","#ffffffff","#ffffffff");
    newTheme->addColorPair(newColorPair);
    listOfThemes->push_back(newTheme);
    themeWidget->setThemeReference(listOfThemes->at(listOfThemes->size() - 1));
    themeList->addWidget(themeWidget);

    // Create the file
    QString fileName = QDir::currentPath() + "/" + newTheme->themeName + ".xml";
    std::cout << "FILE NAME: " << fileName.toStdString() << std::endl;
    int counter = 0;
    bool saved = false;
    QString finalPath = fileName;
    QString finalFileName = newTheme->themeName;
    while(counter < 100 && !saved)
    {
        QFileInfo check_file(finalPath);
        // check if file exists and if yes: Is it really a file and no directory?
        if (check_file.exists() && check_file.isFile())
        {
            finalPath = QDir::currentPath() + "/" + newTheme->themeName + QString::number(counter) + ".xml";
            finalFileName = newTheme->themeName + QString::number(counter);
        }
        else
        {
            newTheme->themePath = finalPath;
            newTheme->setName(finalFileName);
            themeWidget->setThemeReference(listOfThemes->at(listOfThemes->size() - 1));

            XMLReader xmlManager;
            xmlManager.writeTheme(newTheme);
            saved = true;
        }
        counter++;
    }
    if(!saved)
        add_message("Could not save theme, change your theme's name and try again","red");
    else
        add_message("Theme saved on disk at " + newTheme->themePath,"black");
}


void MainWindow::on_saveThemeButton_clicked()
{
    add_message("Saving themes...","black");
    XMLReader reader;
    for(int i =0; i< listOfThemes->count(); i++)
    {
        Theme* newTheme = listOfThemes->at(i);
        QString newPath = newTheme->themePath;
        if(QString::compare(newPath,"") == 0)
        {
            //            newPath = listOfThemes->at(i)->themeName + ".xml";
            QString fileName = QDir::currentPath() + "/" + newTheme->themeName + ".xml";
            std::cout << "FILE NAME: " << fileName.toStdString() << std::endl;
            int counter = 0;
            bool saved = false;
            QString finalPath = fileName;
            while(counter < 100 && !saved)
            {
                QFileInfo check_file(finalPath);
                // check if file exists and if yes: Is it really a file and no directory?
                if (check_file.exists() && check_file.isFile())
                {
                    finalPath = QDir::currentPath() + "/" + newTheme->themeName + QString::number(counter) + ".xml";
                }
                else
                {
                    newTheme->themePath = finalPath;

                    XMLReader xmlManager;
                    xmlManager.writeTheme(newTheme);
                    saved = true;
                }
                counter++;
            }
            if(!saved)
                add_message("Could not save theme, change your theme's name and try again","red");
            else
                add_message("Theme saved on disk at " + newTheme->themePath,"black");
        }
        //        stat->linksToFiles->push_back(newPath);
        //        stat->linksToIcons->push_back("");
        reader.write(stat);
        reader.writeTheme(newTheme);
    }
}

