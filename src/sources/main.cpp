#include <headers/formgenerator.h>
#include <headers/aradstyleformgenerator.h>

#include <stdexcept>
#include <QApplication>
#include <QMessageBox>

#include <QObject>
#include <QFileDialog>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include <algorithm>
#include <memory>
#include <functional>

enum class JsonPathStatus
{
    GENERATE = 0,
    CANCEL
};
// JsonPathStatus getJsonFilePath(QString& filePath);

struct getJsonFilePath
{
public:
    getJsonFilePath() = default;

    JsonPathStatus operator()(QString& filePath);

private:
    // std::unique_ptr<QDialog, std::function<void(QDialog*)>> dialog = \
    //     std::move(std::unique_ptr<QDialog, std::function<void(QDialog*)>>(nullptr, [](QDialog* dialog) -> void {
    //     dialog->deleteLater();
    // }));

    // std::unique_ptr<QLabel, std::function<void(QLabel*)>> label = \
    // std::move(std::unique_ptr<QLabel, std::function<void(QLabel*)>>(nullptr, [](QLabel* label) -> void {
    //     label->deleteLater();
    // }));
};

void Generate(QString const& filePath);

int main(int argc, char *argv[])
{
    try
    {
        QApplication a(argc, argv);

        QString filePath;
        getJsonFilePath jsonFilePath;
        JsonPathStatus status = jsonFilePath(filePath);

        switch (status)
        {
            case JsonPathStatus::GENERATE:
                break;
            case JsonPathStatus::CANCEL:
                break;
        }

        return a.exec();
    }
    catch(std::exception const& ex)
    {
        QMessageBox::critical(nullptr, "Error Occured", ex.what());
        return 1;
    }
}

JsonPathStatus getJsonFilePath::operator()(QString& filePath)
{
    // this->dialog.reset(new QDialog);
    std::unique_ptr<QDialog, std::function<void(QDialog*)>> dialog(new QDialog, [](QDialog* dialog) -> void {
    dialog->deleteLater();
    });
    dialog->setWindowFlags(Qt::WindowMinimizeButtonHint);
    // dialog->setFixedSize(dialog->geometry().width(), dialog->geometry().height());


    std::unique_ptr<QLabel, std::function<void(QLabel*)>> label(new QLabel, [](QLabel* label) -> void {
        label->deleteLater();
    });
    label->setText("JsonFile Path: ");

    std::unique_ptr<QLineEdit, std::function<void(QLineEdit*)>> lineEdit(new QLineEdit, [](QLineEdit* lineEdit) -> void {
        lineEdit->deleteLater();
    });
    lineEdit->setPlaceholderText("Please Enter Json File Path Here");

    std::unique_ptr<QPushButton, std::function<void(QPushButton*)>> browseButton(new QPushButton, [](QPushButton *pushButton) -> void {
        pushButton->deleteLater();
    });
    browseButton->setText("Browse");
    QObject::connect(browseButton.get(), &QPushButton::clicked, [&lineEdit]{
        std::unique_ptr<QFileDialog, std::function<void(QFileDialog*)>> fileDialog(new QFileDialog, [](QFileDialog* fileDialog) -> void {
            fileDialog->deleteLater();
        });

        QString filePath = fileDialog->getOpenFileName();
        if (filePath != "")
            lineEdit->setText(filePath);
    });

    std::unique_ptr<QHBoxLayout, std::function<void(QHBoxLayout*)>> pathHBoxLayout(new QHBoxLayout, [](QHBoxLayout* layout) -> void {
        layout->deleteLater();
    });
    pathHBoxLayout->addWidget(label.get());
    pathHBoxLayout->addWidget(lineEdit.get());
    pathHBoxLayout->addWidget(browseButton.get());

    std::unique_ptr<QPushButton, std::function<void(QPushButton*)>> generateFormButton(new QPushButton, [](QPushButton *pushButton) -> void {
        pushButton->deleteLater();
    });
    generateFormButton->setText("Generate Form");
    QObject::connect(generateFormButton.get(), &QPushButton::clicked, [&lineEdit, &filePath]{
        QString textOfLineEdit = lineEdit->text();

        if ("" == textOfLineEdit)
        {
            QMessageBox::critical(nullptr, "Json File Path", "You Must Enter Json File Path");
        }
        else
        {
            filePath = textOfLineEdit;
            return JsonPathStatus::GENERATE;
        }
    });

    std::unique_ptr<QPushButton, std::function<void(QPushButton*)>> cancelButton(new QPushButton, [](QPushButton *pushButton) -> void {
        pushButton->deleteLater();
    });
    cancelButton->setText("Cancel");
    QObject::connect(cancelButton.get(), &QPushButton::clicked, []{
        return JsonPathStatus::CANCEL;
    });

    std::unique_ptr<QHBoxLayout, std::function<void(QHBoxLayout*)>> saveCancelHLayout(new QHBoxLayout, [](QHBoxLayout* layout) -> void {
        layout->deleteLater();
    });
    saveCancelHLayout->addStretch();
    saveCancelHLayout->addWidget(cancelButton.get());
    saveCancelHLayout->addWidget(generateFormButton.get());
    saveCancelHLayout->addStretch();

    std::unique_ptr<QVBoxLayout, std::function<void(QVBoxLayout*)>> vLayout(new QVBoxLayout(dialog.get()), [](QVBoxLayout* layout) -> void {
        layout->deleteLater();
    });
    vLayout->addLayout(pathHBoxLayout.get());
    vLayout->addLayout(saveCancelHLayout.get());
    
    dialog->exec();
}

void Generate(QString const& filePath)
{
    std::unique_ptr<Arad::GeneratingForm::FormGenerator> formGenerator;
    if ("" != filePath)
    {
        try
        {
            formGenerator = std::make_unique<Arad::GeneratingForm::AradStyleFormGenerator>(filePath);
            formGenerator->setupForm();
        }
        catch (std::runtime_error const& ex)
        {
            QMessageBox::critical(nullptr, "RunTime Error", ex.what());
        }
        catch (std::invalid_argument const& ex)
        {
            QMessageBox::critical(nullptr, "Invalid Argument Error", ex.what());
        }
    }
}