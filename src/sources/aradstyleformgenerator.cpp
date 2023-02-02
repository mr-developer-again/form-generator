#include <headers/aradstyleformgenerator.h>
#include <headers/aradstylejsonparser.h>

#include <QMap>
#include <QVector>
#include <QStringList>
#include <QMessageBox>
#include <QDialog>
#include <QSpacerItem>

#include <limits>
#include <stdexcept>

//////////// TESTING //////////////
#include <QTextStream>
/////////// END OF TESTING ////////////

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
    try
    {
        tempVector = this->_jsonParser->parseJson();
    }
    catch(std::runtime_error const& ex)
    {
        QMessageBox mesgBx;
        mesgBx.setText(ex.what());
        mesgBx.exec();

        return;
    }

    this->_vBoxLayout = new QVBoxLayout(this->_widget);

    this->_splitterLine = new QFrame(this->_widget);
    this->_splitterLine->setFrameShape(QFrame::HLine);
    this->_splitterLine->setFrameShadow(QFrame::Sunken);

    QString defaultValue;
    QString description;
    QString checkBoxLabel;
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
                    this->_label->setText(innerMap[validKey].toLower() + " :");
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
                defaultValue = innerMap[validKey].toLower();
            }
            else if (validKey == "description")
            {
                description = innerMap[validKey];
            }
            else if (validKey == "type")
            {
                if (innerMap[validKey].toLower() == "string")
                {
                    this->_lineEdit = new QLineEdit(this->_widget);
                    this->_lineEdit->setPlaceholderText(description);
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
                    this->_checkBox = new QCheckBox(this->_widget);

                    if (defaultValue == "checked-true" or defaultValue.isEmpty())
                        this->_checkBox->setChecked(true);
                    else if (defaultValue == "checked-false")
                        this->_checkBox->setChecked(false);

                    if (innerMap["readonly"] == "true")
                        this->_checkBox->setEnabled(false);
                    else if (innerMap["readonly"] == "false")
                        this->_checkBox->setEnabled(true);

                    this->_checkBox->setText(checkBoxLabel);
                    this->_checkBoxContainer.push_back(this->_checkBox);

                    this->_hBoxLayout->addWidget(this->_checkBox);
                }
                else if (innerMap[validKey].toLower() == "file")
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
                    else if (innerMap["readonly"] == "false")
                    {
                        this->_lineEdit->setEnabled(true);
                        this->_pushButton->setEnabled(true);
                    }

                    this->_lineEditContainer.push_back(this->_lineEdit);
                    this->_pushButtonContainer.push_back(this->_pushButton);

                    QObject::connect(this->_pushButton, SIGNAL(clicked()), this, SLOT(slot_browsePushButtonClicked()));

                    this->_hBoxLayout->addWidget(this->_lineEdit);
                    this->_hBoxLayout->addWidget(this->_pushButton);
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
        checkBoxLabel = "";
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
    delete this->_fileSystemModel;
    delete this->_treeView;

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


void Arad::GeneratingForm::AradStyleFormGenerator::slot_browsePushButtonClicked()
{
    QDialog *dialog = new QDialog(this);
    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    QVBoxLayout *vBoxLayout = new QVBoxLayout(dialog);
    QPushButton *selectPushButton = new QPushButton(dialog);
    QSpacerItem *spacerItem = new QSpacerItem(400, 20);

    dialog->setFixedSize(600, 400);

    selectPushButton->setText("select");

    this->_fileSystemModel = new QFileSystemModel(dialog);
    this->_fileSystemModel->setReadOnly(false);
    this->_fileSystemModel->setRootPath(QDir::homePath());
    QModelIndex rootPathIndex = this->_fileSystemModel->index(QDir::homePath());

    this->_treeView = new QTreeView(dialog);
    this->_treeView->setModel(this->_fileSystemModel);
    this->_treeView->expand(rootPathIndex);
    this->_treeView->scrollTo(rootPathIndex);
    this->_treeView->setCurrentIndex(rootPathIndex);
    this->_treeView->resizeColumnToContents(0);

    hBoxLayout->addWidget(selectPushButton);
    hBoxLayout->addItem(spacerItem);
    vBoxLayout->addWidget(this->_treeView);
    vBoxLayout->addLayout(hBoxLayout);

    dialog->show();
    dialog->exec();
}
