#ifndef ARAD_GENERATINGFORM_ARADSTYLEFORMGENERATOR_ARADSTYLEFORMGENERATOR_H
#define ARAD_GENERATINGFORM_ARADSTYLEFORMGENERATOR_ARADSTYLEFORMGENERATOR_H

namespace Arad
{
    namespace GeneratingForm
    {
        class SwitchButton;
    }
}

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

namespace Arad
{

    namespace GeneratingForm
    {

        class AradStyleFormGenerator : public Arad::GeneratingForm::FormGenerator
        {
            Q_OBJECT

            struct BrowsingInFileSystem final
            {
                QDialog *dialog = new QDialog;
                QFileSystemModel *fileSystemModel = nullptr;
                QTreeView *treeView = nullptr;
                QHBoxLayout *hBoxLayout = new QHBoxLayout;
                QVBoxLayout *vBoxLayout = new QVBoxLayout(dialog);
                QPushButton *selectPushButton = new QPushButton(dialog);
                QSpacerItem *spacerItem;
                QString pathOfSelectedFile = "";
            };

            struct ThemesSwitchButton final : public QObject
            {
                ThemesSwitchButton() = default;
                ~ThemesSwitchButton();

                QHBoxLayout* operator()(QWidget *widget = nullptr,
                                        Arad::GeneratingForm::AradStyleFormGenerator *mainForm = nullptr);

            private:
                QHBoxLayout *_themeToggleHBoxLayout = nullptr;

                QVector<QString> _themes { "default", "dark" };
                int32_t _themeIndex = 0;
                int32_t counter = 0;
            };

        public:
            explicit AradStyleFormGenerator(QString const& filePath, QWidget* parent = nullptr);
            virtual ~AradStyleFormGenerator();

            void setupForm() override;

        protected:
            void generatedFormSizeFixer() noexcept;
            void JsonGenerator() const noexcept;

        private:
            QHBoxLayout *_hBoxLayout = nullptr;
            QVector<QHBoxLayout*> _hBoxLayoutContainer;

            QVBoxLayout *_vBoxLayout = nullptr;

            QLabel *_label = nullptr;
            QVector<QLabel*> _labelContainer;

            QLineEdit *_lineEdit = nullptr;
            QVector<QLineEdit*> _lineEditContainer;

            QFrame *_splitterLine = nullptr;
            QVector<QFrame*> _splitterLineContainer;

            QSpinBox *_regularSpinBox = nullptr;
            QVector<QSpinBox*> _regularSpinBoxContainer;

            QDoubleSpinBox *_doubleSpinBox = nullptr;
            QVector<QDoubleSpinBox*> _doubleSpinBoxContainer;

            QCheckBox *_checkBox = nullptr;
            QVector<QCheckBox*> _checkBoxContainer;

            QPushButton *_pushButton = nullptr;
            QVector<QPushButton*> _pushButtonContainer;

            QComboBox *_comboBox = nullptr;
            QVector<QComboBox*> _comboBoxContainer;

            QSpacerItem *_spacerItem = nullptr;
            QVector<QSpacerItem*> _spacerItemContainer;

            BrowsingInFileSystem *_browsingInFileSystem = nullptr;

            ThemesSwitchButton _themesSwitchButton;

            /// @brief the following vector stores a hash table
            ///        about items (priority of items)
            /// @details the hash table stores items in the following style
            ///             l  -> Line Edit
            ///             c  -> Check Box
            ///             C  -> Combo Box
            ///             s  -> Spin Box (signed integer spin box)
            ///             us -> Spin Box (unsigned interger spin box)
            ///             S  -> Spin Box (signed double spin box)
            ///             uS -> Spin Box (unsigned double spin box)
            ///             f  -> file
            QVector<QVector<QString>> _hashTable;

            int32_t _itemPrecedence = 0;

            /// @brief by the following variables, we want to keep index of
            ///        item inside related container (to access the item)
            int32_t _pushButtonIndex = 0;
            int32_t _labelIndex = 0;
            int32_t _lineEditIndex = 0;
            int32_t _signedRegularSpinBoxIndex = 0;
            int32_t _unsignedRegularSpinBoxIndex = 0;
            int32_t _signedDoubleSpinIndex = 0;
            int32_t _unsignedDoubleSpinIndex = 0;
            int32_t _checkBoxIndex = 0;
            int32_t _comboBoxIndex = 0;
            int32_t _fileIndex = 0;

            void addItemToHashTable(int32_t itemPrecednece, QString const& itemType, int32_t indexOfItem);

        private slots:
            void slot_browsePushButtonClicked();
            void slot_treeViewDoubleClicked(QModelIndex index);
            void slot_selectPushButtonClicked();

        };

    } // GeneratingForm namespace

} // Arad namespace

#endif // ARAD_GENERATINGFORM_ARADSTYLEFORMGENERATOR_ARADSTYLEFORMGENERATOR_H
