#include <headers/aradstyleformgenerator.h>
#include <headers/aradstylejsonparser.h>

#include <QMap>
#include <QVector>

Arad::GeneratingForm::AradStyleFormGenerator::AradStyleFormGenerator(QString const& filePath, QWidget* parent)
    : Arad::GeneratingForm::FormGenerator(filePath, parent)
{
    this->setWindowTitle("Generated Form");
}

void Arad::GeneratingForm::AradStyleFormGenerator::setupForm()
{
    QString filePath = Arad::GeneratingForm::FormGenerator::getFilePath();
    this->_jsonParser = new Arad::Parser::AradStyleJsonParser(filePath);

    QVector<QMap<QString, QString>> tempVector = this->_jsonParser->parseJson();

    this->_vBoxLayout = new QVBoxLayout(this);

    this->_splitterLine = new QFrame(this);
    this->_splitterLine->setFrameShape(QFrame::HLine);
    this->_splitterLine->setFrameShadow(QFrame::Sunken);

    QString defaultValue;
    for (auto const& innerMap : tempVector)
    {
        this->_hBoxLayout = new QHBoxLayout;
        QMapIterator<QString, QString> it(innerMap);

        while (it.hasNext())
        {
            it.next();

            if (it.key().toLower() == "name")
            {
                this->_label = new QLabel(this);
                this->_label->setText(it.value().toLower() + ":");
                this->_labelContainer.push_back(this->_label);

                this->_hBoxLayout->addWidget(this->_label);
            }
            else if (it.key().toLower() == "value")
            {
                defaultValue = it.value().toLower();
            }
            else if (it.key().toLower() == "type")
            {
                if (it.value().toLower() == "string")
                {
                    this->_lineEdit = new QLineEdit(this);
                    this->_lineEdit->setPlaceholderText(defaultValue);
                    this->_lineEditContainer.push_back(this->_lineEdit);

                    this->_hBoxLayout->addWidget(this->_lineEdit);
                }
            }
            else if (it.key().toLower() == "readonly")
            {
                ;
            }
        }

        this->_hBoxLayoutContainer.push_back(this->_hBoxLayout);
        this->_vBoxLayout->addLayout(this->_hBoxLayout);
        this->_vBoxLayout->addWidget(this->_splitterLine);
    }
}


Arad::GeneratingForm::AradStyleFormGenerator::~AradStyleFormGenerator()
{
    delete this->_label;
    delete this->_lineEdit;

    for (auto &label : this->_labelContainer)
        delete label;

    for (auto &lineEdit : this->_lineEditContainer)
        delete lineEdit;

    for(auto &hBoxLayout : this->_hBoxLayoutContainer)
        delete hBoxLayout;
}
