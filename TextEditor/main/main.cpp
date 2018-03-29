#include "shell.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	SimpleArtificialShell sash;
	sash.setGeometry(200,200,800,600);
	sash.show();

	return a.exec();
}
