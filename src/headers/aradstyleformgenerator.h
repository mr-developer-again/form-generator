#ifndef ARAD_GENERATINGFORM_ARADSTYLEFORMGENERATOR_H
#define ARAD_GENERATINGFORM_ARADSTYLEFORMGENERATOR_H

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
#include <QDialog>
#include <QComboBox>
#include <QStringList>
#include <QSpacerItem>

#include <QVector>
#include <QScopedPointer>

#include <functional>

namespace Arad
{

    namespace GeneratingForm
    {

        class AradStyleFormGenerator final : public Arad::GeneratingForm::FormGenerator
        {
            Q_OBJECT

        public:
            explicit AradStyleFormGenerator(QString const& filePath, QWidget* parent = nullptr);
            virtual ~AradStyleFormGenerator();

            void setupForm() override;

        private:
            void setFixedWidgetSize() noexcept;

            std::unique_ptr<QHBoxLayout, std::function<void(QHBoxLayout*)>> _hBoxLayout = \
                std::move(std::unique_ptr<QHBoxLayout, std::function<void(QHBoxLayout*)>>(nullptr, [](QHBoxLayout *layout) -> void {
                    layout->deleteLater();
                }));
            std::vector<std::unique_ptr<QHBoxLayout, std::function<void(QHBoxLayout*)>>> _hBoxLayoutContainer;

            std::unique_ptr<QVBoxLayout, std::function<void(QVBoxLayout*)>> _vBoxLayout = \
                std::move(std::unique_ptr<QVBoxLayout, std::function<void(QVBoxLayout*)>>(nullptr, [](QVBoxLayout* layout) -> void {
                    layout->deleteLater();
                }));

            std::unique_ptr<QLabel, std::function<void(QLabel*)>> _label = \
                std::move(std::unique_ptr<QLabel, std::function<void(QLabel*)>>(nullptr, [](QLabel* layout) -> void {
                    layout->deleteLater();
                }));
            std::vector<std::unique_ptr<QLabel, std::function<void(QLabel*)>>> _labelContainer;

            std::unique_ptr<QLineEdit, std::function<void(QLineEdit*)>> _lineEdit = \
                std::move(std::unique_ptr<QLineEdit, std::function<void(QLineEdit*)>>(nullptr, [](QLineEdit *lineEdit) -> void
                {
                    lineEdit->deleteLater();
                }));
            std::vector<std::unique_ptr<QLineEdit, std::function<void(QLineEdit*)>>> _lineEditContainer;

            std::unique_ptr<QFrame, std::function<void(QFrame*)>> _splitterLine = \
                std::move(std::unique_ptr<QFrame, std::function<void(QFrame*)>>(nullptr, [](QFrame* frame) -> void {
                    frame->deleteLater();
                }));
            std::vector<std::unique_ptr<QFrame, std::function<void(QFrame*)>>> _splitterLineContainer;

            std::unique_ptr<QSpinBox, std::function<void(QSpinBox*)>> _regularSpinBox = \
                std::move(std::unique_ptr<QSpinBox, std::function<void(QSpinBox*)>>(nullptr, [](QSpinBox *spinBox){
                    spinBox->deleteLater();
                }));
            std::vector<std::unique_ptr<QSpinBox, std::function<void(QSpinBox*)>>> _regularSpinBoxContainer;

            std::unique_ptr<QDoubleSpinBox, std::function<void(QDoubleSpinBox*)>> _doubleSpinBox = \
                std::move(std::unique_ptr<QDoubleSpinBox, std::function<void(QDoubleSpinBox*)>>(nullptr, [](QDoubleSpinBox *spinBox) -> void {
                    spinBox->deleteLater();
                }));
            std::vector<std::unique_ptr<QDoubleSpinBox, std::function<void(QDoubleSpinBox*)>>> _doubleSpinBoxContainer;

            std::unique_ptr<QCheckBox, std::function<void(QCheckBox*)>> _checkBox = \
                std::move(std::unique_ptr<QCheckBox, std::function<void(QCheckBox*)>>(nullptr, [](QCheckBox* checkBox) -> void {
                    checkBox->deleteLater();
                }));
            std::vector<std::unique_ptr<QCheckBox, std::function<void(QCheckBox*)>>> _checkBoxContainer;

            std::unique_ptr<QPushButton, std::function<void(QPushButton*)>> _pushButton = \
                std::move(std::unique_ptr<QPushButton, std::function<void(QPushButton*)>>(nullptr, [](QPushButton* pushButton) -> void {
                    pushButton->deleteLater();
                }));
            std::vector<std::unique_ptr<QPushButton, std::function<void(QPushButton*)>>> _pushButtonContainer;

            std::unique_ptr<QComboBox, std::function<void(QComboBox*)>> _comboBox = \
                std::move(std::unique_ptr<QComboBox, std::function<void(QComboBox*)>>(nullptr, [](QComboBox* comboBox) -> void {
                    comboBox->deleteLater();
                }));
            std::vector<std::unique_ptr<QComboBox, std::function<void(QComboBox*)>>> _comboBoxContainer;

            std::unique_ptr<QSpacerItem> _spacerItem;
            std::vector<std::unique_ptr<QSpacerItem>> _spacerItemContainer;

            /// @brief the following vector stores a hash table
            ///        about items (priority of items)
            /// @details the hash table stores items in the following style (the following keys are used
            ///          for storing data in hash table)
            ///             lineEdit
            ///             checkBox
            ///             comboBox
            ///             int spinBox
            ///             float spinBox
            ///             file
            QVector<QVector<QString>> _hashTable;

            int32_t _itemPrecedence = 0;

            /// @brief by the following variables, we want to keep index of
            ///        item inside related container (to access the item)
            int32_t _lineEditIndex = 0;
            int32_t _regularSpinBoxIndex = 0;
            int32_t _doubleSpinIndex = 0;
            int32_t _checkBoxIndex = 0;
            int32_t _comboBoxIndex = 0;

            void addItemToHashTable(int32_t itemPrecednece, QString const& itemType, int32_t indexOfItem);

        private slots:
            void slot_browsePushButtonClicked();
            void slot_saveButtonPressed();

        };

    } // GeneratingForm namespace

} // Arad namespace

#endif // ARAD_GENERATINGFORM_ARADSTYLEFORMGENERATOR_H
