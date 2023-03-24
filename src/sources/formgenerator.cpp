#include <headers/formgenerator.h>

#include <QWidget>
#include <QString>
#include <QFile>
#include <QTextStream>

#include <stdexcept>

Arad::GeneratingForm::FormGenerator::FormGenerator(QString const& filePath)
{
    this->_widget.reset(new QWidget);
    this->_widget->setAttribute(Qt::WA_DeleteOnClose);
    Arad::GeneratingForm::FormGenerator::setFilePath(filePath);
}

void Arad::GeneratingForm::FormGenerator::darkTheme()
{
    // QFile file(":/themes/qdarkstyle/theme/darkstyle.qss");

    // if (!file.exists())
    //     throw std::runtime_error("file not found\n"
    //                              "-> error in finding theme file\n"
    //                              "   (fix theme file's path in \"src/fromgenerator.cpp\" file or\n"
    //                              "   comment \"formGenerator->darkTheme();\" in \"src/sample.cpp\" file)\n"
    //                              );

    // if (!file.open(QFile::ReadOnly | QFile::Text))
    //     throw std::runtime_error("couldn't open theme file");

    // QTextStream textStream(&file);
    // this->_widget->setStyleSheet(textStream.readAll());
}

void Arad::GeneratingForm::FormGenerator::toggleTheme(QString const& themeName)
{
    if ("default" == themeName)
        this->_widget->setStyleSheet("");
    else if (themeName.trimmed().toLower() == "dark")
        this->darkTheme();
}

void Arad::GeneratingForm::FormGenerator::setFilePath(QString const& filePath)
{
    if (filePath.size() == 0)
        throw std::invalid_argument("the input file path is empty");
    else if (!Arad::Parser::JsonParser::fileCanBeOpened(filePath))
        throw std::runtime_error("couldn't open the file");

    this->_filePath = filePath;
}

QString Arad::GeneratingForm::FormGenerator::getFilePath() const noexcept
{   return this->_filePath; }

bool Arad::GeneratingForm::FormGenerator::validInteger(QString const& inputString) const noexcept
{
    for (uint32_t i = 0; i < inputString.size(); ++i)
    {
        if (!(inputString[i].isDigit() or (inputString[i] == '-'))) /// returns false if the QChar is not digit or negative-sign (-)
            return false;

        if ((inputString[i] == '-') and (i != 0)) /// returns false if negative-sign has been located
                                                  /// in an invalid position (every index except the first one)
            return false;
    }

    return true;
}

bool Arad::GeneratingForm::FormGenerator::validDouble(QString const& inputString) const noexcept
{
    for (uint32_t i = 0; i < static_cast<uint32_t>(inputString.size()); ++i)
    {
        if (!(inputString[i].isDigit() or inputString[i] == '.' or inputString[i] == '-')) /// returns false if the QChar is not digit or dot (.)
                                                                                           /// or negative-sign (-)
            return false;

        if ((inputString[i] == '.') and (i == 0 or i == (inputString.size() - 1))) /// returns false if position of dot (.) is invalid
            return false;

        if ((inputString[i] == '-') and (i != 0)) /// returns false if position of negative-sign (-) is invalid
            return false;
    }

    return true;
}

Arad::GeneratingForm::FormGenerator::~FormGenerator()
{
    //////////////// TESTING ////////////////////
    QTextStream out(stdout);
    out << "FORM GENERATOR DESCTURCTOR" << Qt::endl;
    //////////////// TESTING ////////////////////
}
