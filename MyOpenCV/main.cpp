#include <stdafx.h>
#include <MainWindow.h>
#include <Stopwatch.h>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QApplication::setStyle(QStyleFactory::create("Fusion"));


	MainWindow mainwin;

	mainwin.show();

	

	return a.exec();
}
