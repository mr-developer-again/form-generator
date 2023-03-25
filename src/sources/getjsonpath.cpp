#include "headers/getjsonpath.h"

#include <QFileDialog>
#include <QMessageBox>

///////////////////// testing ////////////////////
#include <QTextStream>
//////////////////// end of testing //////////////////

JsonFilePath::JsonFilePath(QDialog *parent)
    : _dialog(std::make_unique<QDialog>()),
      _label(std::make_unique<QLabel>("Json Path: ", this->_dialog.get())),
      _filePathLineEdit(std::make_unique<QLineEdit>(this->_dialog.get())),
      _browsePushButton(std::make_unique<QPushButton>("Browse", this->_dialog.get())),
      _browseHLayout(std::make_unique<QHBoxLayout>()),

      _generatePushButton(std::make_unique<QPushButton>("Generate Form", this->_dialog.get())),
      _cancelButton(std::make_unique<QPushButton>("Cancel", this->_dialog.get())),
      _closeGenerateHLayout(std::make_unique<QHBoxLayout>()),
      _centeralVLayout(std::make_unique<QVBoxLayout>(this->_dialog.get()))
{
    this->_dialog->setAttribute(Qt::WA_DeleteOnClose);
    this->_filePathLineEdit->setPlaceholderText("Please Enter Json File Path Here");
}

JsonFilePathStatus JsonFilePath::operator()(QString& filePath)
{
    JsonFilePathStatus status;

    this->_browseHLayout->addWidget(this->_label.get());
    this->_browseHLayout->addWidget(this->_filePathLineEdit.get());
    this->_browseHLayout->addWidget(this->_browsePushButton.get());

    this->_closeGenerateHLayout->addStretch();
    this->_closeGenerateHLayout->addWidget(this->_cancelButton.get());
    this->_closeGenerateHLayout->addWidget(this->_generatePushButton.get());
    this->_closeGenerateHLayout->addStretch();

    this->_centeralVLayout->addLayout(this->_browseHLayout.get());
    this->_centeralVLayout->addLayout(this->_closeGenerateHLayout.get());

    QObject::connect(this->_browsePushButton.get(), &QPushButton::clicked, [this]{
        QString filePath = QFileDialog::getOpenFileName();
        if (!filePath.isEmpty())
            this->_filePathLineEdit->setText(filePath);
    });

    QObject::connect(this->_generatePushButton.get(), &QPushButton::clicked, [this, &filePath, &status]() -> JsonFilePathStatus {
        QString selectedFilePath = this->_filePathLineEdit->text();

        if (selectedFilePath.isEmpty())
            QMessageBox::critical(nullptr, "Json File Path", "You Must Enter Json File Path");
        else
        {
            filePath = selectedFilePath;
            this->_dialog->close();
            status = JsonFilePathStatus::GENERATE_FORM;
        }
    });

    QObject::connect(this->_cancelButton.get(), &QPushButton::clicked, [this, &status]{
        this->_dialog->close();
        status = JsonFilePathStatus::CANCEL;
    });

    /////////////////////////// TESTING /////////////////////////////
    QObject::connect(this->_dialog.get(), &QDialog::destroyed, []{
        QTextStream out(stdout);
        out << "dialog destroyed\n";
    });

    QObject::connect(this->_label.get(), &QObject::destroyed, []{
        QTextStream out(stdout);
        out << "label destroyed\n";
    });

    QObject::connect(this->_filePathLineEdit.get(), &QObject::destroyed, []{
        QTextStream out(stdout);
        out << "lineEdit destroyed\n";
    });

    QObject::connect(this->_browsePushButton.get(), &QObject::destroyed, []{
        QTextStream out(stdout);
        out << "browsePushButton destroyed\n";
    });

    QObject::connect(this->_browseHLayout.get(), &QObject::destroyed, []{
        QTextStream out(stdout);
        out << "browseHLayout destroyed\n";
    });

    QObject::connect(this->_generatePushButton.get(), &QObject::destroyed, []{
        QTextStream out(stdout);
        out << "generatePushButton destroyed\n";
    });

    QObject::connect(this->_cancelButton.get(), &QObject::destroyed, []{
        QTextStream out(stdout);
        out << "closeButton destroyed\n";
    });

    QObject::connect(this->_closeGenerateHLayout.get(), &QObject::destroyed, []{
        QTextStream out(stdout);
        out << "close vs generate HBoxLayout destroyed\n";
    });

    QObject::connect(this->_centeralVLayout.get(), &QObject::destroyed, []{
        QTextStream out(stdout);
        out << "VLayout destroyed\n";
    });
    ///////////////////// END OF TESTING ///////////////////

    this->_dialog->exec();

    return status;
}

