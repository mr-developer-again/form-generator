#include <headers/formgenerator.h>
#include <headers/aradstyleformgenerator.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString filePath = "/home/arad/projects/qt_projects/form-generator/src/sample.json";

    Arad::GeneratingForm::FormGenerator *formGenerator = new Arad::GeneratingForm::AradStyleFormGenerator(filePath);
    formGenerator->setupForm();

    return a.exec();
}
