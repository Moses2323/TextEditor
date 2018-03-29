# TextToTabEditor

Текстовый редактор в виде виджета (QWidget), который может преобразовать входной текстовый файл,
размеченный специальным образом, в виджет с таблицами и вкладками.

Пример использования:

```
#include "texttotabeditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	fttw::TextToTabEditorShell sash;
	sash.setGeometry(200,200,800,600);
	sash.show();

	return a.exec();
}
```

При редактировании таблиц или вкладок после кнопки сохранения изменится и сам файл.
Различные примеры использования функционала редактора можно найти в файлах test*.txt

Комментарии к коду сделаны в стиле doxygen
