/** @addtogroup TextEditor
 * @{*/

/** @file */

#ifndef FILETOTABWIDGET16012017GASPARYANMOSES_H
#define FILETOTABWIDGET16012017GASPARYANMOSES_H

#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QTabWidget>
#include <QTextEdit>
#include <QTableWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QItemDelegate>
#include <QHeaderView>
#include <QSyntaxHighlighter>
#include <QPushButton>
#include <QSplitter>
#include <QGridLayout>
#include <QThread>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <limits>
#include <iterator>
#include <memory>

#include "printelement.h"
#include "LoaderWidget.h"

namespace fttw{

/*! \~russian
 * \brief Преобразует элемент на входе в строку.
 * \details У элемента должен быть определен оператор вывода,
 * т.к. преобразование происходит за счет простого вывода в строковый поток.
 * \param elem параметр, который будет преобразован в строку.
 * \return строку, состоящую из входного параметра.
 */
template<typename T>
inline std::string toStr(T elem) {
	std::stringstream vss;
	vss<<elem;
	return vss.str();
}

/*! \~russian
 * \brief Печатает в поток ошибок сообщение об ошибке.
 * \details Печатает в поток ошибок сообщение об ошибке с указанием функции,
 * в которой произошла ошибка и номер строки,
 * на которой произошла ошибка и имя файла, а затем выходит из программы с помощью throw.
 * \param funcName имя функции, которое будет выведено (часто используется макрос __FUNCTION__ с дополнительным указанием аргументов).
 * \param line номер строки, на которой вызвана эта функция (зачастую достаточно использовать макрос __LINE__).
 * \param fileName имя файла, в котором находится функция, из которой произошел вызов функции-ошибки (зачастую достаточно использовать макрос __FILE__).
 * \param message сообщение об ошибке. Должно быть введено вручную.
 */
inline void print_mistake(const std::string& funcName, int line, const std::string& fileName, const std::string& message){
	std::cerr<<"ERROR in "<<funcName<<" : line "<<line<<", file "<<fileName<<std::endl;
	std::cerr<<"\t"<<message<<std::endl;
	throw;
}

/*! \~russian
 * \brief Печатает в поток ошибок сообщение о предупреждении.
 * \details Печатает в поток ошибок сообщение о предупреждении с указанием функции,
 * в которой произошла ошибка и номер строки,
 * на которой произошла ошибка и имя файла. Не завершает работу программы.
 * \param funcName имя функции, которое будет выведено (часто используется макрос __FUNCTION__ с дополнительным указанием аргументов).
 * \param line номер строки, на которой вызвана эта функция (зачастую достаточно использовать макрос __LINE__).
 * \param fileName имя файла, в котором находится функция, из которой произошел вызов данной функции (зачастую достаточно использовать макрос __FILE__).
 * \param message сообщение-предупреждение. Должно быть введено вручную.
 */
inline void print_mistakeLite(const std::string& funcName, int line, const std::string& fileName, const std::string& message) {
	std::cerr<<"WARNING in "<<funcName<<" : line "<<line<<", file "<<fileName<<std::endl;
	std::cerr<<"\t"<<message<<std::endl;
}

/*! \~russian
 * \brief Считывает из строчного потока до символа '\n' не включая и возвращает строку.
 * \param ss поток ввода, из которого происходит считывание.
 * \return строку до символа '\n'.
 */
inline std::string getOneLine(std::istream& ss){
	std::string res = "";
	char c = 0;
	ss>>std::noskipws;
	ss>>c;
	while((c != '\n') && (!ss.eof())){
		res.push_back(c);
		ss>>c;
	}
	ss>>std::skipws;
	return res;
}

/*! \~russian
 * \brief Считывает из строчного потока до символа '\n' не включая и записывает результат в vs.
 * \param ss поток ввода, из которого происходит считывание.
 * \param vs сслыка на строку, которая будет изменена данной функцией.
 * \return true, если поток не закончился
 */
inline bool getOneLine(std::istream& ss, std::string& vs) {
	ss>>std::noskipws;
	vs = "";
	char c = 0;

	ss>>c;
	if ((c != '\n') && (!ss.eof()))
		vs.push_back(c);
	else{
		ss>>std::skipws;
		if (ss.eof())
			return false;
		else
			return true;
	}
	while(ss>>c){
		if (c != '\n')
			vs.push_back(c);
		else{
			ss>>std::skipws;
			return true;
		}
	}

	ss>>std::skipws;
	return false;
}

/*! \~russian
 * \brief Проверяет, является ли строка вещественным числом.
 * \param s входная строка.
 * \return true, если в строке содержится только вещественное число.
 */
inline bool isDouble(const std::string& s) {
	QString vs = QString::fromStdString(s);
	bool res = false;
	vs.toDouble(&res);
	return res;
}

/*! \~russian
 * \brief Преобразует строку в указанный тип.
 * \details У объекта должен быть определен оператор ввода (>>).
 * Также не гарантируется успешность данного преобразования.
 * \return объект указанного типа.
 */
template<typename T>
inline T fromStringTo(const std::string& s) {
	std::stringstream vss;
	vss.str(s);
	T res;
	vss>>res;
	return res;
}

/*! \~russian
 * \brief Печатает имя родителя объекта obj, если он существует.
 * \details Данная функция нужна только для отладки. Если родителя нет, то выводится строка "parent = nullptr".
 * \param s поток вывода, куда выводится информация о родителе объекта obj.
 * \param obj указатель на объект, у которого спрашивается информация о родителе.
 * \return ссылку на поток вывода.
 */
inline std::ostream& print_parent(std::ostream& s, const QObject* obj) {
	if (obj->parent() != nullptr)
		s<<"parent = "<<obj->parent()->objectName().toStdString();
	else
		s<<"parent = nullptr";
	return s;
}

/*! \~russian
 * \brief Печатает имя объекта obj, если он существует.
 * \details Если нет объекта - выводит "nullptr". также печатает имя родителя obj, если это возможно.
 * Данная функция нужна только для отладки.
 * \param s поток вывода, куда выводится информация об объекте obj.
 * \param obj указатель на объект, у которого спрашивается информация.
 * \return ссылку на поток вывода.
 */
inline std::ostream& tryToPrint(std::ostream& s, const QObject* obj) {
	if (obj != nullptr){
		s<<obj->objectName().toStdString()<<", ";
		print_parent(s, obj);
	}
	else
		s<<"nullptr";
	return s;
}

/*! \~russian \brief Вспомогательный класс, позволяющий вводить в таблицу только double.
 *
 * Позволяет вводить в ячейку таблицы только double от его минимального до
 * его максимального значения с точностью до 15-ти знаков после запятой.
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
	Q_OBJECT
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
	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};




/*! \~russian \brief Вспомогательный класс для выделения
 *
 * Позволяет выделять синим все ключевые слова, которые относятся к разметке документа,
 * темно-красным все целые числа(int), красным - все вещественные числа (double).
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
	Q_OBJECT
private:
	/*! \~russian
	 * \brief Правило для выделения.
	 * \details Является структурой, содержащей в себе регулярное выражение и формат для всех слов, которые этому регулярному выражению соответствуют
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




class FTTWFileBufferThread;

/*! \~russian \brief Класс, преобразующий файл в виджет и обратно.
 *
 * Объект данного класса может создать виджет на основе текстового файла,
 * оформленного определенным образом. После внесенных изменений может
 * сохранить данные в формате, который можно будет считать этой же программой.
 *
 * Если текстовый документ оформлен некорректно, то открывается обычный
 * текстовый редактор.
 *
 * Имеет несколько слотов для загрузки, сохранения, очистки.
 *
 * Схема использования:
 * \code
 *
 *	string filename = "/home/moses/Documents/Qt_workspace/FileToWidget/FileToWidget/infiles/infile.in";
 *
 *	fttw::FileToTabWidget mW(filename);
 *	mW.show();
 *
 * \endcode
 *
 */
class FileToTabWidget : public QScrollArea{
	Q_OBJECT
private:
	/*! \~russian
	 * \brief Глобальные переменные.
	 * \details Отображаются как Label    value в виджете.
	 * В текстовом файле отмечаются как:
	 *
	 * \#scalar название_переменной1 название_переменной2 ...
	 *
	 * значение1 значение2 ...
	 *
	 * Пример:
	 *
	 * \#scalar dt T
	 *
	 * 0.1 1000
	 */
	std::vector<std::pair<QLabel*, QLineEdit*> > globalValues_;

	/*! \~russian
	 * \brief Таблицы с данными таблиц.
	 * \details Каждый \#matrix - в файле - это новая таблица.
	 * В программе каждая матрица владеет своей вкладкой.
	 * В текстовом файле записываются как
	 *
	 * \#matrixname Имя_матрицы
	 *
	 * \#matrix Va Vb Vc
	 *
	 * 1 2 3
	 *
	 * 4 5 6
	 *
	 * 7 8 9
	 *
	 * тогда имя вкладки будет соответствовать имени матрицы.
	 *
	 * Или же:
	 *
	 * \#matrix Va Vb Vc
	 *
	 * 1 2 3
	 *
	 * 4 5 6
	 *
	 * 7 8 9
	 *
	 * тогда имя вкладки будет соответствовать порядковому номеру данной матрицы.
	 *
	 */
	std::vector<QTableWidget*> tables_;

	/*! \~russian
	 * \brief Таблицы с данными векторов.
	 * \details Каждый \#vectors или \#vector - в файле - это новая таблица.
	 * В программе каждый вектор владеет своей вкладкой.
	 * Вектора отличаются от матриц тем, что указанные после ключевого слова имена
	 * соответствуют именам строк, а не столбцов.
	 * В текстовом файле записываются как
	 *
	 * \#vectorsname Имя_вкладки_с_векторами
	 *
	 * \#vectors Va Vb Vc
	 *
	 * 1 2 3 4 5
	 *
	 * 2 3 4 5 6
	 *
	 * 3 4 5 6 7
	 *
	 * тогда имя вкладки будет соответствовать имени векторов.
	 *
	 * Или же:
	 *
	 * \#vectors Va Vb Vc
	 *
	 * 1 2 3 4 5
	 *
	 * 2 3 4 5 6
	 *
	 * 3 4 5 6 7
	 *
	 * тогда имя вкладки будет соответствовать порядковому номеру данных векторов.
	 *
	 * Также можно написать
	 *
	 * \#vector Va
	 *
	 * 1 2 3 4 5 6 7 8 9 10
	 *
	 * Тогда появится вкладка с именем Va с одним вектором с именем Va.
	 */
	std::vector<QTableWidget*> vectors_;
	//! \~russian \brief Главный виджет, на который все накладывается.
	QFrame *mW_ {nullptr};
	//! \~russian \brief Менеджер размещения для главного виджета mW_.
	QBoxLayout *mL_ {nullptr};
	//! \~russian \brief Вкладки с таблицами.
	QTabWidget *tabs_ {nullptr};
	//! \~russian \brief Текстовый редактор. Используется, если файл на входе некорректно размечен или указан соответствующий режим.
	QTextEdit* unmarkedValues_ {nullptr};
	//! \~russian \brief Специальный выделитель слов в тексте для текстового редактора QTextEdit.
	HighLightNumbers* textEditorHighLighter_ {nullptr};
	//! \~russian \brief Массив с элементами, у которых вызывается функция для печати. \details В зависимости от внутренней реализации элемента, печать происходит по-разному. Все таблицы, вектора, глобальные переменные, комментарии записываются в этот массив.
	std::vector<PrintElement*> printElements_;

	//! \~russian \brief Разделитель вкладок с таблицами от виджетов с переменными. Позволяет менять соотношение их относительно друг друга на виджете.
	QSplitter* tabsFromLabelsSplitter_;
	//! \~russian \brief Область с прокруткой для части с параметрами (глобальными переменными). \details в нее вкладывается виджет widgetWithLabels_.
	QScrollArea* labelsScrollArea_;
	//! \~russian \brief Виджет, необходимый для области с прокруткой для параметров.
	QWidget* widgetWithLabels_;
	//! \~russian \brief Менеджер размещения для виджета с прокруткой и параметрами. \details В данный менеджер размещения добавляются по одному QLabel и QLineEdit для каждого параметра в отдельности.
	QGridLayout* widgetWithLabelsLayout_;
	//! \~russian \brief Виджет, отображающий прокрутку (процесс загрузки). \details Используется, чтобы не блокировать работу приложения и показать пользователю, что приложение работает, а не зависло.
	LoaderWidget loaderWidget;

	//! \~russian \brief Тред, в который отдается загрузка файла в строковый буфер. \details Когда происходит загрузка, на место виджета блокнота ставится виджет прокрутки.
	std::unique_ptr<FTTWFileBufferThread> fttwFBthread_;
	//! \~russian \brief Строковый буфер. \details Используется как вспомогательный буфер. Ссылка на него используется в другом треде иногда.
	std::stringstream fttwSS_;

public:
	//! \~russian \brief Тип поведения.
	enum class mode : char {
		//! \~russian будет сделана попытка по текстовому файлу создать виджет с таблицами и окошками для глобальных переменных.
		Tabs,
		//! \~russian по файлу будет создан текстовый редактор.
		TextEditor
	};

	/*! \~russian
	 * \brief Функция открывает файл filename и считывает всю информацию из него в буфер s.
	 * \details Эта функция используется в отдельном потоке в блокноте.
	 * \param filename имя файла, из которого считывается информация.
	 * \param s буфер, в который записывается информация.
	 */
	static void createFileBuffer(const std::string& filename, std::ostream& s);

private:
	//! \~russian \brief Значение типа поведения. Может быть задано в конструкторе.
	mode mode_;

	/*! \~russian
	 * \brief Инициализация менеджера размещения.
	 * \details Создает главный менеджер размещения и прикрепляет его к главному виджету.
	 * Используется лишь один раз в функции main_init(). Изменяет защищенные поля mL_ и mW_.
	 */
	void layout_init();

	/*! \~russian
	 * \brief Функция делает из строки с метками один вектор, где каждым элементом является метка.
	 * \param labels метки, записанные в строку через разделитель.
	 * \param vec вектор строк, в который записываются метки по отдельности.
	 */
	void createLabelVector(const std::string& labels, std::vector<std::string>& vec) const;

	/*! \~russian
	 * \brief Функция считывает глобальные параметры с именами в строке labels.
	 * \details Строка уже не должна содержать "#scalar".
	 * Считывает глобальные параметры, их имена. На каждый параметр создает свой QLabel и QLineEdit и добавляет их в менеджер размещения.
	 * Изменяет защищенные поля globalValues_, widgetWithLabelsLayout_.
	 * Строка уже должна быть корректно отформатирована.
	 * Данная функция является вспомогательной и вызывается в createTabsAndGlobalValues() после ( check_correct() == true ).
	 * \param ss поток ввода, из которого происходит считывание.
	 * \param labels несколько меток в одной строке, каждая из которых обозначает отдельный параметр.
	 * \param rowCounter номер следующей строки по счету, куда можно вставлять переменную. Изменяется данной функцией, после того как переменные были вставлены.
	 * \param columnCounter номер столбца, в который можно сейчас вставить новую переменную. Изменяется данной функцией с 1 на 0 и наоборот.
	 * \return количество созданных глобальных переменных.
	 */
	size_t readGlobalValues(std::istream& ss, const std::string& labels, int& rowCounter, int& columnCounter);

	/*! \~russian
	 * \brief Считывает таблицу с данными в виде матрицы.
	 * \details Строка уже не должна содержать "#matrix".
	 * Считывает до тех пор, пока подряд идут вещественные числа.
	 * Возвращает количество пустых строк после таблицы.
	 * Изменяет защищенное поле tables_.
	 * Строка уже должна быть корректно отформатирована.
	 * Данная функция является вспомогательной и вызывается в createTabsAndGlobalValues() после ( check_correct() == true ).
	 * \param ss поток ввода, из которого происходит считывание.
	 * \param labels метки с названиями столбцов таблицы.
	 * \return количество пустых строк после таблицы.
	 */
	size_t readTable(std::istream& ss, const std::string& labels);

	/*! \~russian
	 * \brief Считывает таблицу с данными в виде вектора.
	 * \details Строка уже не должна содержать "#vector" или "#vectors".
	 * Считывает до тех пор, пока подряд идут вещественные числа.
	 * Возвращает количество пустых строк после таблицы.
	 * Изменяет защищенное поле tables_.
	 * Строка уже должна быть корректно отформатирована.
	 * Данная функция является вспомогательной и вызывается в createTabsAndGlobalValues() после ( check_correct() == true ).
	 * \param ss поток ввода, из которого происходит считывание.
	 * \param labels метки с названиями строк таблицы.
	 * \return количество пустых строк после таблицы.
	 */
	size_t readVector(std::istream& ss, const std::string& labels);

	/*! \~russian
	 * \brief Создает под уже оформленную таблицу специальную отдельную вкладку в виджете со вкладками.
	 * \details Название вкладки - ее номер.
	 * Если виджет со вкладками ещё не был создан, то создает его. Изменяет защищенное поле tabs_.
	 * \param table таблица, которую необходимо добавить во вкладку.
	 */
	void addTableToTab(QTableWidget* table);

	/*! \~russian
	 * \brief Перегруженная функция. Создает под уже оформленную таблицу специальную отдельную вкладку в виджете со вкладками и именем.
	 * \details Название вкладки передается в данную функцию в качестве аргумента.
	 * Если виджет со вкладками ещё не был создан, то создает его.
	 * Изменяет защищенное поле tabs_.
	 * \param table таблица, которую необходимо добавить во вкладку.
	 * \param tabname имя вкладки.
	 */
	void addTableToTab(QTableWidget *table, const QString& tabname);

	/*! \~russian
	 * \brief Получает из потока имя вкладки.
	 * \details Функция ничем не отличается от простого считывателя до конца строки.
	 * Входной поток уже не должен содержать ключевого слова \#matrixname или \#vectorsname.
	 * \param ss поток ввода, из которого происходит считывание.
	 * \return имя вкладки.
	 */
	QString getTabName(std::istream& ss) const;

	/*! \~russian
	 * \brief Выполняет сохранение файла из текстового редактора unmarkedValues_.
	 * \details Также удаляет ненужные переносы строки в конце файла.
	 * Используется только в режиме TextEditor.
	 * Является вспомогательной для слота save_file(const string&).
	 * \param fout выходной поток, куда происходит запись (сохранение).
	 */
	void saveFromEditor(std::ostream& fout) const;

	/*! \~russian
	 * \brief Выполняет сохранение файла из созданных объектов.
	 * \details Используется только в режиме Tabs. Вызывает функции printElem(ostream&) из массива printElements_.
	 * \param fout выходной поток, куда происходит запись (сохранение).
	 */
	void saveFromData(std::ostream& fout) const;

	/*! \~russian
	 * \brief Инициализация.
	 * \details Данная функция используется только один раз в конструкторе.
	 * Создает главный виджет mW_, менеджер размещения mL_, прикрепляет все к основному виджету прокрутки.
	 * Также задает некоторые флаги виджета прокрутки.
	 */
	void main_init();

	/*! \~russian
	 * \brief Функция для загрузки файла filename в данный виджет.
	 * \details В данной функции создается отдельный поток, который
	 * считывает всю информацию из файла в строковый буфер. В это время
	 * приложение продолжает свою работу, а на виджет блокнота накладывается
	 * виджет загрузки, чтобы показать, что приложение не зависло.
	 *
	 * Далее происходит вызов функции chooseWhatCreate().
	 * \param filename имя файла, который будет открыт и считан.
	 */
	void load_file(const std::string& filename);

	/*! \~russian
	 * \brief Проверка данных в строке на достаточное количество введенных значений для глобальных переменных с названиями в labels.
	 * \details Является вспомогательной функцией для check_correct().
	 * \param ss поток ввода, из которого считываются данные.
	 * \param labels метки, записанные в одну строку через разделители. Каждая метка соответствует переменной.
	 * \return true, если все введено корректно.
	 */
	bool checkGlobalValues(std::istream& ss, const std::string& labels) const;

	/*! \~russian
	 * \brief Проверка данных в строке на корректный ввод элементов векторов с количеством векторов из labels.
	 * \details Является вспомогательной функцией для check_correct().
	 * \param ss поток ввода, из которого считываются данные.
	 * \param labels метки, записанные в одну строку через разделители. Каждая метка соответствует своему вектору (строке).
	 * \return true, если все введено корректно.
	 */
	bool checkVector(std::istream& ss, const std::string& labels) const;

	/*! \~russian
	 * \brief Проверка данных в строке на корректный ввод элементов матрицы с количеством столбцов из labels.
	 * \details Является вспомогательной функцией для check_correct().
	 * \param ss поток ввода, из которого считываются данные.
	 * \param labels метки, записанные в одну строку через разделители. Каждая метка соответствует своему столбцу.
	 * \return true, если все введено корректно.
	 */
	bool checkTable(std::istream& ss, const std::string& labels) const;

	/*! \~russian
	 * \brief  Функция для проверки корректности всех строк файла с именем filename.
	 * \details Если данные введены некорректно, то режим переключается в mode::TextEditor .
	 * \param vss поток, из которого считываются данные.
	 * \return true, если файл размечен так, что из него можно сделать виджет с таблицами, вкладками.
	 */
	bool check_correct(std::istream& vss) const;

	/*! \~russian
	 * \brief Функция создает виджет с таблицами и полями из потока ввода.
	 * \details Данная функция вызывается уже после проверки файла функцией check_correct().
	 * Изменяет все параметры, кроме unmarkedValues_.
	 * \param fin поток ввода, из которого считывается информация.
	 */
	void createTabsAndGlobalValues(std::istream& fin);

	/*! \~russian
	 * \brief Функция создает текстовый редактор.
	 * \details Функция вызывается, если данные файла были введены некорректно или если выставлен режим TextEditor.
	 * Изменяет защищенное поле unmarkedValues_.
	 * \param vss
	 */
	void createTextEditor(std::stringstream& vss);

public:
	/*! \~russian
	 * \brief Конструктор с именем файла.
	 * \details  После основных инициализаций вызывает метод load_file().
	 * \param filename имя файла, из которого будет считана информация.
	 * \param pwg указатель на виджет-родитель.
	 * \param in_mode режим, под которым открыть файл.
	 */
	explicit FileToTabWidget(const std::string& filename, QWidget* pwg = nullptr, mode in_mode = mode::Tabs);

	//! \~russian \brief Деструктор. \details Удаляет элементы из массива printElements_.
	~FileToTabWidget() {
		for(size_t i=0; i<printElements_.size(); ++i)
			delete printElements_[i];
	}

	/*! \~russian
	 * \brief Функция для печати всех элементов данного виджета в поток. Используется для отладки.
	 * \details Печатает родителей и objectName() всех элементов, что есть в данном виджете.
	 * \param s поток вывода, куда печатается информация.
	 * \return поток вывода, куда была выведена вся информация.
	 */
	std::ostream& print(std::ostream& s) const;

	/*! \~russian
	 * \brief Функция читает защищенное поле режима работы виджета.
	 * \return режим работы виджета-блокнота.
	 */
	mode getMode() const {return mode_;}

	/*! \~russian
	 * \brief Возвращает главный виджет, на который все накладывается.
	 * \details Читает приватное поле mW_. mW_ может быть изменено.
	 * Используется, если необходимо поменять рамку, цвет рамки данного виджета и т.п.
	 * \return указатель на главный виджет для данного объекта.
	 */
	QFrame* get_mainFrame() {return mW_;}

	/*! \~russian
	 * \brief Возвращает главный виджет, на который все накладывается. Константная версия.
	 * \details Читает приватное поле mW_. mW_ не может быть изменено.
	 * \return указатель на главный виджет для данного объекта.
	 */
	const QFrame* get_mainFrame() const {return mW_;}

public slots:
	/*! \~russian
	 * \brief Слот сохраняет текущее состояние переменных в файл.
	 * \details Считывает все параметры виджета и записывает их. Сами параметры не меняются.
	 * Вывод зависит от режима работы редактора:
	 *
	 * если выбран mode::TextEditor, то текст
	 * будет записан в файл именно так, как его видит перед собой пользователь.
	 *
	 * если выбран mode::Tabs, то текст может отличаться от того, который ввел до этого
	 * пользователь, т.к. для матриц, векторов, скалярных величин есть специальные
	 * методы вывода. Гарантируется, что порядок элементов в выводе останется тем же,
	 * что и был до этого. Для вывода используются методы из класса PrintElement.
	 * \param filename имя файла, в который происходит вывод.
	 */
	void save_file(const std::string& filename) const;

private slots:
	/*! \~russian
	 * \brief Слот выбирает, что создавать.
	 * \details Используется после считывания файла в буфер fttwSS_.
	 * Далее создает виджет в зависимости от выбранного режима (mode_) и
	 * возможностей (если файл введен некорректно, то режим mode::Tabs недоступен).
	 *
	 * Слот соединяется с сигналом finishedMy() из класса FTTWFileBufferThread.
	 */
	void chooseWhatCreate();

};
/*! \~russian
 * \brief Оператор вывода для класса FileToTabWidget.
 * \details Вызывает внутри себя метод print(). Используется для отладки.
 * \param s поток вывода, куда выводится информация.
 * \param tw константная ссылка на редактор, из которого выводится информация.
 * \return поток вывода, куда была выведена информация.
 */
inline std::ostream& operator<<(std::ostream& s, const FileToTabWidget& tw){
	return tw.print(s);
}



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




/*! \~russian \brief Виджет с кнопками для редактора FileToTabWidget.
 *
 * Создает виджет, на котором присутствуют:
 *
 * 1. виджет класса FileToTabWidget
 *
 * 2. кнопка для сохранения данных
 *
 * 3. кнопка для попытки считывания файла ещё раз (кнопка перезагрузки).
 *
 * 4. Кнопка переключения режима.
 *
 * Схема использования:
 * \code
 *
 * 	QApplication a(argc, argv);
 *
 *	std::string filename = "/home/moses/Documents/Qt_workspace/FileToWidget/FileToWidget/infiles/infile.in";
 *	fttw::OutSideWidget mW(filename);
 *	mW.show();
 *
 *	return a.exec();
 *
 * \endcode
 *
 * Если необходимо, чтобы имя файла загрузки и имя файла сохранения отличались, то можно сделать так:
 * \code
 *
 *	std::string saveFilename = "/home/moses/Documents/Qt_workspace/FileToWidget/FileToWidget/infiles/outfile.out";
 *  std::string loadFilename = "/home/moses/Documents/Qt_workspace/FileToWidget/FileToWidget/infiles/infile.in";
 *  fttw::OutSideWidget mW(saveFilename, loadFilename);
 *	mW.show();
 *
 * \endcode
 *
 */
class OutSideWidget : public QFrame{
	Q_OBJECT
private:
	/*! \~russian
	 * \brief Указатель на виджет-блокнот.
	 * \details Виджет, который позволяет преобразовать файл в таблицы и окошки для глобальных параметров.
	 * В случае некорректной разметки просто выдает текстовый редактор с введенными данными.
	 */
	FileToTabWidget *fttw_;
	//! \~russian \brief Менеджер размещения данного виджета.
	QVBoxLayout *mL_;
	//! \~russian \brief Кнопка, нажав на которую можно сохранить все введенные данные в файл savename_. \details Связывает сигнал нажатия с вызовом слота savedata_slot().
	QPushButton *saveButton_;
	//! \~russian \brief Кнопка, нажав на которую можно перезагрузить данные из файла. \details Пересоздает виджет fttw_, считывая файл заново. Соединена со слотом reload_slot().
	QPushButton *reloadButton_;
	/*! \~russian
	 * \brief Кнопка переключения просмотра из режима вкладок и таблиц в режим текстового редактора.
	 * \details Появляется только при режиме таблиц и вкладок. Сохраняет состояние в savename_.
	 * Удаляет текущий виджет и загружает заново в режиме текстового редактора.
	 * Связана со слотом toTextEditor().
	 */
	QPushButton *toTextEditorButton_;
	/*! \~russian
	 * \brief Кнопка переключения просмотра из текстового редактора в виджет с таблицами и вкладками.
	 * \details Появляется только в режиме текстового редактора. Сохраняет состояние в savename_.
	 * Удаляет текущий виджет и пытается загрузить заново в режиме таблиц и вкладок.
	 * В случае неудачи оставляет режим текстового редактора.
	 * Связана со слотом toTabsAndTables().
	 */
	QPushButton *toTabsAndTablesButton_;
	//! \~russian \brief Имя файла, в который будут сохраняться данные из виджета fttw_.
	std::string savename_;
	//! \~russian \brief Имя файла, откуда будут загружаться данные для виджета fttw_.
	std::string loadname_;

	/*! \~russian
	 * \brief Функция инициализации.
	 * \details Вызывается один раз в конструкторе. Затрагивает все параметры.
	 * Создает виджет со всеми кнопками, активирует связь между слотами и сигналами.
	 */
	void init();

	/*! \~russian
	 * \brief Функция скрывает ненужные кнопки и открывает нужные.
	 * \details При режиме текстового редактора скрывает кнопку toTextEditorButton_,
	 * но показывает кнопку toTabsAndTablesButton_.
	 * При режиме вкладок и таблиц действует наоборот.
	 */
	void hideSomeButtons();

public:
	/*! \~russian
	 * \brief Конструктор. Задает имя файлов для загрузки и для сохранения одинаковыми и равными name.
	 * \details Вызывает функцию инициализации init().
	 * \param name имя файла, который загружается в редактор и куда будут сохранятся изменения, внесенные пользователем.
	 * \param pwg указатель на виджет-родитель.
	 */
	explicit OutSideWidget(const std::string& name, QWidget* pwg = nullptr);

	/*! \~russian
	 * \brief Конструктор. Позволяет задать различные файлы для загрузки и сохранения.
	 * \details Вызывает функцию инициализации init().
	 * Если имена savename и loadname не совпадают, то файл с именем loadname останется неизменным.
	 * \param savename имя файла, куда будут сохранятся измененные данные.
	 * \param loadname имя файла, из которого происходит загрузка данных.
	 * \param pwg указатель на виджет-родитель.
	 */
	OutSideWidget(const std::string& savename, const std::string& loadname, QWidget *pwg = nullptr);

	/*! \~russian
	 * \brief Функция возвращает виджет-редактор.
	 * \details Читает приватное поле fttw_. fttw_ может быть изменено.
	 * Данная функция необходима, если программист хочет изменить как-то стиль оформления редактора.
	 * \return указатель на виджет-редактор.
	 */
	FileToTabWidget* get_FileToTabWidget() {return fttw_;}
	/*! \~russian
	 * \brief Функция возвращает виджет-редактор. Константная версия.
	 * \details Читает приватное поле fttw_. fttw_ не может быть изменено.
	 * \return указатель на константный виджет-редактор.
	 */
	const FileToTabWidget* get_FileToTabWidget() const {return fttw_;}

	/*! \~russian
	 * \brief Возвращает кнопку сохранения информации из редактора.
	 * \details Читает приватное поле saveButton_. saveButton_ может быть изменено.
	 * Данная функция необходима, если программист хочет изменить как-то стиль оформления кнопки.
	 * \return указатель на кнопку сохранения информации из редактора.
	 */
	QPushButton* get_saveButton() {return saveButton_;}
	/*! \~russian
	 * \brief Возвращает кнопку сохранения информации из редактора. Константная версия.
	 * \details Читает приватное поле saveButton_. saveButton_ не может быть изменено.
	 * \return указатель на константную кнопку сохранения.
	 */
	const QPushButton* get_saveButton() const {return saveButton_;}

	/*! \~russian
	 * \brief Возвращает кнопку перезагрузки данных из файла.
	 * \details Читает приватное поле reloadButton_. reloadButton_ может быть изменено.
	 * Данная функция необходима, если программист хочет изменить как-то стиль оформления кнопки.
	 * \return указатель на кнопку перезагрузки данных из файла.
	 */
	QPushButton* get_reloadButton() {return reloadButton_;}
	/*! \~russian
	 * \brief Возвращает кнопку перезагрузки данных из файла. Константная версия.
	 * \details Читает приватное поле reloadButton_. reloadButton_ не может быть изменено.
	 * \return указатель на константную кнопку перезагрузки данных из файла.
	 */
	const QPushButton* get_reloadButton() const {return reloadButton_;}

	/*! \~russian
	 * \brief Возвращает кнопку переключения просмотра из режима вкладок и таблиц в режим текстового редактора.
	 * \details Читает приватное поле toTextEditorButton_. toTextEditorButton_ может быть изменено.
	 * Данная функция необходима, если программист хочет изменить как-то стиль оформления кнопки.
	 * \return указатель на кнопку переключения просмотра в режим текстового редактора.
	 */
	QPushButton* get_toTextEditorButton() {return toTextEditorButton_;}
	/*! \~russian
	 * \brief Возвращает кнопку переключения просмотра из режима вкладок и таблиц в режим текстового редактора. Константная версия.
	 * \details Читает приватное поле toTextEditorButton_. toTextEditorButton_ не может быть изменено.
	 * \return указатель на константную кнопку переключения просмотра в режим текстового редактора.
	 */
	const QPushButton* get_toTextEditorButton() const {return toTextEditorButton_;}

	/*! \~russian
	 * \brief Возвращает кнопку переключения просмотра из режима текстового редактора в режим вкладок и таблиц.
	 * \details Читает приватное поле toTabsAndTablesButton_. toTabsAndTablesButton_ может быть изменено.
	 * Данная функция необходима, если программист хочет изменить как-то стиль оформления кнопки.
	 * \return указатель на кнопку переключения просмотра в режим таблиц и вкладок.
	 */
	QPushButton* get_toTabsAndTablesButton() { return toTabsAndTablesButton_; }
	/*! \~russian
	 * \brief Возвращает кнопку переключения просмотра из режима текстового редактора в режим вкладок и таблиц. Константная версия.
	 * \details Читает приватное поле toTabsAndTablesButton_. toTabsAndTablesButton_ не может быть изменено.
	 * \return указатель на константную кнопку переключения просмотра в режим таблиц и вкладок.
	 */
	const QPushButton* get_toTabsAndTablesButton() const { return toTabsAndTablesButton_; }

	/*! \~russian
	 * \brief Возвращает имя файла для сохранения данных.
	 * \details Читает приватное поле savename_.
	 * \return ссылку на имя файла для сохранения данных.
	 */
	const std::string& get_saveName() const {return savename_;}

	/*! \~russian
	 * \brief Устанавливает имя файла для сохранения данных.
	 * \details Изменяет приватное поле savename_.
	 * \param savename новое имя файла для сохранения данных.
	 */
	void set_saveName(const std::string& savename) {savename_ = savename;}

	/*! \~russian
	 * \brief Возвращает имя файла, из которого загружаются данные.
	 * \details Читает приватное поле loadname_.
	 * \return ссылку на имя файла, из которого загружаются данные.
	 */
	const std::string& get_loadName() const {return loadname_;}

	/*! \~russian
	 * \brief Устанавливает имя файла, из которого загружаются данные.
	 * \details Изменяет приватное поле loadname_.
	 * \param loadName новое имя файла для загрузки данных.
	 */
	void set_loadName(const std::string& loadName) {loadname_ = loadName;}

public slots:
	/*! \~russian
	 * \brief Слот, который вызывает сигнал savedata(const string&), чтобы можно было вызвать другой слот из класса FileToTabWidget.
	 * Связан с кнопкой saveButton_.
	 */
	void savedata_slot();

	/*! \~russian
	 * \brief Слот для перезагрузки данных из файла.
	 * \details Слот удаляет виджет fttw_ и создает его заново из файла с именем loadname_.
	 * Режим открытия оставляет тем, который был.
	 * Связан с кнопкой reloadButton_.
	 * \warning НЕ СОХРАНЯЕТ ТЕКУЩЕЕ СОСТОЯНИЕ.
	 */
	void reload_slot();

	/*! \~russian
	 * \brief Слот смены режима на текстовый редактор.
	 * \details Слот удаляет виджет fttw_ и создает его заново из файла с именем loadname_ в режиме текстового редактора.
	 * Сохраняет состояние перед преобразованием.
	 * Связан с кнопкой toTextEditorButton_.
	 */
	void toTextEditor();
	/*! \~russian
	 * \brief Слот смены режима на таблицы и вкладки в редакторе.
	 * \details Удаляет виджет fttw_ и создает его заново из файла с именем loadname_ в режиме таблиц и вкладок, по возможности.
	 * Сохраняет состояние перед преобразованием.
	 * Связан с кнопкой toTabsAndTablesButton_.
	 */
	void toTabsAndTables();

signals:
	//! \~russian \brief Сигнал, который позволяет вызвать слот из класса FileToTabWidget.
	void savedata(const std::string& name);
};

}

#endif // FILETOTABWIDGET16012017GASPARYANMOSES_H


/*@}*/
