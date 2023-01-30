#ifndef ARAD_GENERATINGFORM_ARADSTYLEFORMGENERATOR_ARADSTYLEFORMGENERATOR_H
#define ARAD_GENERATINGFORM_ARADSTYLEFORMGENERATOR_ARADSTYLEFORMGENERATOR_H

#include <headers/formgenerator.h>

#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>

namespace Arad
{

    namespace GeneratingForm
    {

        class AradStyleFormGenerator : public Arad::GeneratingForm::FormGenerator
        {
        public:
            explicit AradStyleFormGenerator(QString const& filePath, QWidget* parent = nullptr);
            virtual ~AradStyleFormGenerator();

            void setupForm() override;

        private:
            QHBoxLayout *_hBoxLayout = nullptr;
            QVector<QHBoxLayout*> _hBoxLayoutContainer;

            QVBoxLayout *_vBoxLayout = nullptr;

            QLabel *_label = nullptr;
            QVector<QLabel*> _labelContainer;

            QLineEdit *_lineEdit = nullptr;
            QVector<QLineEdit*> _lineEditContainer;

            QFrame *_splitterLine = nullptr;

        };

    } // GeneratingForm namespace

} // Arad namespace

#endif // ARAD_GENERATINGFORM_ARADSTYLEFORMGENERATOR_ARADSTYLEFORMGENERATOR_H
