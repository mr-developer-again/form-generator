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
    delete this->_browsingInFileSystem;

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
//    else if (this->_browsingInFileSystem->fileSystemModel->fileInfo(index).isDir())
//    {
//        QTextStream out(stdout);
//        out << "directory double clicked\n";
//    }
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
