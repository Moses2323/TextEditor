#include <QLabel>
#include <QMovie>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <functional>

#include "LoaderWidget.h"

namespace {
	const QVector<QString> c_animFileNames = {
		"",
	    ":line.gif",
	    ":spinner1.gif",
	    ":spinner2.gif",
	    ":spinner3.gif",
	    ":cat.gif"
	};

	const unsigned int c_textFrameTime = 300;

}

/*! \~russian
 * \brief Служебная структура для работы класса LoaderWidget
 */
struct LoaderWidgetPrivate {
	/*! \~russian
	 * \brief Основной виджет
	 */
	QLabel *pGlass;
	/*! \~russian
	 * \brief Виджет с текстом
	 */
	QLabel *pText;
	/*! \~russian
	 * \brief Виджет с анимацией
	 */
	QLabel *pAnim;
	/*! \~russian
	 * \brief Контейнер с анимацией
	 */
	QMovie *pMovie;
	/*! \~russian
	 * \brief Таймер обновления текста
	 */
	QTimer *pUpdateTimer;
	/*! \~russian
	 * \brief Графический эффект для прозрачности виджета
	 */
	QGraphicsOpacityEffect *pEffect;
	/*! \~russian
	 * \brief Список строк текста
	 */
	QStringList text;
	/*! \~russian
	 * \brief Индекс текущей строки
	 */
	unsigned int textIndex;

	/*! \~russian
	 * \brief Конструктор
	 */
	LoaderWidgetPrivate();
	/*! \~russian
	 * \brief Деструктор
	 */
	~LoaderWidgetPrivate();
	/*! \~russian
	 * \brief Обноление позиции
	 */
	void updatePos();
	/*! \~russian
	 * \brief Обновление текста
	 */
	void updateText();
};

LoaderWidget::LoaderWidget(QObject *parent) :
	QObject(parent),
	m(new LoaderWidgetPrivate)
{
	setColor(QColor(128,128,128));
	setOpacity(0.75f);

	setText(QStringList() << tr("Loading") << tr("Loading.") << tr("Loading..") << tr("Loading..."));
	setAnimationType(AnimationType_Spinner2);

	connect(m->pUpdateTimer, SIGNAL(timeout()), this, SLOT(sUpdateText()));
}

LoaderWidget::~LoaderWidget()
{
	remove();
	delete m;
}

void LoaderWidget::setColor(const QColor& color)
{
	m->pGlass->setStyleSheet(QString("background-color: %1;").arg(color.name()));
}

void LoaderWidget::setOpacity(float value)
{
	m->pEffect->setOpacity(value);
	m->pGlass->setGraphicsEffect(m->pEffect);
}

void LoaderWidget::setText(const QString& text)
{
	setText(QStringList() << text);
}

void LoaderWidget::setText(const QStringList& text)
{
	m->text = text;
	sUpdateText();
}

void LoaderWidget::setAnimationType(const LoaderWidget::AnimationType type)
{
	m->pMovie->setFileName(c_animFileNames[type]);
	m->pMovie->jumpToFrame(0);

	QSize size = m->pMovie->currentPixmap().size();
	float scaleCoef = 64.0f / qMax(size.width(), size.height());
	size.setWidth(size.width() * scaleCoef);
	size.setHeight(size.height() * scaleCoef);
	m->pMovie->setScaledSize(size);

	m->pAnim->setMovie(m->pMovie);
	m->pAnim->resize(size);
	m->pMovie->start();
}

void LoaderWidget::install(QWidget *victimWidget)
{
	remove();

	m->pGlass->setParent(victimWidget);
	victimWidget->installEventFilter(this);

	m->pGlass->show();
	m->updatePos();

	m->pUpdateTimer->start();

	QEvent event(QEvent::Resize);
	eventFilter(0, &event);
}

void LoaderWidget::remove()
{
	if (m->pGlass->parentWidget() != 0) {
		m->pUpdateTimer->stop();
		m->pGlass->parentWidget()->removeEventFilter(this);
		m->pGlass->hide();
		m->pGlass->setParent(0);
	}
}

bool LoaderWidget::eventFilter(QObject *object, QEvent *event)
{
	QEvent::Type eventType = event->type();
	if ((eventType == QEvent::Show) || (eventType == QEvent::Resize)) {
		m->pGlass->resize(m->pGlass->parentWidget()->size());
		m->pGlass->move(0, 0);
		m->updatePos();
		return true;
	}

	if (eventType == QEvent::Shortcut || eventType == QEvent::ShortcutOverride) {
	}

	if (eventType == QEvent::FocusAboutToChange)
		return false;

	m->pGlass->setFocus();
	event->accept();
	return false;
}

void LoaderWidget::sUpdateText()
{
	m->updateText();
}

LoaderWidgetPrivate::LoaderWidgetPrivate()
{
	pGlass = new QLabel();
	pText = new QLabel(pGlass);
	pAnim = new QLabel(pGlass);
	pMovie = new QMovie();
	pEffect = new QGraphicsOpacityEffect;
	textIndex = 0;

	pUpdateTimer = new QTimer;
	pUpdateTimer->setInterval(300);
}

LoaderWidgetPrivate::~LoaderWidgetPrivate()
{
	pUpdateTimer->deleteLater();
	pAnim->deleteLater();
	pText->deleteLater();
	pMovie->deleteLater();
	pGlass->deleteLater();
	pEffect->deleteLater();
}

void LoaderWidgetPrivate::updatePos()
{
	pAnim->move((pGlass->width() - pAnim->width()) / 2, pGlass->height() / 2 - pAnim->height());
	pText->move((pGlass->width() - pText->width()) / 2, pGlass->height() / 2 + pText->height());
}

void LoaderWidgetPrivate::updateText()
{
	if (text.size()) {
		pText->setText(text.at(textIndex));
		pText->adjustSize();
		textIndex = (textIndex + 1) % text.size();
	}
	else
		pText->setText("");
}
