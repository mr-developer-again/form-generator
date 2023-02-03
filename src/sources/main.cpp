#include <headers/formgenerator.h>
#include <headers/aradstyleformgenerator.h>

#include <stdexcept>
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString filePath = "/home/ali/projects/cpp_projects/Qt Projects/3.form_generator/src/sample.json";

    Arad::GeneratingForm::FormGenerator *formGenerator = nullptr;

    QMessageBox mesgBx;
    try
    {
        formGenerator = new Arad::GeneratingForm::AradStyleFormGenerator(filePath);
        formGenerator->setupForm();
    }
    catch (std::runtime_error const& ex)
    {
        mesgBx.setText(ex.what());
        mesgBx.exec();
    }
    catch (std::invalid_argument const& ex)
    {
        mesgBx.setText(ex.what());
        mesgBx.exec();
    }

    return a.exec();
}
