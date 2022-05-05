#include "xmlreader.h"

XMLReader::XMLReader()
{

}

void XMLReader::read(QFile& file, QList<Theme>& listOfThemes)
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

    Theme theme;
    theme.setName(fName);

    // Extract the root markup
    QDomElement root=xmlBOM.documentElement();

    // Get root names and attributes
    QString Type=root.tagName();
//    QString Board=root.attribute("BOARD","No name");
//    int Year=root.attribute("YEAR","1900").toInt();

    // Display root data
    std::cout << "Type  = " << Type.toStdString().c_str() << std::endl;
//    std::cout << "Board = " << Board.toStdString().c_str() << std::endl;
//    std::cout << "Year  = " << Year << std::endl;
    std::cout << std::endl;

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
            // Get the first child of the component
//            QDomElement Child=Component.firstChild().toElement();

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

            ColorPair pair(id,source,target);
            theme.addColorPair(pair);
        }
        // Next component
        Component = Component.nextSibling().toElement();
    }//End of while loop
    listOfThemes.push_back(theme);
}//End of method
