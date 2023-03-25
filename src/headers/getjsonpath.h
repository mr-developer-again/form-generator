#ifndef GETJSONPATH_H
#define GETJSONPATH_H


#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <memory>

enum class JsonFilePathStatus
{
    GENERATE_FORM = 1,
    CANCEL,
    UNKNOWN_STATUS
};

class JsonFilePath
{
public:
    JsonFilePath(QDialog *parent = nullptr);
    ~JsonFilePath() = default;

    JsonFilePathStatus operator()(QString& filePath);

private:
    std::unique_ptr<QDialog> _dialog;
    std::unique_ptr<QLabel> _label;
    std::unique_ptr<QLineEdit> _filePathLineEdit;
    std::unique_ptr<QPushButton> _browsePushButton;
    std::unique_ptr<QHBoxLayout> _browseHLayout;

    std::unique_ptr<QPushButton> _generatePushButton;
    std::unique_ptr<QPushButton> _cancelButton;
    std::unique_ptr<QHBoxLayout> _closeGenerateHLayout;

    std::unique_ptr<QVBoxLayout> _centeralVLayout;
};

#endif // GETJSONPATH_H

