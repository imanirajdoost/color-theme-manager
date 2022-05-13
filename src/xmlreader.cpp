#include "xmlreader.h"

XMLReader::XMLReader()
{

}

void XMLReader::writeTheme(Theme* theme)
{
    QFile xmlFile(theme->themePath);
    if (!xmlFile.open(QFile::ReadWrite | QFile::Text ))
    {
        qDebug() << "Already opened or there is another issue";
        xmlFile.close();
    }
    QTextStream xmlContent(&xmlFile);

    QDomDocument document;

    //make the root element
    QDomElement root = document.createElement("colors");
    //add it to document
    document.appendChild(root);

    QList<ColorPair*>* cols = theme->getColorPair();
    for(int j = 0; j < cols->count(); j++)
    {
        QDomElement color = document.createElement("color");
        color.setAttribute("id",cols->at(j)->getId());
        color.setAttribute("source",ColorPair::toRGBA(cols->at(j)->getColorSource()));
        color.setAttribute("target",ColorPair::toRGBA(cols->at(j)->getColorTarget()));
        root.appendChild(color);
    }
    xmlContent << document.toString();
}

void XMLReader::writeThemes(QList<Theme*>* themes)
{
    for(int i = 0; i < themes->count(); i++)
    {
        QFile xmlFile(themes->at(i)->themePath);
        if (!xmlFile.open(QFile::ReadWrite | QFile::Text ))
        {
            qDebug() << "Already opened or there is another issue";
            xmlFile.close();
        }
        QTextStream xmlContent(&xmlFile);

        QDomDocument document;

        //make the root element
        QDomElement root = document.createElement("colors");
        //add it to document
        document.appendChild(root);

        QList<ColorPair*>* cols = themes->at(i)->getColorPair();
        for(int j = 0; j < cols->count(); j++)
        {
            QDomElement color = document.createElement("color");
            color.setAttribute("id",cols->at(j)->getId());
            color.setAttribute("source",ColorPair::toRGBA(cols->at(j)->getColorSource()));
            color.setAttribute("target",ColorPair::toRGBA(cols->at(j)->getColorTarget()));
            root.appendChild(color);
        }
            xmlContent << document.toString();
    }
}


void XMLReader::write(SystemStat* stat)
{
    QFile xmlFile("xmlSettings.xml");
    if (!xmlFile.open(QFile::ReadWrite | QFile::Text ))
    {
        qDebug() << "Already opened or there is another issue";
        xmlFile.close();
    }
    QTextStream xmlContent(&xmlFile);

    QDomDocument document;

    //make the root element
    QDomElement root = document.createElement("ColorManagerSettings");
    //add it to document
    document.appendChild(root);

    for(int i = 0; i < stat->linksToFiles->count(); i++)
    {
        QDomElement link = document.createElement("theme");
        link.setAttribute("path",stat->linksToFiles->at(i));
        link.setAttribute("icon",stat->linksToIcons->at(i));
        root.appendChild(link);
    }

    xmlContent << document.toString();
}

SystemStat* XMLReader::getLastStat()
{
    SystemStat* stat = new SystemStat();
    QDomDocument myXML;
        QFile xmlFile("xmlSettings.xml");
        if (!xmlFile.open(QIODevice::ReadOnly ))
        {
            // Error while loading file
        }
        myXML.setContent(&xmlFile);
        xmlFile.close();

        QDomElement root = myXML.documentElement();
        QDomElement node = root.firstChild().toElement();

        QString datas = "";

        while(node.isNull() == false)
        {
            qDebug() << node.tagName();
            if(node.tagName() == "theme"){
                while(!node.isNull()){
                    QString path = node.attribute("path","");
                    QString icon = node.attribute("icon","");
                    stat->linksToFiles->push_back(path);
                    stat->linksToIcons->push_back(icon);
                    node = node.nextSibling().toElement();
                }
            }
            node = node.nextSibling().toElement();
        }
        return stat;
}

void XMLReader::update(Theme* _theme, QString content, bool shouldReplace)
{
    QDomDocument xmlBOM;
    xmlBOM.setContent(content);

    QDomElement root=xmlBOM.documentElement();

    // Get root names and attributes
    QString Type=root.tagName();

    // Get the first child of the root (Markup color is expected)
    QDomElement Component=root.firstChild().toElement();

    // Loop while there is a child
    while(!Component.isNull())
    {
        // Check if the child tag name is color
        if (Component.tagName()=="color")
        {
            // Read and display the component ID
            QString id = Component.attribute("id","No ID");

            // Read and display the component ID
            QString source =Component.attribute("source","No Source");

            // Read and display the component ID
            QString target =Component.attribute("target","No Target");


            // Display component data
            std::cout << "id: " << id.toStdString().c_str() << std::endl;
            std::cout << "   source  = " << source.toStdString().c_str() << std::endl;
            std::cout << "   target  = " << target.toStdString().c_str() << std::endl;
            std::cout << std::endl;

            //ColorPair cp(id,source,target);
            m_set.emplace(id,source,target);

            //            std::cout << "KIR: " + ColorPair::toRGBA(ColorPair::fromRGBA(source)).toStdString() << std::endl;
            ColorPair* pair = new ColorPair(id,ColorPair::fromRGBA(source),ColorPair::fromRGBA(target));
            if(!shouldReplace)
                _theme->addColorPair(pair);
            else {
                bool hasFound = false;
                QList<ColorPair*>* cols = _theme->getColorPair();
                // check if a color with this id exists:
                for(int i =0; i < cols->count(); i++) {
                    if(QString::compare(cols->at(i)->getId(), id) == 0) {
                        cols->at(i)->setColorSource(ColorPair::fromRGBA(source));
                        cols->at(i)->setColorTarget(ColorPair::fromRGBA(target));
                        hasFound = true;
                        break;;
                    }
                }
                if(!hasFound)
                    _theme->addColorPair(pair);
            }
        }
        // Next component
        Component = Component.nextSibling().toElement();
    }//End of while loop
}

void XMLReader::read(QFile& file, QList<Theme*>* listOfThemes)
{
    //Read the xml file
    QDomDocument xmlBOM;
    // Load xml file as raw data
    //QFile f("billofmaterial.xml");
    if (!file.open(QIODevice::ReadOnly ))
    {
        // Error while loading file
        std::cout << "Error while loading file" << std::endl;
        return;
    }
    QFileInfo fileInfo(file.fileName());
    QString fName = fileInfo.fileName();
    // Set data into the QDomDocument before processing
    xmlBOM.setContent(&file);
    file.close();

    Theme* theme = new Theme();
    theme->setName(fName);
    theme->themePath = file.fileName();
    std::cout << "Theme Path: " + theme->themePath.toStdString() << std::endl;

    // Extract the root markup
    QDomElement root=xmlBOM.documentElement();

    // Get root names and attributes
    QString Type=root.tagName();

    // Get the first child of the root (Markup color is expected)
    QDomElement Component=root.firstChild().toElement();

    // Loop while there is a child
    while(!Component.isNull())
    {
        // Check if the child tag name is color
        if (Component.tagName()=="color")
        {
            // Read and display the component ID
            QString id = Component.attribute("id","No ID");

            // Read and display the component ID
            QString source =Component.attribute("source","No Source");

            // Read and display the component ID
            QString target =Component.attribute("target","No Target");


            // Display component data
            std::cout << "id: " << id.toStdString().c_str() << std::endl;
            std::cout << "   source  = " << source.toStdString().c_str() << std::endl;
            std::cout << "   target  = " << target.toStdString().c_str() << std::endl;
            std::cout << std::endl;

            //ColorPair cp(id,source,target);
            m_set.emplace(id,source,target);

            //            std::cout << "KIR: " + ColorPair::toRGBA(ColorPair::fromRGBA(source)).toStdString() << std::endl;
            ColorPair* pair = new ColorPair(id,ColorPair::fromRGBA(source),ColorPair::fromRGBA(target));
            theme->addColorPair(pair);
        }
        // Next component
        Component = Component.nextSibling().toElement();
    }//End of while loop
    listOfThemes->push_back(theme);
}//End of method
