/** @addtogroup TextToTabEditor
 * @{*/

/** @file */

#ifndef SIMPLEARTIFICIALSHELL200217_GASPARYANMOSES
#define SIMPLEARTIFICIALSHELL200217_GASPARYANMOSES

#include <texttotabeditorsettings.h>

#include <QWidget>

class QLineEdit;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QLabel;
class QSplitter;
namespace fttw{
    class OutSideWidget;

/*! \~russian \brief Виджет для открытия файла в специальном редакторе.
 *
 * Виджет с кнопкой для открытия диалогового окна,
 * которое позволяет выбрать in-файл для специального
 * текстового редактора.
 * Текстовый редактор открывает файл в соответствии с метками:
 *
 * либо как файл с таблицами и вкладками, либо как обычный
 * текстовый редактор, но с подсветкой синтаксиса (подробности можно
 * найти в описании класса fttw::FileToTabWidget).
 *
 * Класс содержит слоты для открытия файла в специальном текстовом редакторе.
 * Использовать как обычный виджет.
 *
 * Схема использования:
 * \code
 *
 * 	SimpleArtificialShell* mW = new SimpleArtificialShell;
 *	mW->show();
 *
 * \endcode
 *
 */
class TEXTTOTABEDITOR_EXPORT TextToTabEditorShell : public QWidget{
	Q_OBJECT
private:
	//! \~russian \brief Главный виджет с текстовым редактором и его кнопками.
	fttw::OutSideWidget *fileToTab_ {nullptr};
	//! \~russian \brief Главный менеджер размещения для данного виджета.
	QVBoxLayout *mL_;
	//! \~russian \brief Менеджер размещения для текстового редактора (название входного файла) и кнопки.
	QHBoxLayout *buttonLayout_;
	//! \~russian \brief Кнопка для открытия файла. \details Связана со слотом chooseFileForEditing().
	QPushButton *openFileButton_;

	/*! \~russian
	 * \brief Текстовый редактор с именем файла, который сейчас открыт в специальном текстовом редакторе.
	 * \details Основан на неизменяемом текстовом редакторе QLineEdit.
	 * Сигнал изменения текста внутри редактора textChanged(QString) связан со слотом
	 * openFileForEditing(QString).
	 */
    QLineEdit* inFileLab_;

	//! \~russian \brief Папка по умолчанию, которая открывается перед пользователем при нажатии кнопки "Open in-file". Устанавливается внешним виджетом, если нужно.
	QString defaultDir_ {""};
	//! \~russian \brief Фильтры на файлы в QFileDialog, которые применяются при открытии файла пользователем при нажатии кнопки "Open in-file". Устанавливается внешним виджетом, если нужно.
	QString filters_ {""};

	/*! \~russian
	 * \brief Функция проверяет, существует ли файл.
	 * \details Если не существует, то создает новый пустой файл.
	 * Также в случае неудачи выдает окно с предупреждением.
	 * \param filename имя файла, который необходимо проверить.
	 * \return false, если файл создать не удалось.
	 */
	bool checkExistenceAndCreateIfNot(const QString& filename) const;

public:
	/*! \~russian
	 * \brief Конструктор.
	 * \details Создает менеджер размещения, кнопки, располагает
	 * их по виджету. Соединяет необходимые сигналы со слотами.
	 * \param obj указатель на виджет-родитель.
	 */
	explicit TextToTabEditorShell(QWidget* obj = nullptr);
	//! \~russian \brief Деструктор.
	virtual ~TextToTabEditorShell() {}

	/*! \~russian
	 * \brief Возвращает главный виджет с текстовым редактором и его кнопками.
	 * \details Читает защищенное поле fileToTab_. fileToTab_ можно менять.
	 * Данная функция может быть использована, чтобы изменить стиль оформления редактора.
	 * \return указатель на виджет с текстовым редактором и его кнопками.
	 */
	fttw::OutSideWidget* get_OutsideFileToTabWidget() {return fileToTab_;}

	/*! \~russian
	 * \brief Возвращает главный виджет с текстовым редактором и его кнопками.
	 * \details Читает защищенное поле fileToTab_. fileToTab_ нельзя менять.
	 * \return указатель на константный виджет с текстовым редактором и его кнопками.
	 */
	const fttw::OutSideWidget* get_OutsideFileToTabWidget() const {return fileToTab_;}

	/*! \~russian
	 * \brief Возвращает кнопку для открывания диалогового окна выбора файла.
	 * \details Читает защищенное поле openFileButton_. openFileButton_ можно менять.
	 * \return указатель на кнопку для открытия диалогового окна выбора файла.
	 */
	QPushButton* get_openFileButton() {return openFileButton_;}

	/*! \~russian
	 * \brief Возвращает кнопку для открывания диалогового окна выбора файла. Константная версия.
	 * \details Читает защищенное поле openFileButton_. openFileButton_ нельзя менять.
	 * \return
	 */
	const QPushButton* get_openFileButton() const {return openFileButton_;}

	/*! \~russian
	 * \brief Возвращает имя файла, который сейчас открыт в текстовом редакторе.
	 * \details Читает часть приватного поля inFileLab_.
	 * \return строку, содержащую имя файла, открытого в текстовом редакторе.
	 */
	QString get_inFileName() const;

	/*! \~russian
	 * \brief Возвращает текстовый редактор с именем файла,
	 * который открыт сейчас в специальном текстовом редакторе.
	 * \details Читает приватное поле inFileLab_. inFileLab_ можно менять.
	 * \return указатель на текстовый редактор с именем файла,
	 * который открыт сейчас в специальном текстовом редакторе.
	 */
	QLineEdit* get_inFileLineEditor() {return inFileLab_;}

	/*! \~russian
	 * \brief Возвращает текстовый редактор с именем файла, который открыт сейчас в специальном текстовом редакторе.
	 * \details Читает приватное поле inFileLab_. inFileLab_ нельзя менять.
	 * \return указатель на константный текстовый редактор с именем файла.
	 */
	const QLineEdit* get_inFileLineEditor() const {return inFileLab_;}

	/*! \~russian
	 * \brief Устанавливает имя папки по умолчанию, открывающейся перед пользователем при нажатии кнопки "Open file".
	 * \details Изменяет приватное поле defaultDir_.
	 * \param dirIn новое имя папки по умолчанию.
	 */
	void set_defaultDir(const QString& dirIn) { defaultDir_ = dirIn; }

	/*! \~russian
	 * \brief Возвращает имя папки по умолчанию, открывающейся перед пользователем при нажатии кнопки "Open file".
	 * \details Читает приватное поле defaultDir_.
	 * \return константную ссылку на имя директории по умолчанию.
	 */
	const QString& get_defaultDir() const { return defaultDir_; }

	/*! \~russian
	 * \brief Функция устанавливает фильтры, которые применяются при открытии пользователем диалогового окна при нажатии кнопки "Open file".
	 * \param newFilters
	 */
	void set_fileFilters( const QString& newFilters ) { filters_ = newFilters; }

	/*! \~russian
	 * \brief Функция возвращает строку, содержащую фильтры, применяющиеся при открытии пользователем диалогового окна при нажатии кнопки "Open file".
	 * \return константную ссылку на строку, содержащую фильтры.
	 */
	const QString& get_fileFilters() const { return filters_; }

	/*! \~russian
	 * \brief Функция для перевода элементов виджета "на лету".
	 */
	void retranslate();

private slots:
	/*! \~russian
	 * \brief Открывает указанный файл с помощью специального редактора (таблицы/вкладки или текстовый редактор).
	 * \details Не меняет inFileLab_. Вызывается сигналом изменения текста в inFileLab_.
	 * \param str имя файла, который будет открыт в специальном редакторе.
	 */
	void openFileForEditing(QString str);


public slots:
	/*! \~russian
	 * \brief Создает диалоговое окно выбора файла.
	 * \details затем открывает файл в специальном текстовом редакторе
	 * (вызывает метод openFileForEditing через сигнал).
	 */
	void chooseFileForEditing();

	/*! \~russian
	 * \brief Сравнивает имя файла в inFileLab_ и str. Если не совпадает, то заменяет текущее имя in-файла на str.
	 * \param str имя файла, который будет открыт в специальном редакторе.
	 */
	void openFileForEditingWithLineEditor(const QString& str);

};

} // namespace fttw

#endif // SIMPLEARTIFICIALSHELL200217_GASPARYANMOSES

/*@}*/
