#ifndef ARAD_GENERATINGFORM_ARADSTYLEFORMGENERATOR_ARADSTYLEFORMGENERATOR_H
#define ARAD_GENERATINGFORM_ARADSTYLEFORMGENERATOR_ARADSTYLEFORMGENERATOR_H

#include <headers/formgenerator.h>

#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QFileSystemModel>
#include <QTreeView>

namespace Arad
{

    namespace GeneratingForm
    {

        class AradStyleFormGenerator : public QPushButton, public Arad::GeneratingForm::FormGenerator
        {
            Q_OBJECT

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

            QSpinBox *_regularSpinBox = nullptr;
            QVector<QSpinBox*> _regularSpinBoxContainer;

            QDoubleSpinBox *_doubleSpinBox = nullptr;
            QVector<QDoubleSpinBox*> _doubleSpinBoxContainer;

            QCheckBox *_checkBox = nullptr;
            QVector<QCheckBox*> _checkBoxContainer;

            QPushButton *_pushButton = nullptr;
            QVector<QPushButton*> _pushButtonContainer;

            QFileSystemModel *_fileSystemModel = nullptr;

            QTreeView *_treeView = nullptr;

        private slots:
            void slot_browsePushButtonClicked();

        };

    } // GeneratingForm namespace

} // Arad namespace

#endif // ARAD_GENERATINGFORM_ARADSTYLEFORMGENERATOR_ARADSTYLEFORMGENERATOR_H
