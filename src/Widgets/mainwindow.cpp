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
            openFile(stat->linksToFiles->at(i),stat->linksToIcons->at(i));
        add_message("Last session loaded successfully","black");
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
        connect(themeWidget,SIGNAL(updateTheme(Theme*)),this,SLOT(updateSingleTheme(Theme*)));
        connect(themeWidget,SIGNAL(deleteTheme(Theme*)),this,SLOT(deleteTheme(Theme*)));
        themeList->addWidget(themeWidget);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deleteTheme(Theme* _theme)
{
    QList<Theme*>* temp = new QList<Theme*>;

    for(int i=0; i < listOfThemes->count(); i++)
    {
        if(QString::compare(listOfThemes->at(i)->themeName,_theme->themeName) == 0)
        {
            //Delete the file
            QFile file (listOfThemes->at(i)->themePath);
            file.remove();
            continue;
        }
        temp->push_back(listOfThemes->at(i));
    }
//    listOfThemes->clear();
    listOfThemes = new QList<Theme*>;
    for(int i=0; i < temp->count(); i++) {
        listOfThemes->push_back(temp->at(i));
    }
//    listOfThemes = temp;

    updateSingleTheme(NULL);
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
        add_message("Opening dragged file " + fileName,"black");
        if(openFile(fileName,""))
            updateSingleTheme(listOfThemes->at(listOfThemes->count() - 1));
    }
}

void MainWindow::on_button_import_theme_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"), "", tr("XML files (*.xml)"));
    if(openFile(fileName,""))
        updateSingleTheme(listOfThemes->at(listOfThemes->count() - 1));
}

bool MainWindow::openFile(QString fileName, QString iconPath)
{
    if(!fileName.isEmpty()) {
        //Check if such file already exists
        for(int i = 0; i < listOfThemes->count(); i++)
        {
            if(QString::compare(listOfThemes->at(i)->themePath,fileName) == 0)
            {
                add_message("A theme with this name already exists, please change it before importing","red");

                QMessageBox messageBox;
                messageBox.critical(this,"Error","A theme with this name already exists, please change the theme name and try again.");
                messageBox.setFixedSize(500,200);

                return false;
            }
        }


        XMLReader reader;
        QFile file;
        file.setFileName(fileName);
        reader.read(file,listOfThemes, iconPath);
        //        updateSingleTheme(listOfThemes->at(listOfThemes->count() - 1));
        update_themes();
        add_message("File imported: " + fileName,"black");
        return true;
    }
    else {
        add_message("Error in loading file: No file or wrong extension was chosen","red");
        //std::cout << "ERROR: No file was found" << std::endl;
    }
    return false;
}

void MainWindow::updateSingleTheme(Theme* _theme)
{
    if(_theme != NULL)
        add_message("Theme Updated: " + _theme->themeName,"black");
    update_themes();

    if(_theme != NULL)
        add_message("Saving theme on disk: " + _theme->themeName,"black");

    stat = new SystemStat();
    XMLReader reader;
    for(int i = 0; i < listOfThemes->count(); i++) {
        Theme* _t = listOfThemes->at(i);
        stat->linksToFiles->push_back(_t->themePath);
        stat->linksToIcons->push_back(_t->iconPath);
        reader.writeTheme(_t);
    }
    reader.write(stat);
    add_message("Theme saved on disk","black");
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
    while(counter < 1000 && !saved)
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

    updateSingleTheme(newTheme);
}

void MainWindow::on_clearThemesButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm clearing list",
                                  "Are you sure you want to clear the themes list?\n"
                                  "This will NOT delete theme files from your hard drive",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        listOfThemes->clear();
        updateSingleTheme(NULL);
    }
}

