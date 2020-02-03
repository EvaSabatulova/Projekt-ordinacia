#include "pharmacy.h"
#include"doctor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	pharmacy w;
	w.show();
	return a.exec();
} 