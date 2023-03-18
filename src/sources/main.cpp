#include <headers/formgenerator.h>
#include <headers/aradstyleformgenerator.h>

#include <stdexcept>
#include <QApplication>
#include <QMessageBox>

#include <algorithm>
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString filePath = "/home/arad/projects/projects3/qt_projects/form-generator/src/sample.json";

    std::unique_ptr<Arad::GeneratingForm::FormGenerator> formGenerator;
    try
    {
        formGenerator = std::make_unique<Arad::GeneratingForm::AradStyleFormGenerator>(filePath);
        formGenerator->darkTheme();
        formGenerator->setupForm();
    }
    catch (std::runtime_error const& ex)
    {
        QMessageBox mesgBx;
        mesgBx.setText(ex.what());
        mesgBx.setWindowTitle("runtime error");
        mesgBx.exec();
    }
    catch (std::invalid_argument const& ex)
    {
        QMessageBox mesgBx;
        mesgBx.setText(ex.what());
        mesgBx.setWindowTitle("invalid_argument error");
        mesgBx.exec();
    }

    return a.exec();
}
