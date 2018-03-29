/** @addtogroup NNTools
 * @{*/

/** @file */

#ifndef SBGLASS_H
#define SBGLASS_H

#include <toolssettings.h>
#include <QObject>

class QColor;
class QString;
class QStringList;
class QWidget;

struct LoaderWidgetPrivate;

/*! \~russian
 * \brief Виджет с анимацией и текстом
 * \details Данный виджет отображает внутри себя анимацию загрузки и сменяющиеся строки текста.
 * Имеется возможно менять вид анимации, строки текста, цвет и прозрачность виджета.
 * Данный виджет устанавливается поверх другого любого виджета, используя метод install() и удаляется, используя метод remove().
 * Данный виджет обычно применяется, чтобы заблокировать какой-то другой виджет во время загрузки данных или внутренних расчетов.
 */
class TOOLS_EXPORT LoaderWidget : public QObject{
	Q_OBJECT
public:
	/*! \~russian
	 * \brief Тип анимации
	 */
	enum AnimationType {
		AnimationType_None = 0, //!< \~russian Анимация отсутствует
		AnimationType_Line, //!< \~russian Полоса загрузки
		AnimationType_Spinner1, //!< \~russian Загрузка с прокручиванием
		AnimationType_Spinner2, //!< \~russian Загрузка с прокручиванием 2
		AnimationType_Spinner3, //!< \~russian Загрузка с прокручиванием 3
		AnimationType_Cat //!< \~russian Загрузка с котом
	};

	/*! \~russian
	 * \brief Конструктор
	 * \param[in] parent Родительский виджет
	 */
	LoaderWidget(QObject *parent = 0);
	/*! \~russian
	 * \brief Деструктор
	 */
	~LoaderWidget();

	/*! \~russian
	 * \brief Установка цвета
	 * \param[in] color Цвет
	 */
	void setColor(const QColor& color);
	/*! \~russian
	 * \brief Установка прозрачности
	 * \param[in] value Значение прозрачночти. 0 - полностью прозрачный, 1 - полностью не прозрачный
	 */
	void setOpacity(float value = 0.75f);
	/*! \~russian
	 * \brief Установка текста
	 * \param[in] text Строка текста
	 */
	void setText(const QString& text);
	/*! \~russian
	 * \brief Установка текста
	 * \param[in] text Список строк текста
	 */
	void setText(const QStringList& text);
	/*! \~russian
	 * \brief Установка типа анимации
	 * \param type
	 */
	void setAnimationType(const AnimationType type);

public slots:
	/*! \~russian
	 * \brief Установка
	 * \param[in] victimWidget Виджет, на который следует установить LoaderWidget
	 */
	void install(QWidget* victimWidget);

	/*! \~russian
	 * \brief Удаление
	 */
	void remove();

protected:
	/*! \~russian
	 * \brief Обработчик событий Qt
	 * \param[in] object Объект
	 * \param[in] event Событие
	 * \return Флаг обработки события
	 */
	bool eventFilter(QObject*, QEvent* event);

private:
	/*! \~russian
	 * \brief Внутренние данные
	 */
	LoaderWidgetPrivate *m;

private slots:
	/*! \~russian
	 * \brief Слот обновления текста
	 */
	void sUpdateText();

};

#endif // SBGLASS_H

/*@}*/
