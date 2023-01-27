#include <headers/formgenerator.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Arad::GeneratingForm::FormGenerator *formGenerator = new Arad::GeneratingForm::FormGenerator;
    formGenerator->show();

    return a.exec();
}
