#include <headers/aradstyleformgenerator.h>
#include <headers/aradstylejsonparser.h>

#include <QMap>
#include <QVector>
#include <QStringList>
#include <limits>

Arad::GeneratingForm::AradStyleFormGenerator::AradStyleFormGenerator(QString const& filePath, QWidget* parent)
    : Arad::GeneratingForm::FormGenerator(filePath, parent)
{
    this->_widget->setWindowTitle("Generated Form");
}

void Arad::GeneratingForm::AradStyleFormGenerator::setupForm()
{
    QString filePath = Arad::GeneratingForm::FormGenerator::getFilePath();
    this->_jsonParser = new Arad::Parser::AradStyleJsonParser(filePath);
    QStringList validKeys = this->_jsonParser->getValidKeys();

    QVector<QMap<QString, QString>> tempVector = this->_jsonParser->parseJson();

    this->_vBoxLayout = new QVBoxLayout(this->_widget);

    this->_splitterLine = new QFrame(this->_widget);
    this->_splitterLine->setFrameShape(QFrame::HLine);
    this->_splitterLine->setFrameShadow(QFrame::Sunken);

    QString defaultValue;
    for (auto const& innerMap : tempVector)
    {
        this->_hBoxLayout = new QHBoxLayout;

        for (auto const& validKey : validKeys)
        {
            if (validKey == "name")
            {
                this->_label = new QLabel(this->_widget);
                this->_label->setText(innerMap[validKey].toLower() + " :");
                this->_labelContainer.push_back(this->_label);

                this->_hBoxLayout->addWidget(this->_label);
            }
            else if (validKey == "value")
            {
                defaultValue = innerMap[validKey].toLower();
            }
            else if (validKey == "type")
            {
                if (innerMap[validKey].toLower() == "string")
                {
                    this->_lineEdit = new QLineEdit(this->_widget);
                    this->_lineEdit->setPlaceholderText(defaultValue);
                    this->_lineEditContainer.push_back(this->_lineEdit);

                    if (innerMap["readonly"].toLower() == "true")
                        this->_lineEdit->setEnabled(false);

                    this->_hBoxLayout->addWidget(this->_lineEdit);
                }
                else if (innerMap[validKey].toLower() == "string list")
                {
                    ;
                }
                else if (innerMap[validKey].toLower() == "bool")
                {
                    ;
                }
                else if (innerMap[validKey].toLower() == "file")
                {
                    ;
                }
                else if (innerMap[validKey].toLower() == "number_i" or innerMap[validKey].toLower() == "number_ui")
                {
                    if (!defaultValue.isEmpty())
                    {
                        if (!this->validInteger(defaultValue))
                            throw std::invalid_argument("you should enter only number for default value (because the type is number)");
                    }
                    else
                        defaultValue = "0";

                    this->_regularSpinBox = new QSpinBox(this->_widget);

                    if (innerMap["readonly"].toLower() == "true")
                        this->_regularSpinBox->setEnabled(false);

                    int32_t maxInt = static_cast<int>(std::numeric_limits<int>::max());
                    int32_t minInt = static_cast<int>(std::numeric_limits<int>::min());
                    int32_t value = defaultValue.toInt();
                    if (innerMap[validKey].toLower() == "number_i")
                    {
                        this->_regularSpinBox->setRange(minInt, maxInt);
                    }
                    else if (innerMap[validKey].toLower() == "number_ui")
                    {
                        if (value < 0)
                            throw std::invalid_argument("you should enter only non-negative numbers because the type is number_ui");

                        this->_regularSpinBox->setRange(0, maxInt);
                    }

                    this->_regularSpinBox->setValue(value);
                    this->_regularSpinBoxContainer.push_back(this->_regularSpinBox);

                    this->_hBoxLayout->addWidget(this->_regularSpinBox);
                }
                else if (innerMap[validKey].toLower() == "number_f" or innerMap[validKey].toLower() == "number_uf")
                {
                    if (!defaultValue.isEmpty())
                    {
                        if (!this->validDouble(defaultValue))
                            throw std::invalid_argument("you should enter only number for default value (because the type is number)");
                    }
                    else
                        defaultValue = "0.0";

                    this->_doubleSpinBox = new QDoubleSpinBox(this->_widget);

                    if (innerMap["readonly"] == "true")
                        this->_doubleSpinBox->setEnabled(false);

                    double minDouble = static_cast<float>(std::numeric_limits<int>::min());
                    double maxDouble = std::numeric_limits<float>::max();
                    double value = defaultValue.toDouble();
                    if (innerMap[validKey].toLower() == "number_f")
                    {
                        this->_doubleSpinBox->setRange(minDouble, maxDouble);
                    }
                    else if (innerMap[validKey].toLower() == "number_uf")
                    {
                        if (value < 0)
                            throw std::invalid_argument("you should enter only non-negative numbers because the type is number_uf");

                        this->_doubleSpinBox->setRange(0.0f, maxDouble);
                    }

                    this->_doubleSpinBox->setValue(value);
                    this->_doubleSpinBoxContainer.push_back(this->_doubleSpinBox);

                    this->_hBoxLayout->addWidget(this->_doubleSpinBox);
                }
            }
        }

        this->_hBoxLayoutContainer.push_back(this->_hBoxLayout);
        this->_vBoxLayout->addWidget(this->_splitterLine);
        this->_vBoxLayout->addLayout(this->_hBoxLayout);

        defaultValue = "";
    }

    this->_widget->show();
}


Arad::GeneratingForm::AradStyleFormGenerator::~AradStyleFormGenerator()
{
    delete this->_label;
    delete this->_lineEdit;
    delete this->_splitterLine;
    delete this->_hBoxLayout;
    delete this->_vBoxLayout;
    delete this->_regularSpinBox;
    delete this->_doubleSpinBox;

    for (auto &label : this->_labelContainer)
        delete label;

    for (auto &lineEdit : this->_lineEditContainer)
        delete lineEdit;

    for(auto &hBoxLayout : this->_hBoxLayoutContainer)
        delete hBoxLayout;

    for (auto &regularSpinBox : this->_regularSpinBoxContainer)
        delete regularSpinBox;

    for (auto &doubleSpinBox : this->_doubleSpinBoxContainer)
        delete doubleSpinBox;
}
