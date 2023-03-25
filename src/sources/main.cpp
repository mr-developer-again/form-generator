#include <headers/formgenerator.h>
#include <headers/aradstyleformgenerator.h>
#include <headers/getjsonpath.h>

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

void generate(QString const& filePath);

int main(int argc, char *argv[])
{
    try
    {
        QApplication a(argc, argv);

        QString filePath;
        JsonFilePath getJsonFilePath;
        JsonFilePathStatus status = getJsonFilePath(filePath);

        if (JsonFilePathStatus::GENERATE_FORM == status)
        {
            std::unique_ptr<Arad::GeneratingForm::FormGenerator> formGenerator;
            formGenerator = std::make_unique<Arad::GeneratingForm::AradStyleFormGenerator>(filePath);
            formGenerator->setupForm();

            return a.exec();
        }
        else if (JsonFilePathStatus::CANCEL == status)
        {
            ;
        }
        else if (JsonFilePathStatus::UNKNOWN_STATUS == status)
        {
            QMessageBox::information(nullptr, "What Happened!!", "Unknown Status!!!");
        }
    }
    catch (std::runtime_error const& ex)
    {
        QMessageBox::critical(nullptr, "RunTime Error", ex.what());
        return 1;
    }
    catch (std::invalid_argument const& ex)
    {
        QMessageBox::critical(nullptr, "Invalid Argument Error", ex.what());
        return 1;
    }
    catch(std::exception const& ex)
    {
        QMessageBox::critical(nullptr, "Error Occured", ex.what());
        return 1;
    }
    catch(...)
    {
        QMessageBox::critical(nullptr, "Error occured", "Something Went Wrong!!");
        return 1;
    }

    return 0;
}

void generate(QString const& filePath)
{

}
