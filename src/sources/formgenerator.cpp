#include <headers/formgenerator.h>

Arad::GeneratingForm::FormGenerator::FormGenerator(QString const& filePath, QWidget *parent)
{
    this->_widget = new QWidget;
    Arad::GeneratingForm::FormGenerator::setFilePath(filePath);
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

Arad::GeneratingForm::FormGenerator::~FormGenerator()
{
    delete this->_jsonParser;
    delete this->_widget;
}

