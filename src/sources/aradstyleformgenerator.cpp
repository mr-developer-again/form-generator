#include <headers/aradstyleformgenerator.h>
#include <headers/aradstylejsonparser.h>
#include <headers/switchbutton.h>
#include <headers/aradstylejsongenerator.h>

#include <QMap>
#include <QVector>
#include <QStringList>
#include <QDialog>
#include <QSpacerItem>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QAbstractButton>

#include <limits>
#include <stdexcept>

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

    QVector<QMap<QString, QString>> tempVector;
    tempVector = this->_jsonParser->parseJson();

    this->_vBoxLayout = new QVBoxLayout(this->_widget);

    this->_hBoxLayout = this->_themesSwitchButton(this->_widget, this);
    this->_hBoxLayoutContainer.push_back(this->_hBoxLayout);
    this->_vBoxLayout->addLayout(this->_hBoxLayout);

    QString defaultValue;
    QString description;
    QString checkBoxLabel;
    int32_t counter = 0;
    for (auto const& innerMap : tempVector)
    {
        this->_hBoxLayout = new QHBoxLayout;

        for (auto const& validKey : validKeys)
        {
            if (validKey == "name")
            {
                if (innerMap["type"].toLower() != "bool")
                {
                    this->_label = new QLabel(this->_widget);
                    this->_label->setText(innerMap[validKey].toLower().trimmed() + " :");
                    this->_labelContainer.push_back(this->_label);

                    this->_hBoxLayout->addWidget(this->_label);
                }
                else
                {
                    checkBoxLabel = innerMap[validKey];
                }
            }
            else if (validKey == "default value")
            {
                defaultValue = innerMap[validKey].toLower().trimmed();
            }
            else if (validKey == "description")
            {
                description = innerMap[validKey];
            }
            else if (validKey == "type")
            {
                if (innerMap[validKey].toLower().trimmed() == "string")
                {
                    this->_lineEdit = new QLineEdit(this->_widget);
                    this->_lineEdit->setPlaceholderText(description);
                    this->_lineEditContainer.push_back(this->_lineEdit);

                    if (innerMap["readonly"].toLower().trimmed() == "true")
                        this->_lineEdit->setEnabled(false);

                    this->_hBoxLayout->addWidget(this->_lineEdit);

                    this->addItemToHashTable(this->_itemPrecedence, "l", this->_lineEditIndex);
                    this->_lineEditIndex++;
                }
                else if (innerMap[validKey].toLower().trimmed() == "string list")
                {
                    this->_comboBox = new QComboBox(this->_widget);

                    QString comboBoxValues_str = innerMap["string list values"];
                    QStringList comboBoxValues_list = comboBoxValues_str.split(u' ', Qt::KeepEmptyParts);
                    this->_comboBox->addItems(comboBoxValues_list);
                    this->_comboBox->setCurrentText(defaultValue);

                    if (innerMap["readonly"].toLower().trimmed() == "false" or !innerMap.contains("readonly"))
                        this->_comboBox->setEnabled(true);
                    else if (innerMap["readonly"].toLower().trimmed() == "true")
                        this->_comboBox->setEnabled(false);

                    this->_comboBoxContainer.push_back(this->_comboBox);
                    this->_hBoxLayout->addWidget(this->_comboBox);

                    this->addItemToHashTable(this->_itemPrecedence, "C", this->_comboBoxIndex);
                    this->_comboBoxIndex++;
                }
                else if (innerMap[validKey].toLower().trimmed() == "bool")
                {
                    this->_checkBox = new QCheckBox(this->_widget);

                    if (defaultValue == "checked-true" or defaultValue.isEmpty())
                        this->_checkBox->setChecked(true);
                    else if (defaultValue == "checked-false")
                        this->_checkBox->setChecked(false);

                    if (innerMap["readonly"] == "true")
                        this->_checkBox->setEnabled(false);
                    else if (innerMap["readonly"] == "false" or !innerMap.contains("readonly"))
                        this->_checkBox->setEnabled(true);

                    this->_checkBox->setText(checkBoxLabel);
                    this->_checkBoxContainer.push_back(this->_checkBox);

                    this->_hBoxLayout->addWidget(this->_checkBox);

                    this->addItemToHashTable(this->_itemPrecedence, "c", this->_checkBoxIndex);
                    this->_checkBoxIndex++;
                }
                else if (innerMap[validKey].toLower().trimmed() == "file")
                {
                    this->_lineEdit = new QLineEdit(this->_widget);
                    this->_lineEdit->setPlaceholderText(description);

                    this->_pushButton = new QPushButton(this->_widget);
                    this->_pushButton->setText("Browse");

                    if (innerMap["readonly"] == "true")
                    {
                        this->_lineEdit->setEnabled(false);
                        this->_pushButton->setEnabled(false);
                    }
                    else if (innerMap["readonly"] == "false" or !innerMap.contains("readonly"))
                    {
                        this->_lineEdit->setEnabled(true);
                        this->_pushButton->setEnabled(true);
                    }

                    this->_lineEditContainer.push_back(this->_lineEdit);
                    this->_pushButtonContainer.push_back(this->_pushButton);

                    QObject::connect(this->_pushButton, SIGNAL(clicked()), this, SLOT(slot_browsePushButtonClicked()));

                    this->_hBoxLayout->addWidget(this->_lineEdit);
                    this->_hBoxLayout->addWidget(this->_pushButton);

                    this->addItemToHashTable(this->_itemPrecedence, "f", this->_fileIndex);
                    this->_fileIndex++;
                }
                else if (innerMap[validKey].toLower().trimmed() == "number_i" or innerMap[validKey].toLower().trimmed() == "number_ui")
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
                    if (innerMap[validKey].toLower().trimmed() == "number_i")
                    {
                        this->_regularSpinBox->setRange(minInt, maxInt);

                        this->addItemToHashTable(this->_itemPrecedence, "s", this->_signedRegularSpinBoxIndex);
                        this->_signedRegularSpinBoxIndex++;
                    }
                    else if (innerMap[validKey].toLower().trimmed() == "number_ui")
                    {
                        if (value < 0)
                            throw std::invalid_argument("you should enter only non-negative numbers because the type is number_ui");

                        this->_regularSpinBox->setRange(0, maxInt);

                        this->addItemToHashTable(this->_itemPrecedence, "us", this->_unsignedRegularSpinBoxIndex);
                        this->_unsignedRegularSpinBoxIndex++;
                    }

                    this->_regularSpinBox->setValue(value);
                    this->_regularSpinBoxContainer.push_back(this->_regularSpinBox);

                    this->_hBoxLayout->addWidget(this->_regularSpinBox);
                }
                else if (innerMap[validKey].toLower().trimmed() == "number_f" or innerMap[validKey].toLower().trimmed() == "number_uf")
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
                    if (innerMap[validKey].toLower().trimmed() == "number_f")
                    {
                        this->_doubleSpinBox->setRange(minDouble, maxDouble);

                        this->addItemToHashTable(this->_itemPrecedence, "S", this->_signedDoubleSpinIndex);
                        this->_signedDoubleSpinIndex++;
                    }
                    else if (innerMap[validKey].toLower().trimmed() == "number_uf")
                    {
                        if (value < 0)
                            throw std::invalid_argument("you should enter only non-negative numbers because the type is number_uf");

                        this->_doubleSpinBox->setRange(0.0f, maxDouble);

                        this->addItemToHashTable(this->_itemPrecedence, "uS", this->_unsignedDoubleSpinIndex);
                        this->_unsignedDoubleSpinIndex++;
                    }

                    this->_doubleSpinBox->setValue(value);
                    this->_doubleSpinBoxContainer.push_back(this->_doubleSpinBox);

                    this->_hBoxLayout->addWidget(this->_doubleSpinBox);
                }
            }
        }


        this->_hBoxLayoutContainer.push_back(this->_hBoxLayout);
        this->_vBoxLayout->addLayout(this->_hBoxLayout);
        if (counter != (tempVector.size() - 1))
        {
            this->_splitterLine = new QFrame(this->_widget);
            this->_splitterLine->setFrameShape(QFrame::HLine);
            this->_splitterLine->setFrameShadow(QFrame::Sunken);
            this->_splitterLineContainer.push_back(this->_splitterLine);
            this->_vBoxLayout->addWidget(this->_splitterLine);
        }

        defaultValue = "";
        checkBoxLabel = "";

        ++counter;
        this->_itemPrecedence++;
    }

    this->_spacerItem = new QSpacerItem(50, 100);
    this->_spacerItemContainer.push_back(this->_spacerItem);

    this->_pushButton = new QPushButton("Cancel", this->_widget);
    this->_pushButtonContainer.push_back(this->_pushButton);

    this->_pushButton = new QPushButton("Save", this->_widget);
    this->_pushButtonContainer.push_back(this->_pushButton);

    this->_hBoxLayout = new QHBoxLayout;
    this->_hBoxLayout->addItem(this->_spacerItem);
    this->_hBoxLayout->addWidget(this->_pushButtonContainer[this->_pushButtonContainer.size() - 2]);
    this->_hBoxLayout->addWidget(this->_pushButtonContainer[this->_pushButtonContainer.size() - 1]);
    this->_hBoxLayout->addItem(this->_spacerItem);

    this->_vBoxLayout->addLayout(this->_hBoxLayout);

    /// @brief cancel button pushed
    QObject::connect(this->_pushButtonContainer[this->_pushButtonContainer.size() - 2], &QPushButton::clicked, this, [this]{
        this->_widget->close();
    });

    /// @brief save button pushed
    QObject::connect(this->_pushButtonContainer[this->_pushButtonContainer.size() - 1], &QPushButton::clicked, [this]{
        QFileDialog *fileDialog = new QFileDialog;
        QString filePath = fileDialog->getSaveFileName();
        QVector<QMap<QString, QString>> tempMap = this->_jsonParser->parseJson();
//        Arad::JsonGeneratorNP::JsonGenerator *jsonGenerator = new Arad::JsonGeneratorNP::AradStyleJsonGenerator(filePath);
    });

    this->_widget->show();
    this->generatedFormSizeFixer();
}

void Arad::GeneratingForm::AradStyleFormGenerator::generatedFormSizeFixer() noexcept
{
    this->_widget->setFixedSize(this->_widget->geometry().width(), this->_widget->geometry().height());
}

void Arad::GeneratingForm::AradStyleFormGenerator::addItemToHashTable(int32_t itemPrecednece, QString const& itemType, int32_t indexOfItem)
{
    this->_hashTable.push_back({QString::number(itemPrecednece), itemType, QString::number(indexOfItem)});
}

void Arad::GeneratingForm::AradStyleFormGenerator::JsonGenerator() const noexcept
{
    QVector<QMap<QString, QString>> tempVector;
    tempVector = this->_jsonParser->parseJson();

    int32_t counter = 0;
    for (auto innerMap : tempVector)
    {
        for (auto mapItem : innerMap)
        {
            ;
        }

        counter++;
    }
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
    delete this->_browsingInFileSystem;
    delete this->_spacerItem;

    for (auto label : this->_labelContainer)
        delete label;

    for (auto lineEdit : this->_lineEditContainer)
        delete lineEdit;

    for(auto hBoxLayout : this->_hBoxLayoutContainer)
        delete hBoxLayout;

    for (auto regularSpinBox : this->_regularSpinBoxContainer)
        delete regularSpinBox;

    for (auto doubleSpinBox : this->_doubleSpinBoxContainer)
        delete doubleSpinBox;

    for (auto spacerItem : this->_spacerItemContainer)
        delete spacerItem;
}

void Arad::GeneratingForm::AradStyleFormGenerator::slot_browsePushButtonClicked()
{
    this->_browsingInFileSystem = new Arad::GeneratingForm::AradStyleFormGenerator::BrowsingInFileSystem;

    this->_browsingInFileSystem->dialog->setFixedSize(600, 400);

    this->_browsingInFileSystem->selectPushButton->setText("select");
    this->_browsingInFileSystem->selectPushButton->setEnabled(true);

    this->_browsingInFileSystem->spacerItem = new QSpacerItem(400, 20);

    this->_browsingInFileSystem->fileSystemModel = new QFileSystemModel(this->_browsingInFileSystem->dialog);
    this->_browsingInFileSystem->fileSystemModel->setReadOnly(true);
    this->_browsingInFileSystem->fileSystemModel->setRootPath(QDir::homePath());
    QModelIndex rootPathIndex = this->_browsingInFileSystem->fileSystemModel->index(QDir::homePath());

    this->_browsingInFileSystem->treeView = new QTreeView(this->_browsingInFileSystem->dialog);
    this->_browsingInFileSystem->treeView->setModel(this->_browsingInFileSystem->fileSystemModel);
    this->_browsingInFileSystem->treeView->expand(rootPathIndex);
    this->_browsingInFileSystem->treeView->scrollTo(rootPathIndex);
    this->_browsingInFileSystem->treeView->setCurrentIndex(rootPathIndex);
    this->_browsingInFileSystem->treeView->resizeColumnToContents(0);

    QObject::connect(this->_browsingInFileSystem->treeView, SIGNAL(doubleClicked(QModelIndex)), this,
                     SLOT(slot_treeViewDoubleClicked(QModelIndex)));

    QObject::connect(this->_browsingInFileSystem->selectPushButton, SIGNAL(clicked()), this,
                     SLOT(slot_selectPushButtonClicked()));

    this->_browsingInFileSystem->hBoxLayout->addWidget(this->_browsingInFileSystem->selectPushButton);
    this->_browsingInFileSystem->hBoxLayout->addItem(this->_browsingInFileSystem->spacerItem);
    this->_browsingInFileSystem->vBoxLayout->addWidget(this->_browsingInFileSystem->treeView);
    this->_browsingInFileSystem->vBoxLayout->addLayout(this->_browsingInFileSystem->hBoxLayout);

    this->_browsingInFileSystem->dialog->show();
    this->_browsingInFileSystem->dialog->exec();
}

void Arad::GeneratingForm::AradStyleFormGenerator::slot_treeViewDoubleClicked(QModelIndex index)
{
    if (this->_browsingInFileSystem->fileSystemModel->fileInfo(index).isFile())
    {
        this->_browsingInFileSystem->pathOfSelectedFile = \
                this->_browsingInFileSystem->fileSystemModel->fileInfo(index).absoluteFilePath();

        this->_lineEdit->setText(this->_browsingInFileSystem->pathOfSelectedFile);

        this->_browsingInFileSystem->dialog->close();
    }
}

void Arad::GeneratingForm::AradStyleFormGenerator::slot_selectPushButtonClicked()
{
    QModelIndex currentIndex = this->_browsingInFileSystem->treeView->currentIndex();

    if (this->_browsingInFileSystem->fileSystemModel->fileInfo(currentIndex).isFile())
        emit this->_browsingInFileSystem->treeView->doubleClicked(currentIndex);
    else if (this->_browsingInFileSystem->fileSystemModel->fileInfo(currentIndex).isDir())
    {
        if (!this->_browsingInFileSystem->treeView->isExpanded(currentIndex))
        {
            this->_browsingInFileSystem->treeView->expand(currentIndex);
            this->_browsingInFileSystem->treeView->scrollTo(currentIndex);
        }
        else
        {
            this->_browsingInFileSystem->treeView->collapse(currentIndex);
        }
    }
}

QHBoxLayout* Arad::GeneratingForm::AradStyleFormGenerator::ThemesSwitchButton::operator()(QWidget* widget,
                                                                                          Arad::GeneratingForm::AradStyleFormGenerator *mainForm)
{
    if (this->counter == 0)
    {
        widget->setStyleSheet("");
        ++this->counter;
    }

    Arad::GeneratingForm::SwitchButton *switchButton = new struct Arad::GeneratingForm::SwitchButton::SwitchButton(widget);
    QObject::connect(switchButton, &QAbstractButton::pressed, [this, mainForm]()
    {
        this->_themeIndex++;

        if (this->_themes.size() == this->_themeIndex)
            this->_themeIndex = 0;

        mainForm->toggleTheme(this->_themes[this->_themeIndex]);
    });

    QLabel *nameLabel = new QLabel(widget);
    nameLabel->setText("Dark Mode on: ");

    QSpacerItem *spacerItem = new QSpacerItem(250, 50);
    this->_themeToggleHBoxLayout = new QHBoxLayout;

    this->_themeToggleHBoxLayout->addWidget(nameLabel);
    this->_themeToggleHBoxLayout->addWidget(switchButton);
    this->_themeToggleHBoxLayout->addItem(spacerItem);

    return this->_themeToggleHBoxLayout;
}

Arad::GeneratingForm::AradStyleFormGenerator::ThemesSwitchButton::~ThemesSwitchButton()
{
    delete this->_themeToggleHBoxLayout;

    //////////////// TESTING ////////////////////
    QTextStream out(stdout);
    out << "ARAD STYLE FORM GENERATOR DESCTURCTOR" << Qt::endl;
    //////////////// TESTING ////////////////////
}
