#include <headers/aradstyleformgenerator.h>
#include <headers/aradstylejsonparser.h>
#include <headers/switchbutton.h>
#include <headers/aradstylejsongenerator.h>

#include <stdexcept>

Arad::GeneratingForm::AradStyleFormGenerator::AradStyleFormGenerator(QString const& filePath, QWidget* parent)
    : Arad::GeneratingForm::FormGenerator(filePath)
{
    this->_widget->setWindowTitle("Generated Form");
}

void Arad::GeneratingForm::AradStyleFormGenerator::setupForm()
{
    QString filePath = Arad::GeneratingForm::FormGenerator::getFilePath();
    this->_jsonParser = std::make_unique<Arad::Parser::AradStyleJsonParser>(filePath);
    QVector<QString> validKeys = this->_jsonParser->getValidKeys();

    QVector<QHash<QString, QString>> tempVector = this->_jsonParser->parseJson();

    this->_vBoxLayout.reset(new QVBoxLayout(this->_widget.get()));

    QString checkBoxLabel;
    int32_t counter = 0;
    for (auto& innerMap : tempVector)
    {
        this->_hBoxLayout.reset(new QHBoxLayout);

        for (auto const& validKey : validKeys)
        {
            if ("name" == validKey)
            {
                if (innerMap["type"] != "bool")
                {
                    this->_label.reset(new QLabel (this->_widget.get()));
                    this->_label->setText(innerMap[validKey].trimmed() + " :");
                    this->_hBoxLayout->addWidget(this->_label.get());
                    this->_labelContainer.push_back(std::move(this->_label));
                }
                else
                    checkBoxLabel = innerMap[validKey];
            }
            else if ("type" == validKey)
            {
                if (innerMap[validKey].trimmed() == "string")
                {
                    this->_lineEdit.reset(new QLineEdit(this->_widget.get()));
                    this->_lineEdit->setPlaceholderText(innerMap["description"].trimmed());

                    if (innerMap["default value"] != "")
                        this->_lineEdit->setText(innerMap["default value"]);

                    if (innerMap["readonly"].trimmed() == "true")
                        this->_lineEdit->setEnabled(false);

                    this->_hBoxLayout->addWidget(this->_lineEdit.get());

                    this->_lineEditContainer.push_back(std::move(this->_lineEdit));

                    this->addItemToHashTable(this->_itemPrecedence, "lineEdit", this->_lineEditIndex);
                    this->_lineEditIndex++;
                }
                else if (innerMap[validKey].trimmed() == "string list")
                {
                    this->_comboBox.reset(new QComboBox(this->_widget.get()));

                    QString comboBoxValues_str = innerMap["string list values"];
                    QStringList comboBoxValues_list = comboBoxValues_str.split(u' ', Qt::KeepEmptyParts);
                    this->_comboBox->addItems(comboBoxValues_list);
                    this->_comboBox->setCurrentText(innerMap["default value"].trimmed());

                    if (innerMap["readonly"].trimmed() == "false" or !innerMap.contains("readonly"))
                        this->_comboBox->setEnabled(true);
                    else if (innerMap["readonly"].trimmed() == "true")
                        this->_comboBox->setEnabled(false);

                    this->_hBoxLayout->addWidget(this->_comboBox.get());
                    this->_comboBoxContainer.push_back(std::move(this->_comboBox));

                    this->addItemToHashTable(this->_itemPrecedence, "comboBox", this->_comboBoxIndex);
                    this->_comboBoxIndex++;
                }
                else if (innerMap[validKey].trimmed() == "bool")
                {
                    this->_checkBox.reset(new QCheckBox(this->_widget.get()));

                    if (innerMap["default value"].trimmed() == "checked-true" or innerMap["default value"].trimmed().isEmpty())
                        this->_checkBox->setChecked(true);
                    else if (innerMap["default value"].trimmed() == "checked-false")
                        this->_checkBox->setChecked(false);

                    if (innerMap["readonly"] == "true")
                        this->_checkBox->setEnabled(false);
                    else if (innerMap["readonly"] == "false" or !innerMap.contains("readonly"))
                        this->_checkBox->setEnabled(true);

                    this->_checkBox->setText(checkBoxLabel);

                    this->_hBoxLayout->addWidget(this->_checkBox.get());

                    this->_checkBoxContainer.push_back(std::move(this->_checkBox));

                    this->addItemToHashTable(this->_itemPrecedence, "checkBox", this->_checkBoxIndex);
                    this->_checkBoxIndex++;
                }
                else if (innerMap[validKey].trimmed() == "file")
                {
                    this->_lineEdit.reset(new QLineEdit(this->_widget.get()));
                    this->_lineEdit->setPlaceholderText(innerMap["description"].trimmed());
                    this->_lineEdit->setText(innerMap["default value"].trimmed());

                    this->_pushButton.reset(new QPushButton(this->_widget.get()));
                    this->_pushButton->setText("Browse");

                    if (innerMap["readonly"] == "true")
                    {
                        this->_lineEdit->setEnabled(false);
                        this->_pushButton->setEnabled(false);
                    }
                    else if ("false" == innerMap["readonly"] or !innerMap.contains("readonly"))
                    {
                        this->_lineEdit->setEnabled(true);
                        this->_pushButton->setEnabled(true);
                    }

                    this->_hBoxLayout->addWidget(this->_lineEdit.get());
                    this->_hBoxLayout->addWidget(this->_pushButton.get());

                    this->_lineEditContainer.push_back(std::move(this->_lineEdit));
                    this->_pushButtonContainer.push_back(std::move(this->_pushButton));

                    QObject::connect(this->_pushButton.get(), SIGNAL(clicked()), \
                        this, SLOT(slot_browsePushButtonClicked()));

                    this->addItemToHashTable(this->_itemPrecedence, "file", this->_lineEditIndex);
                    this->_lineEditIndex++;
                }
                else if (innerMap[validKey].trimmed() == "number_i" or innerMap[validKey].trimmed() == "number_ui")
                {
                    if (!innerMap["default value"].trimmed().isEmpty())
                    {
                        if (!this->validInteger(innerMap["default value"].trimmed()))
                            throw std::invalid_argument("you should enter only number for default value (because the type is number)");
                    }
                    else
                        innerMap["default value"] = "0";

                    this->_regularSpinBox.reset(new QSpinBox(this->_widget.get()));

                    if (innerMap["readonly"] == "true")
                        this->_regularSpinBox->setEnabled(false);

                    int32_t maxInt = static_cast<int32_t>(std::numeric_limits<int32_t>::max());
                    int32_t minInt = static_cast<int32_t>(std::numeric_limits<int32_t>::min());
                    int32_t value = innerMap["default value"].trimmed().toInt();
                    if (innerMap[validKey].trimmed() == "number_i")
                        this->_regularSpinBox->setRange(minInt, maxInt);
                    else if (innerMap[validKey].trimmed() == "number_ui")
                    {
                        if (value < 0)
                            throw std::invalid_argument("you should enter only non-negative numbers because the type is number_ui");

                        this->_regularSpinBox->setRange(0, maxInt);
                    }

                    this->_regularSpinBox->setValue(value);

                    this->_hBoxLayout->addWidget(this->_regularSpinBox.get());
                    this->_regularSpinBoxContainer.push_back(std::move(this->_regularSpinBox));

                    this->addItemToHashTable(this->_itemPrecedence, "integer spinBox", this->_regularSpinBoxIndex);
                    this->_regularSpinBoxIndex++;
                }
                else if ("number_f" == innerMap[validKey].trimmed() or "number_uf" == innerMap[validKey].trimmed())
                {
                    if (!innerMap["default value"].trimmed().isEmpty())
                    {
                        if (!this->validDouble(innerMap["default value"].trimmed()))
                            throw std::invalid_argument("you should enter only number for default value (because the type is number)");
                    }
                    else
                        innerMap["default value"].trimmed() = "0.0";

                    this->_doubleSpinBox.reset(new QDoubleSpinBox(this->_widget.get()));

                    if (innerMap["readonly"] == "true")
                        this->_doubleSpinBox->setEnabled(false);

                    double minDouble = static_cast<float>(std::numeric_limits<int>::min());
                    double maxDouble = static_cast<float>(std::numeric_limits<float>::max());
                    double value = innerMap["default value"].trimmed().toDouble();
                    if (innerMap[validKey].trimmed() == "number_f")
                        this->_doubleSpinBox->setRange(minDouble, maxDouble);
                    else if (innerMap[validKey].trimmed() == "number_uf")
                    {
                        if (value < 0)
                            throw std::invalid_argument("you should enter only non-negative numbers because the type is number_uf");

                        this->_doubleSpinBox->setRange(0.0f, maxDouble);
                    }

                    this->_doubleSpinBox->setValue(value);

                    this->_hBoxLayout->addWidget(this->_doubleSpinBox.get());
                    this->_doubleSpinBoxContainer.push_back(std::move(this->_doubleSpinBox));

                    this->addItemToHashTable(this->_itemPrecedence, "float spinBox", this->_doubleSpinIndex);
                    this->_doubleSpinIndex++;
                }
            }
        }


        this->_vBoxLayout->addLayout(this->_hBoxLayout.get());
        this->_hBoxLayoutContainer.push_back(std::move(this->_hBoxLayout));
        if (counter != (tempVector.size() - 1))
        {
            this->_splitterLine.reset(new QFrame(this->_widget.get()));
            this->_splitterLine->setFrameShape(QFrame::HLine);
            this->_splitterLine->setFrameShadow(QFrame::Sunken);
            this->_vBoxLayout->addWidget(this->_splitterLine.get());
            this->_splitterLineContainer.push_back(std::move(this->_splitterLine));
        }

        checkBoxLabel = "";

        ++counter;
        this->_itemPrecedence++;
    }

    this->_hBoxLayout.reset(new QHBoxLayout);

    this->_spacerItem = std::make_unique<QSpacerItem>(50, 100);
    this->_hBoxLayout->addItem(this->_spacerItem.get());

    this->_pushButton.reset(new QPushButton("Cancel", this->_widget.get()));
    this->_hBoxLayout->addWidget(this->_pushButton.get());
    
    /// @brief cancel button pushed
    QObject::connect(this->_pushButton.get(), &QPushButton::clicked, [this]{
        this->_widget->close();
    });
    this->_pushButtonContainer.push_back(std::move(this->_pushButton));

    this->_pushButton.reset(new QPushButton("Save", this->_widget.get()));
    this->_hBoxLayout->addWidget(this->_pushButton.get());

    /// @brief save button pushed
    QObject::connect(this->_pushButton.get(), &QPushButton::clicked, [this]{
        this->slot_saveButtonPressed();
    });
    this->_pushButtonContainer.push_back(std::move(this->_pushButton));

    this->_hBoxLayout->addItem(this->_spacerItem.get());

    this->_spacerItemContainer.push_back(std::move(this->_spacerItem));

    this->_vBoxLayout->addLayout(this->_hBoxLayout.get());

    this->_hBoxLayoutContainer.push_back(std::move(this->_hBoxLayout));


    this->_widget->show();
    Arad::GeneratingForm::AradStyleFormGenerator::setFixedWidgetSize();
}

void Arad::GeneratingForm::AradStyleFormGenerator::setFixedWidgetSize() noexcept
{
    this->_widget->setFixedSize(this->_widget->geometry().width(), this->_widget->geometry().height());
}

void Arad::GeneratingForm::AradStyleFormGenerator::addItemToHashTable(int32_t itemPrecednece, QString const& itemType, int32_t indexOfItem)
{
    this->_hashTable.push_back(QVector<QString>({QString::number(itemPrecednece), itemType, QString::number(indexOfItem)}));
}

Arad::GeneratingForm::AradStyleFormGenerator::~AradStyleFormGenerator()
{
    /////////////// TESTING ///////////////
    QTextStream out(stdout);
    out << "inside aradStyleFormGenerator destructor\n";
    ////////////// END OF TESTING /////////////////
}

void Arad::GeneratingForm::AradStyleFormGenerator::slot_browsePushButtonClicked()
{
    std::unique_ptr<QFileDialog> fileDialog = std::make_unique<QFileDialog>();

    QString filePath;
    if ((filePath = fileDialog->getOpenFileName(nullptr, QString(), QDir::homePath())) != "")
        this->_lineEdit->setText(filePath);
}

void Arad::GeneratingForm::AradStyleFormGenerator::slot_saveButtonPressed()
{
    std::unique_ptr<QFileDialog> fileDialog = std::make_unique<QFileDialog>();
    QString filePath;
    if ((filePath = fileDialog->getSaveFileName(nullptr, QString(), QDir::homePath())) != "")
    {
        QVector<QHash<QString, QString>> tempContainer = this->_jsonParser->parseJson();
        QVector<QString> validKeys = this->_jsonParser->getValidKeys();
        for (int precedence = 0; precedence < tempContainer.size(); precedence++)
        {
            QHash<QString, QString> &innerMap = tempContainer[precedence];

            if ("lineEdit" == this->_hashTable[precedence][1])
            {
                if ("" != this->_lineEditContainer[this->_hashTable[precedence][2].toUInt()]->text())
                    innerMap["default value"] = \
                            this->_lineEditContainer[this->_hashTable[precedence][2].toUInt()]->text();
            }
            else if ("checkBox" == this->_hashTable[precedence][1])
            {
                innerMap["default value"] = \
                        this->_checkBoxContainer[this->_hashTable[precedence][2].toUInt()]->isChecked() ? "true" : "false";
            }
            else if ("comboBox" == this->_hashTable[precedence][1])
            {
                innerMap["default value"] = \
                        this->_comboBoxContainer[this->_hashTable[precedence][2].toUInt()]->currentText();
            }
            else if ("integer spinBox" == this->_hashTable[precedence][1] or "unsigned int spinBox" == this->_hashTable[precedence][1])
            {
                innerMap["default value"] = \
                        this->_regularSpinBoxContainer[this->_hashTable[precedence][2].toUInt()]->text();
            }
            else if ("float spinBox" == this->_hashTable[precedence][1] or "unsigned float spinBox" == this->_hashTable[precedence][1])
            {
                innerMap["default value"] = \
                        this->_doubleSpinBoxContainer[this->_hashTable[precedence][2].toUInt()]->text();
            }
            else if ("file" == this->_hashTable[precedence][1])
            {
                if ("" != this->_lineEditContainer[this->_hashTable[precedence][2].toUInt()]->text())
                    innerMap["default value"] = \
                        this->_lineEditContainer[this->_hashTable[precedence][2].toUInt()]->text();
            }
        }

        std::unique_ptr<Arad::JsonGeneratorNP::JsonGenerator> jsonGenerator = \
                std::make_unique<Arad::JsonGeneratorNP::AradStyleJsonGenerator>();

        (*jsonGenerator)(tempContainer, filePath/*, orderedKeys*/);
    }
}