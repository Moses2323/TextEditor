/** @addtogroup NNTools
 * @{*/

/** @file */

#ifndef PRINTELEMENT13022017GASPARYANMOSES_H
#define PRINTELEMENT13022017GASPARYANMOSES_H

#include <iostream>

#include<QTableWidget>
#include<QString>
#include<QLabel>
#include<QLineEdit>

namespace fttw {


/*! \~russian \brief Класс для печати элемента в стиле оформления FileToTabWidget
 *
 * Является классом, от которого наследуются другие классы.
 * Объект данного класса может быть создан только внутри объектов,
 * классы которых отнаследованы от данного.
 *
 * Также класс записывает id каждого созданного элемента (начиная с 0).
 *
 * Имеет единственный виртуальный метод для вывода элемента
 * в стиле разметки из класса FileToTabWidget.
 *
 * Схема использования:
 * \code
 *
 * fttw::printElem *pe = new fttw::CommentElement("#comment");
 *
 * //распечатаем содержимое комментария в поток вывода
 * pe->printElem(std::cout);
 * //выведет "#comment" + '\n'
 *
 * delete pe;
 *
 * \endcode
 *
 * \warning Классы, отнаследованные от PrintElement,
 * не несут ответственности за элементы, хранящиеся в них в качестве указателя.
 * Пользователь должен сам удалить элемент или предоставить это механизму Qt.
 *
 */
class PrintElement{
	//! \~russian \brief Статическая переменная для выдачи разных id для элементов данного класса. Выдает начиная с 0.
	static size_t nextSerialNum;
	//! \~russian \brief Идентификационный номер (id) данного элемента.
	const size_t serialNum_;

protected:
	//! \~russian \brief Конструктор. Устанавливает идентификационный номер элемента. \details Изменяет статическую переменную nextSerialNum, защищенное поле serialNum_.
	PrintElement() : serialNum_(nextSerialNum++) {}

public:
	//! \~russian \brief Запрет копирования элемента данного класса через конструктор копирования.
	PrintElement(const PrintElement&) = delete;
	//! \~russian \brief Запрет копирования элемента данного класса через оператор присваивания.
	PrintElement& operator=(const PrintElement&) = delete;
	//! \~russian \brief Запрет копирования элемента данного класса через перемещающий оператор присваивания.
	PrintElement& operator=(PrintElement&&) = delete;

	//! \~russian \brief Виртуальный деструктор.
	virtual ~PrintElement() {}

	/*! \~russian
	 * \brief Функция читает приватное поле идентификатора элемента.
	 * \details Читает приватное поле serialNum_.
	 * \return Константная ссылка на идентификационный номер данного элемента.
	 */
	const size_t& getSerialNum() const {return serialNum_;}

	/*! \~russian
	 * \brief Абстрактная функция для печати данного элемента в стиле разметки из fttw::FileToTabWidget.
	 * \param s поток вывода, куда печатается элемент.
	 * \return поток вывода, куда были выведены данные.
	 */
	virtual std::ostream& printElem(std::ostream& s) const =0;

};



/*! \~russian \brief Класс для печати Таблицы в стиле оформления FileToTabWidget.
 *
 * Наследуется от класса fttw::PrintElement.
 *
 * Позволяет печатать таблицу в стиле оформления файла, который подается
 * в fttw::FileToTabWidget.
 *
 * Хранит в себе имя таблицы и указатель на таблицу.
 * Выдает таблицу в стиле:
 *
 * 1. Если нет имени:
 *
 * \#matrix Va Vb Vc
 *
 * 1 2 3
 *
 * 4 5 6
 *
 * 7 8 9
 *
 *
 * 2. Если есть имя:
 *
 * \#matrixname TableName
 *
 * \#matrix Va Vb Vc
 *
 * 1 2 3
 *
 * 4 5 6
 *
 * 7 8 9
 *
 * Тогда перед пользователем будет выведена таблица:
 *
 * |   | Va | Vb | Vc |
 * |---|----|----|----|
 * | 1 | 1  | 2  | 3  |
 * | 2 | 4  | 5  | 6  |
 * | 3 | 7  | 8  | 9  |
 *
 * Если имя указано, то название вкладки будет соответствовать имени.
 *
 * Схема использования:
 * \code
 *
 * QTableWidget *table = new QTableWidget;
 * //настройка таблицы, создание элементов таблицы
 * //...
 *
 * fttw::TablePrintElem *tpe = new fttw::TablePrintElem;
 * tpe -> setTable(table);
 *
 * //если есть имя:
 * tpe -> setName("TableName");
 *
 * //печать, например, в стандартный поток вывода
 * tpe -> printElem();
 *
 * delete tpe; // в деструкторе
 *
 * \endcode
 *
 */
class TablePrintElem : public PrintElement{
	//! \~russian \brief Указатель на таблицу, элементы и названия столбцов которой будут печататься.
	QTableWidget* table_;
	//! \~russian \brief Название таблицы. Если его нет, то поле равно "". \details Используется в редакторе в качестве названия вкладки с таблицей.
	QString name_;

public:
	//! \~russian \brief Имя для распознавания таблицы. Например, "#matrix" . \details Используется внутри редактора, чтобы распознавать таблицу.
	static const QString trigger;
	//! \~russian \brief Имя для распознавания строки с именем файла, после которой сразу должна идти строка с trigger. Например, "#matrixname". \details Используется внутри редактора, чтобы распознавать имя таблицы.
	static const QString triggerName;

	//! \~russian \brief Пустой конструктор. Все поля должны быть заданы методами set.
	TablePrintElem() : PrintElement(), table_(nullptr), name_("") {}

	/*! \~russian
	 * \brief Конструктор с указателем на таблицу.
	 * \param table указатель на таблицу, элементы и названия столбцов которой будут печататься.
	 */
	explicit TablePrintElem(QTableWidget* table) : PrintElement(), table_(table), name_("") {}

	/*! \~russian
	 * \brief Конструктор с указателем на таблицу и именем.
	 * \param table указатель на таблицу, элементы и названия столбцов которой будут печататься.
	 * \param name название таблицы.
	 */
	TablePrintElem(QTableWidget* table, const QString& name) : PrintElement(), table_(table), name_(name) {}
	//! \~russian \brief Деструктор.
	virtual ~TablePrintElem() {}

	/*! \~russian
	 * \brief Функция задает указатель на таблицу.
	 * \details Изменяет защищенное поле table_.
	 * \param table новый указатель на таблицу, элементы и названия столбцов которой будут печататься.
	 */
	void setTable(QTableWidget* table) { table_ = table; }

	/*! \~russian
	 * \brief Функция возвращает указатель на таблицу.
	 * \details Читает защищенное поле table_. Таблицу можно менять.
	 * \return указатель на таблицу, элементы и названия столбцов которой будут печататься.
	 */
	QTableWidget* getTable() {return table_;}

	/*! \~russian
	 * \brief Функция возвращает указатель на таблицу. Константная версия.
	 * \details Читает защищенное поле table_. Таблицу менять нельзя.
	 * \return Указатель на константную таблицу.
	 */
	const QTableWidget* getTable() const {return table_;}

	/*! \~russian
	 * \brief Функция задает имя таблице.
	 * \details Изменяет защищенное поле name_.
	 * \param str новое имя таблицы.
	 */
	void setName(const QString& str) {name_ = str;}

	/*! \~russian
	 * \brief Функция возвращает имя таблицы.
	 * \details Читает защищенное поле name_. Имя можно менять.
	 * \return Ссылка на имя таблицы.
	 */
	QString& getName() {return name_;}

	/*! \~russian
	 * \brief Функция возвращает имя таблицы.
	 * \details Читает защищенное поле name_. Имя менять нельзя.
	 * \return Константная ссылка на имя таблицы.
	 */
	const QString& getName() const {return name_;}

	/*! \~russian
	 * \brief Печатает таблицу в стиле файла для fttw::FileToTabWidget (см. описание класса fttw::TablePrintElem).
	 * \param s поток вывода, куда печатается данный элемент.
	 * \return поток вывода, куда была выведена информация.
	 */
	virtual std::ostream& printElem(std::ostream &s) const override;
};

/*! \~russian \brief Класс для печати векторов в стиле оформления FileToTabWidget.
 *
 * Наследуется от класса fttw::PrintElement .
 *
 * Позволяет печатать вектора в стиле оформления файла, который подается
 * в fttw::FileToTabWidget .
 *
 * Хранит в себе имя таблицы и указатель на виджет таблицы.
 * fttw::FileToTabWidget выдает перед пользователем таблицу в стиле:
 *
 * 1. если в файле содержится
 *
 * \#vectors V1 V2
 *
 * 10 20
 *
 * 30 40
 *
 * 50 60
 *
 * То перед пользователем будет выведена таблица:
 *
 * |    | 1  | 2  | 3  |
 * |----|----|----|----|
 * | V1 | 10 | 20 | 30 |
 * | V2 | 40 | 50 | 60 |
 *
 * С именем вкладки равным порядковому номеру таблицы.
 *
 * 2. если в файле содержится
 *
 * \#vectorsname MyVec
 *
 * \#vectors V1 V2
 *
 * 10 20
 *
 * 30 40
 *
 * 50 60
 *
 * То перед пользователем будет выведена таблица:
 *
 * |    | 1  | 2  | 3  |
 * |----|----|----|----|
 * | V1 | 10 | 20 | 30 |
 * | V2 | 40 | 50 | 60 |
 *
 * С именем вкладки MyVec.
 *
 * Данный класс лишь делает обратную работу:
 * печатает таблицу с данными в выходной поток так, чтобы
 * затем fttw::FileToTabWidget смог прочесть данные как будто
 * это вектора, которые можно вывести перед пользователем
 * как показано выше.
 *
 * Схема использования:
 * \code
 *
 * QTableWidget *table = new QTableWidget;
 * //настройка таблицы, создание элементов таблицы
 * //...
 *
 * fttw::VectorsPrintElem *vpe = new fttw::VectorsPrintElem;
 * vpe -> setVectors(table);
 *
 * //если есть имя:
 * vpe -> setName("TableName");
 *
 * //печать, например, в стандартный поток вывода
 * vpe -> printElem();
 *
 * delete vpe; // в деструкторе
 *
 * \endcode
 *
 */
class VectorsPrintElem : public PrintElement{
	//! \~russian \brief Указатель на таблицу, элементы и названия строк которой будут печататься.
	QTableWidget* vectors_{nullptr};
	//! \~russian \brief Имя вкладки с векторами.
	QString name_ {""};

public:
	//! \~russian \brief Имя для распознавания векторов. Например, "#vectors". \details Используется в редакторе для распознавания названия векторов.
	static const QString trigger;
	//! \~russian \brief Имя для распознавания строки с именем вкладки данных векторов. Например, "#vectorsname". \details Используется в редакторе для распознавания имени вектора.
	static const QString triggername;

	//! \~russian \brief Пустой конструктор.
	VectorsPrintElem() : PrintElement() {}

	/*! \~russian
	 * \brief Конструктор, в котором сразу задается таблица.
	 * \param vectors указатель на таблицу, элементы и названия строк которой будут печататься.
	 */
	explicit VectorsPrintElem(QTableWidget* vectors) : PrintElement(), vectors_(vectors) {}

	/*! \~russian
	 * \brief Конструктор, в котором сразу задается таблица и название вкладки.
	 * \param vectors указатель на таблицу, элементы и названия строк которой будут печататься.
	 * \param name имя вкладки с векторами.
	 */
	VectorsPrintElem(QTableWidget* vectors, const QString& name) : PrintElement(), vectors_(vectors), name_(name) {}
	//! \~russian \brief Деструктор.
	~VectorsPrintElem() {}

	/*! \~russian
	 * \brief Функция задает указатель на таблицу.
	 * \details Изменяет приватное поле vectors_.
	 * \param vectors новый указатель на таблицу, элементы и строки которой будут печататься.
	 */
	void setVectors(QTableWidget* vectors) { vectors_ = vectors; }

	/*! \~russian
	 * \brief Функция возвращает указатель на таблицу.
	 * \details Читает приватное поле vectors_. Таблицу можно менять.
	 * \return указатель на таблицу, элементы которой будут печататься.
	 */
	QTableWidget* getVectors() { return vectors_; }

	/*! \~russian
	 * \brief Функция возвращает указатель на таблицу. Константная версия.
	 * \details Читает приватное поле vectors_. Таблицу нельзя менять.
	 * \return указатель на константную таблицу, элементы которой будут печататься.
	 */
	const QTableWidget* getVectors() const { return vectors_; }

	/*! \~russian
	 * \brief Функция задает имя вкладки с данными векторами.
	 * \details Изменяет приватное поле name_.
	 * \param name новое имя для векторов.
	 */
	void setName( const QString& name ) { name_ = name; }

	/*! \~russian
	 * \brief Функция возвращает имя вкладки с данными векторами.
	 * \details Читает приватное поле name_.
	 * \return Константная ссылка на имя вкладки с векторами.
	 */
	const QString& getName() const { return name_; }

	/*! \~russian
	 * \brief Печатает вектора в стиле файла для fttw::FileToTabWidget (см. описание класса fttw::VectorsPrintElem).
	 * \param s поток вывода, куда печатается данный элемент.
	 * \return поток вывода, куда был напечатан данный элемент.
	 */
	virtual std::ostream& printElem(std::ostream &s) const override;
};

/*! \~russian \brief Класс для печати вектора в стиле оформления FileToTabWidget.
 *
 * Наследуется от класса fttw::PrintElement.
 *
 * Позволяет печатать вектор в стиле оформления файла, который подается
 * в fttw::FileToTabWidget.
 *
 * Хранит в себе имя таблицы и указатель на виджет таблицы. Для данного
 * класса имя вкладки создается автоматически из названия первой строки
 * таблицы.
 * fttw::FileToTabWidget выдает перед пользователем таблицу в стиле:
 *
 * если в файле содержится:
 *
 * \#vector V1
 *
 * 1 2 3 4 5 6
 *
 * 7 8
 *
 * 9 20 33
 *
 * То будет создана таблица вида:
 *
 * |    | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 |
 * |----|---|---|---|---|---|---|---|---|---|----|----|
 * | V1 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 20 | 33 |
 *
 * С именем вкладки V1.
 *
 * Данный класс лишь делает обратную работу:
 * печатает таблицу с данными в выходной поток так, чтобы
 * затем fttw::FileToTabWidget смог прочесть данные как будто
 * это вектор, который можно вывести перед пользователем
 * как показано выше.
 *
 * Схема использования:
 * \code
 *
 * QTableWidget *table = new QTableWidget;
 * //настройка таблицы, создание элементов таблицы лишь с одной строкой.
 * //...
 *
 * fttw::VectorPrintElem *vpe = new fttw::VectorPrintElem;
 * vpe -> setVector(table);
 *
 * //печать, например, в стандартный поток вывода
 * vpe -> printElem();
 *
 * delete vpe; // в деструкторе
 *
 * \endcode
 *
 */
class VectorPrintElem : public PrintElement{
	//! \~russian \brief Имя вкладки с вектором. Создается из названия первой строки таблицы. \details Используется в fttw::FileToTabWidget, чтобы обозначить имя вкладки с таблицей.
	QString name_{""};
	//! \~russian \brief Указатель на таблицу, содержащую элементы для вывода. \details Таблица должна содержать только одну строчку, иначе вывод будет некорректен.
	QTableWidget* vector_{nullptr};

	//! \~russian \brief Функция устанавливает имя вкладки с вектором. \details Создает имя из названия первой строки таблицы. Изменяет приватное поле name_.
	void setName() {name_ = vector_->verticalHeaderItem(0)->text();}

public:
	//! \~russian \brief Имя для распознавания вектора. Например, "#vector". \details Используется в редакторе fttw::FileToTabWidget, чтобы найти вектор.
	static const QString trigger;

	//! \~russian \brief Пустой конструктор.
	VectorPrintElem() {}

	/*! \~russian
	 * \brief Конструктор сразу вместе с указателем на таблицу.
	 * \param vec указатель на таблицу, содержащую элементы для вывода.
	 */
	explicit VectorPrintElem(QTableWidget* vec) : vector_(vec) {setName();}
	//! \~russian \brief Деструктор.
	virtual ~VectorPrintElem() {}

	/*! \~russian
	 * \brief Функция устанавливает указатель на таблицу.
	 * \details Изменяет приватные поля vector_ и name_.
	 * \param vec новый указатель на таблицу, содержащую элементы для вывода.
	 */
	void setVector(QTableWidget* vec) {vector_ = vec; setName();}

	/*! \~russian
	 * \brief Функция возвращает указатель на таблицу.
	 * \details Читает приватное поле vector_.
	 * \return указатель на константную таблицу, содержащую элементы для вывода.
	 */
	const QTableWidget* getVector() const { return vector_; }

	/*! \~russian
	 * \brief Функция возвращает имя вкладки с вектором.
	 * \details Читает приватное поле name_.
	 * \return константная ссылка на имя вкладки с вектором.
	 */
	const QString& getName() const { return name_; }

	/*! \~russian
	 * \brief Печатает вектор в стиле файла для fttw::FileToTabWidget (см. описание класса fttw::VectorPrintElem).
	 * \param s поток вывода, куда печатается данный элемент.
	 * \return ссылку на поток вывода, куда был распечатан данный элемент.
	 */
	virtual std::ostream& printElem(std::ostream &s) const override;

};

/*! \~russian \brief Класс для печати глобальной переменной в стиле оформления FileToTabWidget.
 *
 * Наследуется от класса fttw::PrintElement.
 *
 * Позволяет печатать название и значение глобальной переменной в стиле
 * файла, который подается в fttw::FileToTabWidget.
 *
 * Хранит в себе указатель на метку с названием глобальной переменной,
 * текстовое поле со значением переменной.
 *
 * Выводит глобальную переменную в виде:
 *
 * \#scalar Name
 *
 * Value
 *
 *
 * Т.е.:
 *
 * \#scalar T
 *
 * 1000
 *
 * Схема использования:
 * \code
 *
 * //создаем метку и поле для редактирования со значением
 * QLabel *label = new QLabel("T");
 * QLineEdit *le = new QLineEdit("1000");
 *
 * //создаем элемент данного класса
 * fttw::GlobalValuePrintElem glv = new fttw::GlobalValuePrintElem;
 *
 * glv -> setLabel(label);
 * glv -> setLineEditor(le);
 *
 * //вывод, например, в стандартный поток вывода
 * glv -> printElem(std::cout);
 *
 * delete glv;
 * delete label;
 * delete le;
 *
 * \endcode
 *
 */
class GlobalValuePrintElem : public PrintElement{
	//! \~russian \brief Метка с названием глобальной переменной.
	QLabel* label_;

	/*! \~russian
	 * \brief Текстовый редактор для значения переменной.
	 * \details Данный QLineEdit в редакторе fttw::FileToTabWidget содержит ограничение на вводимые параметры:
	 * вводить можно только вещественное число, вещественное число должно изменяться в пределах числа с двойной
	 * точностью (double).
	 */
	QLineEdit* editor_;

public:
	//! \~russian \brief Имя для распознавания глобальной переменной. Например, "#scalar". \details Используется редактором, чтобы находить глобальную переменную.
	static const QString trigger;

	//! \~russian \brief Пустой конструктор. \details Задает поля nullptr-ами. Все поля должны быть заданы позже функциями set.
	GlobalValuePrintElem() : PrintElement(), label_(nullptr), editor_(nullptr) {}
	/*! \~russian
	 * \brief Конструктор с параметрами.
	 * \param label метка с названием глобальной переменной.
	 * \param lineedit текстовый редактор для значения переменной.
	 */
	GlobalValuePrintElem(QLabel* label, QLineEdit* lineedit) : PrintElement(), label_(label), editor_(lineedit) {}
	//! \~russian \brief Деструктор.
	virtual ~GlobalValuePrintElem() {}

	/*! \~russian
	 * \brief Функция задает метку с названием переменной.
	 * \details Изменяет защищенное поле label_.
	 * \param label указатель на новую метку с названием глобальной переменной.
	 */
	void setLabel(QLabel* label) {label_ = label;}

	/*! \~russian
	 * \brief Функция возвращает метку с названием переменной.
	 * \details Читает защищенное поле label_. Метка может быть изменена.
	 * \return Указатель на метку с названием глобальной переменной.
	 */
	QLabel* getLabel() {return label_;}

	/*! \~russian
	 * \brief Функция возвращает метку с названием переменной. Константная версия.
	 * \details Читает защищенное поле label_. Метка не может быть изменена.
	 * \return Указатель на константную метку с названием глобальной переменной.
	 */
	const QLabel* getLabel() const {return label_;}

	/*! \~russian
	 * \brief Функция задает текстовый редактор для значения переменной.
	 * \details Изменяет защищенное поле editor_.
	 * \param lineedit указатель на новый текстовый редактор для значения переменной.
	 */
	void setLineEditor(QLineEdit* lineedit) {editor_ = lineedit;}

	/*! \~russian
	 * \brief Функция возвращает указатель на текстовый редактор со значением переменной.
	 * \details Читает защищенное поле editor_. Текстовый редактор может быть изменен.
	 * \return Указатель на текстовый редактор со значением переменной.
	 */
	QLineEdit* getLineEditor() {return editor_;}

	/*! \~russian
	 * \brief Функция возвращает указатель на текстовый редактор со значением переменной.
	 * \details Читает защищенное поле editor_. Текстовый редактор не может быть изменен.
	 * \return Указатель на константный текстовый редактор со значением переменной.
	 */
	const QLineEdit* getLineEditor() const {return editor_;}

	/*! \~russian
	 * \brief Печатает глобальную переменную в стиле файла для fttw::FileToTabWidget (см. описание класса fttw::GlobalValuePrintElem).
	 * \param s поток вывода, куда печатается данный элемент.
	 * \return поток вывода, куда был напечатан данный элемент.
	 */
	virtual std::ostream& printElem(std::ostream& s) const override;

};

/*! \~russian \brief Класс для печати глобальных переменных в стиле оформления FileToTabWidget.
 *
 * Наследуется от класса fttw::PrintElement.
 *
 * В отличие от класса fttw::GlobalValuePrintElem, позволяет печатать сразу несколько
 * глобальных переменных.
 * Данный класс применяется тогда, когда в .in-файле в одной строке задано сразу
 * несколько переменных.
 *
 * Хранит в себе массив меток с названиями переменных и массив меток с текстовыми
 * редакторами, которые содержат значения переменных.
 * Переменной с именем в labels_[i] соответствует текстовый редактор со значением
 * в editors_[i].
 *
 * Выводит глобальные переменные в виде:
 *
 * \#scalar NameOfValue1 NameOfValue2 NameOfValue3
 *
 * Value1 Value2 Value3
 *
 *
 * пример:
 *
 * \#scalar T dt A
 *
 * 1000 0.01 12321.323
 *
 *
 * Схема использования:
 * \code
 *
 * //создаем метки и поля для редактирования со значением
 * QLabel *label1 = new QLabel("T");
 * QLineEdit *le1 = new QLineEdit("1000");
 *
 * QLabel *label2 = new QLabel("dt");
 * QLineEdit *le2 = new QLineEdit("0.01");
 *
 * QLabel *label3 = new QLabel("A");
 * QLineEdit *le3 = new QLineEdit("12321.323");
 *
 * //создаем элемент данного класса
 * fttw::GlobalValuesPrintElem glv = new fttw::GlobalValuesPrintElem;
 *
 * glv -> addLabel(label1);
 * glv -> addEditor(le1);
 *
 * glv -> addLabel(label2);
 * glv -> addEditor(le2);
 *
 * glv -> addLabel(label3);
 * glv -> addEditor(le3);
 *
 * //вывод, например, в стандартный поток вывода
 * glv -> printElem(std::cout);
 *
 * delete glv;
 *
 * delete label1;
 * delete le1;
 *
 * delete label2;
 * delete le2;
 *
 * delete label3;
 * delete le3;
 *
 * \endcode
 *
 */
class GlobalValuesPrintElem : public PrintElement{
	//! \~russian \brief Массив, содержащий указатели на метки с названиями переменных.
	std::vector<QLabel*> labels_;
	//! \~russian \brief Массив, содержащий указатели на текстовые редакторы со значениями переменных.
	std::vector<QLineEdit*> editors_;

public:
	//! \~russian \brief Имя для распознавания глобальных переменных. Например, "#scalar". \details Используется редактором fttw::FileToTabWidget, чтобы находить глобальные переменные.
	static const QString trigger;

	//! \~russian \brief Пустой конструктор. \details Все элементы должны быть добавлены с помощью функций addLabel() и addEditor().
	GlobalValuesPrintElem() : PrintElement() {}
	//! \~russian \brief Деструктор.
	virtual ~GlobalValuesPrintElem() {}

	/*! \~russian
	 * \brief Функция добавляет метку с названием переменной в конец массива указателей на метки.
	 * \details Изменяет защищенное поле labels_ (добавляет элемент в конец).
	 * \param label указатель на новую метку.
	 */
	void addLabel(QLabel* label) {labels_.push_back(label);}

	/*! \~russian
	 * \brief Функция возвращает метку с названием переменной из массива по индексу ind.
	 * \details Читает элемент защищенного поля labels_. Элемент может быть изменен.
	 * \param ind индекс метки в массиве.
	 * \return Указатель на метку с названием переменной.
	 */
	QLabel* getLabel(size_t ind) {return labels_[ind];}

	/*! \~russian
	 * \brief Функция возвращает метку с названием переменной из массива по индексу ind. Константная версия.
	 * \details Читает элемент защищенного поля labels_. Элемент не может быть изменен.
	 * \param ind индекс элемента в массиве.
	 * \return указатель на константную метку с названием переменной.
	 */
	const QLabel* getLabel(size_t ind) const {return labels_[ind];}

	/*! \~russian
	 * \brief Функция добавляет текстовый редактор со значением переменной в конец массива указателей на текстовые редакторы со значениями переменных.
	 * \details Изменяет защищенное поле editors_ (добавляет элемент в конец).
	 * \param editor указатель на текстовый редактор, который добавляется в конец массива.
	 */
	void addEditor(QLineEdit* editor) {editors_.push_back(editor);}

	/*! \~russian
	 * \brief Функция возвращает текстовый редактор со значением переменной из массива по индексу ind.
	 * \details Читает элемент защищенного поля editors_. Элемент может быть изменен.
	 * \param ind индекс элемента в массиве.
	 * \return указатель на текстовый редактор.
	 */
	QLineEdit* getEditor(size_t ind) {return editors_[ind];}

	/*! \~russian
	 * \brief Функция возвращает текстовый редактор со значением переменной из массива по индексу ind. Константная версия.
	 * \details Читает элемент защищенного поля editors_. Элемент не может быть изменен.
	 * \param ind индекс элемента в массиве.
	 * \return Указатель на константный текстовый редактор.
	 */
	const QLineEdit* getEditor(size_t ind) const{return editors_[ind];}

	/*! \~russian
	 * \brief Печатает глобальные переменные в стиле файла для fttw::FileToTabWidget (см. описание класса fttw::GlobalValuesPrintElem).
	 * \param s поток вывода, куда печатается данный элемент.
	 * \return поток вывода, куда был напечатан данный элемент.
	 */
	virtual std::ostream& printElem(std::ostream &s) const override;
};


/*! \~russian \brief Класс для печати комментариев в стиле оформления fttw::FileToTabWidget
 *
 * Наследуется от класса fttw::PrintElement.
 *
 * Печатает комментарий в указанный поток вывода.
 *
 * Хранит в себе только строку, содержащую комментарий. Комментарием также
 * может быть пустая строка — тогда просто выведется знак перевода строки.
 *
 * Выводит комментарий просто строкой, после которой следует символ перевода строки.
 *
 * Комментарий должен начинаться с "# ", чтобы не мешать разметке таблиц и вкладок.
 *
 * Схема использования:
 * \code
 *
 * CommentElement *ce = new CommentElement("# this is my comment");
 * ce -> printElem();
 *
 * \endcode
 *
 */
class CommentElement : public PrintElement{
	//! \~russian \brief Строка, содержащая комментарий.
	std::string str_;

public:
	//! \~russian \brief Символ для распознавания комментария. Например, '#' . \details В принципе, символ может быть отличным от '#'.
	static const char trigger;

	//! \~russian \brief Пустой конструктор. Задает поле str_ равным "".
	CommentElement() : PrintElement(), str_("") {}

	/*! \~russian
	 * \brief Конструктор с параметром.
	 * \param str строка, содержащая комментарий.
	 */
	explicit CommentElement(const std::string& str) : PrintElement(), str_(str) {}
	//! \~russian \brief Деструктор.
	~CommentElement() {}

	/*! \~russian
	 * \brief Функция задает строку, содержащую комментарий.
	 * \details Изменяет защищенное поле str_. Версия функции для обычного класса std::string.
	 * \param str строка, содержащая новый комментарий (std::string).
	 */
	void setComment(const std::string& str) {str_ = str;}
	/*! \~russian
	 * \brief Функция задает строку, содержащую комментарий.
	 * \details Изменяет защищенное поле str_. Версия функции для QString.
	 * \param str строка, содержащая новый комментарий (QString).
	 */
	void setComment(const QString& str) {str_ = str.toStdString();}

	/*! \~russian
	 * \brief Функция возвращает строку, содержащую комментарий.
	 * \details Читает защищенное поле str_. Строка не может быть изменена.
	 * \return Константная ссылка на строку, содержащую комментарий.
	 */
	const std::string& getComment() const {return str_;}

	/*! \~russian
	 * \brief Печатает комментарий в стиле файла для fttw::FileToTabWidget (см. описание класса fttw::CommentElement).
	 * \param s поток вывода, куда печатается данный элемент.
	 * \return Поток вывода, куда был напечатан данный элемент.
	 */
	virtual std::ostream& printElem(std::ostream &s) const override;
};

} // namespace fttw

#endif // PRINTELEMENT13022017GASPARYANMOSES_H

/*@}*/
