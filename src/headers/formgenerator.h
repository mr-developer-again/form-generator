#ifndef ARAD_GENERATINGFORM_FORMGENERATOR_H
#define ARAD_GENERATINGFORM_FORMGENERATOR_H

#include <headers/jsonparser.h>

#include <QMainWindow>

#include <memory>
#include <functional>

class QWidget;
class QString;

namespace Arad
{
    namespace GeneratingForm
    {

        class FormGenerator : public QMainWindow
        {
        public:
            explicit FormGenerator(QString const& filePath);
            virtual ~FormGenerator();

            virtual void setupForm() = 0;

            /// this method loads some file and colorize the main widget
            /// (and also changes style of the widget)
            virtual void darkTheme();

            /// @brief This method change current theme to a abitrary theme
            /// @param themeName: Theme name that we want to make it as the current them
            ///        (valid themes that you can mention as param:
            ///             1. dark  2. default)
            virtual void toggleTheme(QString const& themeName);

        protected:
            virtual void setFilePath(QString const& filePath);
            virtual QString getFilePath() const noexcept;

            /// @brief an unique-pointer to widget
            std::unique_ptr<QWidget, std::function<void(QWidget*)>> _widget = \
                std::move(std::unique_ptr<QWidget, std::function<void(QWidget*)>>(nullptr, [](QWidget* widget) -> void {
                    widget->deleteLater();
                }));

            /// @brief an unique-pointer to json parser base-class (for polymorphic reasons)
            std::unique_ptr<Arad::Parser::JsonParser> _jsonParser;

            virtual bool validInteger(QString const& inputString) const noexcept;
            virtual bool validDouble(QString const& inputString) const noexcept;

        private:
            QString _filePath = "";
        };

    } // GeneratingForm namespace

} // Arad namespace


#endif // ARAD_GENERATINGFORM_FORMGENERATOR_H
