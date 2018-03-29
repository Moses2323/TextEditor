/** @addtogroup TextToTabEditor
 * @{*/

/** @file */

#ifndef FILETOTAB_PRIV_GASPARYANMOSES_26102017
#define FILETOTAB_PRIV_GASPARYANMOSES_26102017

#include <QSyntaxHighlighter>
#include <QItemDelegate>
#include <QThread>

#include <string>

class QWidget;

namespace fttw{

/*! \~russian \brief Вспомогательный класс, позволяющий вводить в таблицу только double.
 *
 * Позволяет вводить в ячейку таблицы только double (от его минимального до
 * его максимального значения) с точностью до 15-ти знаков после запятой.
 *
 * Схема использования:
 * \code
 *
 *  int Nrow = 5;
 *  int Ncol = 3;
 *
 * 	QTableWidget* table = new QTableWidget(Nrow, Ncol);
 *	table->setItemDelegate(new OnlyDoubleDelegate);
 *  //далее заполняем таблицу числами как обычно
 *  //...
 *
 * \endcode
 *
 */
class OnlyDoubleDelegate : public QItemDelegate{
public:
	/*! \~russian
	 * \brief Отнаследованная от QItemDelegate виртуальная функция. Создана для того, чтобы нельзя было вводить в текстовое поле не вещественное число.
	 * \details Создает новый редактор однострочного текста и применяет к нему валидатор:
	 * устанавливает ввод числа с разделителем-точкой, с 15 знаками после запятой и границами от минимального значения double до максимального.
	 * За основу берется QDoubleValidator.
	 * \param parent указатель на виджета-родителя.
	 * \param option не используемый параметр.
	 * \param index не используемый параметр.
	 * \return указатель на редактор однострочного текста, к которому был применен валидатор.
	 */
	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &) const;
};

/*! \~russian \brief Вспомогательный класс для выделения
 *
 * Позволяет выделять синим все ключевые слова, которые относятся к разметке документа,
 * темно-красным все целые числа (int), красным — все вещественные числа (double).
 *
 * Схема использования:
 * \code
 *
 * QTextEdit *te = new QTextEdit;
 * HighLightNumbers = new HighLightNumbers(te);
 * //теперь все символы будут подсвечены согласно выделителю.
 * //...
 *
 * \endcode
 *
 */
class HighLightNumbers : public QSyntaxHighlighter{
private:
	/*! \~russian
	 * \brief Правило для выделения.
	 * \details Является структурой, содержащей в себе регулярное выражение и формат для всех слов, которые этому регулярному выражению соответствуют.
	 * В нашей программе используется только цвет слов из всех возможностей формата.
	 */
	struct HighlightRule{
		//! \~russian \brief Регулярное выражение для поиска слов.
		QRegExp pattern;
		//! \~russian \brief Формат для слов, соответствующих регулярному выражению.
		QTextCharFormat format;
	};

	//! \~russian \brief Формат для всех вещественных чисел (красный цвет). \details Создается в конструкторе и больше нигде не меняется.
	QTextCharFormat doubleFormat_;
	//! \~russian \brief Формат для всех целых чисел (темно-красный цвет). \details Создается в конструкторе и больше нигде не меняется.
	QTextCharFormat intFormat_;
	//! \~russian \brief Формат для всех ключевых слов, которые участвуют в разметке текста (синий). \details Создается в конструкторе и больше нигде не меняется.
	QTextCharFormat keywordFormat_;
	//! \~russian \brief Вектор по всеми правилами форматирования. \details Создается в конструкторе и больше нигде не меняется.
	QVector<HighlightRule> rules_;

	/*! \~russian
	 * \brief Функция инициализации.
	 * \details Вызывается только в конструкторе и только один раз.
	 *
	 */
	void init();

public:
	/*! \~russian
	 * \brief Пустой конструктор. Запускает функцию инициализации.
	 * \param pwg указатель на объект-предок. Аргумент может отсутствовать.
	 */
	explicit HighLightNumbers(QObject* pwg = nullptr) : QSyntaxHighlighter(pwg){init();}
	/*! \~russian
	 * \brief Конструктор с текстовым документом.
	 * \details Делает текстовый документ pwg родителем объекта данного класса, а также выполняет для pwg функцию highlightBlock(const QString&).
	 * \param pwg текстовый документ.
	 */
	explicit HighLightNumbers(QTextDocument* pwg) : QSyntaxHighlighter(pwg){init();}
	/*! \~russian
	 * \brief Функция форматирования текста в блоках.
	 * \details Виртуальная функция, в которой применяются правила выделения текста rules_ к одной строке str.
	 * \param str строка, в которой форматируются определенным образом символы.
	 */
	virtual void highlightBlock(const QString &str) override;
};


/*! \~russian
 * \brief Тред для блокнота.
 *
 * Тред используется, чтобы загружать файл в строковый буфер,
 * не останавливая работу приложения. На время использования
 * данного треда можно, например, поставить анимацию загрузки
 * на виджет.
 *
 * Также с помощью сигнально-слотового соединения можно задать реакцию
 * на завершение процесса.
 *
 * Схема использования:
 * \code
 *
 * // inside of some QObject class.
 *
 * std::string filename = "myFile.txt";
 * std::stringstream vss;
 *
 * FTTWFileBufferThread myThr(filename, vss);
 *
 * // prepearing
 * // ...
 *
 * // connect with finished signal
 * connect(&myThr, SIGNAL(finishedReading()), this, SLOT(reactionForFinished()));
 *
 * // run this thread
 * myThr.start();
 *
 * // another job
 * // ...
 *
 * \endcode
 *
 */
class FTTWFileBufferThread : public QThread{
	Q_OBJECT
private:
	//! \~russian \brief Имя файла, из которого происходит считывание.
	std::string filename_;
	//! \~russian \brief Ссылка на строковый буфер, в который происходит вывод того, что считано из файла.
	std::stringstream& s_;

protected:
	/*! \~russian
	 * \brief Функция запуска треда.
	 * \details Активируется, когда вызывается метод start().
	 * Производит вызов статической функции FileToTabWidget::createFileBuffer().
	 *
	 * После завершения считывания испускает сигнал finishedReading().
	 */
	void run();

public:
	/*! \~russian
	 * \brief Конструктор.
	 * \param filename имя файла, из которого будет производиться считывание.
	 * \param s ссылка на строковый буфер, в который будет считана информация из файла.
	 */
	FTTWFileBufferThread(const std::string& filename, std::stringstream& s);
	//! \~russian \brief Деструктор.
	~FTTWFileBufferThread();

signals:
	//! \~russian \brief Сигнал, который испускается после завершения процесса считывания.
	void finishedReading();
};

} // namespace fttw

#endif // FILETOTAB_PRIV_GASPARYANMOSES_26102017

/*@}*/
