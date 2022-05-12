#include "theme_editor_page.h"
#include "ui_theme_editor_page.h"

theme_editor_page::theme_editor_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::theme_editor_page)
{
    ui->setupUi(this);

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

    ui->editThemeNameButton->setVisible(false);
}

void theme_editor_page::update_colors()
{
    for(int i =0; i < colorList->count(); i++) {
        ColorWidget* colW = (ColorWidget*)colorList->itemAt(i);
        disconnect(this,SIGNAL(saveAllColors()),colW,SLOT(saveColor()));
        disconnect(colW,SIGNAL(deleteMe(ColorPair*)),this,SLOT(deleteColor(ColorPair*)));
        colorList->itemAt(i)->widget()->deleteLater();
        colorList->removeWidget(colorList->itemAt(i)->widget());
    }
    for(int i =0; i < listOfColors->size(); i++)
    {
        ColorWidget* colorWidget = new ColorWidget(this);
        colorWidget->setColorRef(listOfColors->at(i));
        connect(this,SIGNAL(saveAllColors()),colorWidget,SLOT(saveColor()));
        connect(colorWidget,SIGNAL(deleteMe(ColorPair*)),this,SLOT(deleteColor(ColorPair*)));
        colorList->addWidget(colorWidget);
    }
}

theme_editor_page::~theme_editor_page()
{
    delete ui;
}

void theme_editor_page::receiveThemeData(Theme* theme){
    currentTheme = theme;
    // std::cout << "Theme name: " << currentTheme.themeName.toStdString() << std::endl;
    ui->themeNameEditText->setText(currentTheme->themeName);
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
    colorList->addWidget(colorWidget);
}


void theme_editor_page::on_applyOnFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"), "", tr("Text Files (*.xml *.txt *.php *.html *.htm)"));
    QByteArray fileData;
    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite))
    {
        int skipped = 0;
        int replaced = 0;

        fileData = file.readAll(); // read all the data into the byte array
        QString text(fileData); // add to text string for easy string replace

        std::cout << text.toStdString() << std::endl;

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

        QMessageBox *msgBox = new QMessageBox(this);
        QString txt = "Task finished!\nTotal colors replaced: " + QString::number(replaced) +
                "\nTotal skipped: " + QString::number(skipped);
        msgBox->setText(txt);
        msgBox->exec();
    }
    else
        std::cout << "Error openning file" << std::endl;


}

void theme_editor_page::on_getFromWebButton_clicked()
{
    bool ok;
    // Ask for birth date as a string.
    QString text = QInputDialog::getText(this, "Download from Internet",
                                         "Paste the link here:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty()) {
        std::cout << text.toStdString() << std::endl;
        QUrl* url = new QUrl(text);
        if(url->isValid())
        {
            std::cout << "Url is valid" << std::endl;
            QNetworkAccessManager manager;
            QNetworkRequest request(*url);
            request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
            request.setUrl(*url);
            QNetworkReply *response = manager.get(request);
            QEventLoop event;
            connect(response, SIGNAL(finished()), &event, SLOT(quit()));
            event.exec();
            QString content = response->readAll();
            std::cout << content.toStdString() << std::endl;

            //TODO: Now read the colors into the current theme
            XMLReader reader;
            reader.update(currentTheme, content);

            listOfColors = currentTheme->getColorPair();

            update_colors();
        }
    }
}

void theme_editor_page::on_themeNameEditText_textChanged()
{
    QString newThemeName = ui->themeNameEditText->toPlainText();
    if(QString::compare(currentTheme->themeName,newThemeName)) {
        ui->editThemeNameButton->setVisible(true);
        ui->themeNameEditText->setStyleSheet(QString("background-color: rgb(255, 142, 144);"));
        isThemeNameChanged = true;
    }
    else {
        ui->editThemeNameButton->setVisible(false);
        ui->themeNameEditText->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
        isThemeNameChanged = false;
    }
}


void theme_editor_page::on_editThemeNameButton_clicked()
{
    ui->themeNameEditText->setText(currentTheme->themeName);
    isThemeNameChanged = false;
    ui->editThemeNameButton->setVisible(false);
    ui->themeNameEditText->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
}

void theme_editor_page::on_saveThemeButton_clicked()
{
    if(isThemeNameChanged)
    {
        currentTheme->themeName = ui->themeNameEditText->toPlainText();
        emit sendNewThemeName(currentTheme->themeName);
    }

    emit saveAllColors();
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
            image.scaledToWidth(ui->themeIcon->width(), Qt::SmoothTransformation);
//            image.scaledToHeight(ui->themeIcon->height(), Qt::SmoothTransformation);
            ui->themeIcon->setPixmap(QPixmap::fromImage(image));
        }
    }
}

