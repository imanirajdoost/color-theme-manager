#include "theme_editor_page.h"
#include "ui_theme_editor_page.h"

theme_editor_page::theme_editor_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::theme_editor_page)
{
    ui->setupUi(this);
    // For drag and drops
    setAcceptDrops(true);

    colorList = new QGridLayout;
    listOfColors = new QList<ColorPair*>();
    QWidget* scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(colorList);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable (true);
    scrollArea->setWidget(scrollAreaContent);

    QHBoxLayout* colors_container = ui->colorLayout;
    colors_container->addLayout(colorList);
    colors_container->addWidget(scrollArea);

    //    ui->undoThemeNameButton->setVisible(false);
    ui->undoThemeNameButton->setStyleSheet("border-image: url(:/icons/trans.png);");
    hideProgress();
}

void theme_editor_page::update_colors()
{
    for(int i =0; i < colorList->count(); i++) {
//        ColorWidget* colW = (ColorWidget*)colorList->itemAt(i);
        //        disconnect(this,SIGNAL(saveAllColors()),colW,SLOT(saveColor()));
        //        disconnect(colW,SIGNAL(deleteMe(ColorPair*)),this,SLOT(deleteColor(ColorPair*)));
        colorList->itemAt(i)->widget()->deleteLater();
        colorList->removeWidget(colorList->itemAt(i)->widget());
    }
    for(int i =0; i < listOfColors->size(); i++)
    {
        ColorWidget* colorWidget = new ColorWidget(this);
        colorWidget->setColorRef(listOfColors->at(i));
        connect(this,SIGNAL(saveAllColors()),colorWidget,SLOT(saveColor()));
        connect(colorWidget,SIGNAL(deleteMe(ColorPair*)),this,SLOT(deleteColor(ColorPair*)));
        connect(colorWidget,SIGNAL(setEditedColor(bool)),this,SLOT(setColorEdited(bool)));
        colorList->addWidget(colorWidget);
    }
}

void theme_editor_page::setColorEdited(bool edited)
{
    std::cout << "TESTIIIIING" << std::endl;
    std::cout << "EDITED: " << (edited ? "1" : "0") << std::endl;
    isColorsChanged = edited;
}

theme_editor_page::~theme_editor_page()
{
    delete ui;
}

void theme_editor_page::receiveThemeData(Theme* theme){
    currentTheme = theme;
    // std::cout << "Theme name: " << currentTheme.themeName.toStdString() << std::endl;
    ui->themeNameEditText->setText(currentTheme->themeName);
    if(QString::compare(currentTheme->iconPath, "") != 0)
    {
        QImage image;
        if(image.load(currentTheme->iconPath))
        {
            image.scaledToWidth(ui->themeIcon->width(), Qt::SmoothTransformation);
            ui->themeIcon->setPixmap(QPixmap::fromImage(image));
        }
    }
    listOfColors = currentTheme->getColorPair();
    update_colors();
}

void theme_editor_page::on_addColorButton_clicked()
{
    ColorPair* newCol = new ColorPair("Color new","#ffffffff","#ffffffff");
    currentTheme->addColorPair(newCol);
    listOfColors = currentTheme->getColorPair();

    ColorWidget* colorWidget = new ColorWidget(this);
    colorWidget->setColorRef(listOfColors->at(listOfColors->size() - 1));
    connect(this,SIGNAL(saveAllColors()),colorWidget,SLOT(saveColor()));
    connect(colorWidget,SIGNAL(deleteMe(ColorPair*)),this,SLOT(deleteColor(ColorPair*)));
    connect(colorWidget,SIGNAL(setEditedColor(bool)),this,SLOT(setColorEdited(bool)));
    colorList->addWidget(colorWidget);
}

void theme_editor_page::applyOnFile(QString fileName)
{
    QByteArray fileData;
    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite))
    {
        add_message("Applying colors on the selected file...","black");

        int skipped = 0;
        int replaced = 0;

        fileData = file.readAll(); // read all the data into the byte array
        QString text(fileData); // add to text string for easy string replace

        //        std::cout << text.toStdString() << std::endl;

        for (int i=0; i < listOfColors->count(); i++)
        {
            ColorPair* pair = listOfColors->at(i);
            std::cout << "Replacing Color " + ColorPair::toRGBA(pair->getColorSource()).toStdString()
                      << std::endl;

            // Only replace color if it's not the same
            QString str1 = ColorPair::toRGBA(pair->getColorSource());
            QString str2 = ColorPair::toRGBA(pair->getColorTarget());
            if(QString::compare(str1, str2, Qt::CaseInsensitive))
            {
                QString rep = text.replace(QString(str1), QString(str2)); // replace text in string
                std::cout << "Replaced: " + rep.toStdString() << std::endl;
                if(rep.size() != 0) // it's not null or empty
                    replaced++;
            }
            else
                skipped++;
        }

        std::cout << text.toStdString() << std::endl;

        file.seek(0); // go to the beginning of the file
        file.write(text.toUtf8()); // write the new text back to the file

        file.close(); // close the file handle.

        add_message("Total colors replaced: " + QString::number(replaced),"black");
        add_message("Total colors skipped: " + QString::number(skipped),"black");

        QMessageBox *msgBox = new QMessageBox(this);
        QString txt = "Task finished!\nTotal colors replaced: " + QString::number(replaced) +
                "\nTotal skipped: " + QString::number(skipped);
        msgBox->setText(txt);
        msgBox->setWindowTitle("Info");
        msgBox->exec();
    }
    else
        add_message("Error openning the file", "red");
}


void theme_editor_page::on_applyOnFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"), "", tr("Text Files (*.xml *.txt *.php *.html *.htm)"));
    applyOnFile(fileName);
}

void theme_editor_page::add_message(QString m, QString color)
{
    QString errorHtml = "<font color=\"" + color + "\">";
    QString endHtml = "</font>";
    ui->errorText->appendHtml("[" + QTime::currentTime().toString() + "] "+ errorHtml + m + endHtml);
}

void theme_editor_page::downloadFinished()
{
    add_message(tr("Download finished"),"black");
    hideProgress();
}

void theme_editor_page::downloadProgress(qint64 ist, qint64 max)
{
    ui->progressBar->setVisible(true);
    ui->progressBar->setRange(0,max);
    ui->progressBar->setValue(ist);
    if(max < 0)
        hideProgress();
}

void theme_editor_page::hideProgress()
{
    ui->progressBar->setVisible(false);
}

bool theme_editor_page::isUrlValid(QString url)
{
    // regex pattern
    std::string pattern = ".*\\..*";

    // Construct regex object
    std::regex url_regex(pattern);

    // An url-string for example
    std::string my_url = url.toStdString();

    // Check for match
    if (std::regex_match(my_url, url_regex) == true) {
        return true;
    }
    return false;
}

void theme_editor_page::on_getFromWebButton_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Download from Internet"),
                                         tr("Paste the link here:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty()) {
        std::cout << text.toStdString() << std::endl;
        QUrl* url = new QUrl(text);
        if(url->isValid() && isUrlValid(url->toString()))
        {
            add_message("Starting download...","black");
            std::cout << "Url is valid" << std::endl;

            bool shouldReplace = false;
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Overwrite Colors",
                                          "Would you like to overwrite colors with the same id with "
                                          "the ones that exist in the link?\n"
                                          "If you choose YES, colors will be replaced,\n"
                                          "If you choose NO, all colors will be added as new colors",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                shouldReplace = true;
            }

            QNetworkAccessManager manager;
            QNetworkRequest request(*url);
            request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
            request.setUrl(*url);
            QNetworkReply *response = manager.get(request);
            QEventLoop event;
            connect(response, SIGNAL(finished()), &event, SLOT(quit()));
            connect(response, SIGNAL(finished()), this, SLOT(downloadFinished()));
            connect(response, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
            event.exec();
            QString content = response->readAll();
            std::cout << content.toStdString() << std::endl;

            XMLReader reader;
            reader.update(currentTheme, content, shouldReplace);

            listOfColors = currentTheme->getColorPair();

            update_colors();
        }
        else
        {
            add_message(tr("Invalid link, make sure you use a link that points to a valid theme file with colors"),"red");
        }
    }
    else
    {
        add_message(tr("Invalid link, make sure you use a link that points to a valid theme file with colors"),"red");
    }
}

void theme_editor_page::on_themeNameEditText_textChanged()
{
    QString newThemeName = ui->themeNameEditText->toPlainText();
    if(QString::compare(currentTheme->themeName,newThemeName)) {
        //        ui->undoThemeNameButton->setVisible(true);
        ui->undoThemeNameButton->setStyleSheet("border-image: url(:/icons/undo.png);");
        ui->themeNameEditText->setStyleSheet(QString("background-color: rgb(255, 142, 144);"));
        isThemeNameChanged = true;
    }
    else {
        //        ui->undoThemeNameButton->setVisible(false);
        ui->undoThemeNameButton->setStyleSheet("border-image: url(:/icons/trans.png);");
        ui->themeNameEditText->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
        isThemeNameChanged = false;
    }
}


void theme_editor_page::on_editThemeNameButton_clicked()
{

}

void theme_editor_page::on_saveThemeButton_clicked()
{
    if(isThemeNameChanged)
    {
        //Check if file name already exist:
        QString path = QDir::currentPath() + "/" + ui->themeNameEditText->toPlainText() + ".xml";
        QFileInfo check_file(path);
        // check if file exists and if yes: Is it really a file and no directory?
        if (check_file.exists() && check_file.isFile()) {
            QMessageBox messageBox;
            messageBox.critical(this,"Error","A theme with this name already exists, please change the theme name and try again.");
            messageBox.setFixedSize(500,200);
            return;
        }
        //If file doesn't exist...
        currentTheme->themeName = ui->themeNameEditText->toPlainText();
        //Rename the file
        QString oldPath = currentTheme->themePath;
        std::cout << "Old path: " + oldPath.toStdString() << std::endl;
        QFileInfo info(oldPath);
        QDir directory(info.dir());
        std::cout << "Info File Name: " + info.fileName().toStdString() << std::endl;
        std::cout << "File directory: " + info.absoluteDir().absolutePath().toStdString() << std::endl;
        if(directory.rename(info.fileName(),currentTheme->themeName + ".xml"))
            currentTheme->themePath = info.absoluteDir().absolutePath() + "/" + currentTheme->themeName + ".xml";

        emit sendNewThemeName(currentTheme->themeName);
        isThemeNameChanged = false;
    }
    if(isThemeIconChanged)
    {
        currentTheme->iconPath = newThemeIcon;
        emit sendNewThemeIcon(newThemeIcon);
        isThemeIconChanged = false;
    }

    emit saveAllColors();
    emit sendUpdatedTheme(currentTheme);
    this->close();
}

void theme_editor_page::deleteColor(ColorPair *col)
{
    for(int i = 0; i < listOfColors->size(); i++)
    {
        if(listOfColors->at(i) == col)
            listOfColors->removeAt(i);
    }
}


void theme_editor_page::on_editIconButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose image"),"" ,tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    if(QString::compare(fileName,"") != 0)
    {
        QImage image;
        if(image.load(fileName))
        {
            newThemeIcon = fileName;
            image.scaledToWidth(ui->themeIcon->width(), Qt::SmoothTransformation);
            //            image.scaledToHeight(ui->themeIcon->height(), Qt::SmoothTransformation);
            ui->themeIcon->setPixmap(QPixmap::fromImage(image));
            isThemeIconChanged = true;
        }
    }
}


void theme_editor_page::on_undoThemeNameButton_clicked()
{
    ui->themeNameEditText->setText(currentTheme->themeName);
    isThemeNameChanged = false;
    //    ui->undoThemeNameButton->setVisible(false);
    ui->undoThemeNameButton->setStyleSheet("border-image: url(:/icons/trans.png);");
    ui->themeNameEditText->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
}

void theme_editor_page::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void theme_editor_page::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        applyOnFile(fileName);
    }
}

void theme_editor_page::closeEvent(QCloseEvent *event)  // show prompt when user wants to close app
{
    if(isChangesPresent())
    {
        event->ignore();
        if (QMessageBox::Yes == QMessageBox::question(this, "Close Confirmation", "You have unsaved modifications, are you"
                                                      " sure you want to close the window without saving? (All your modifications"
                                                      " will be lost)", QMessageBox::Yes | QMessageBox::No))
        {
            event->accept();
        }
    }
    else
        event->accept();
}

bool theme_editor_page::isChangesPresent()
{
    if(isThemeIconChanged || isThemeNameChanged || isColorsChanged)
        return true;
//    for(int i =0; i < colorList->count(); i++) {
//        ColorWidget* colW = (ColorWidget*)colorList->itemAt(i);
//        if(colW->isChangesPresent())
//            return true;
//    }
    return false;
}
