#include "insurance.h"
#include "pharmacist.h"
#include "pharmacy.h"
#include "doctor.h"

insurance::insurance(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	loggedInsuranceEm = pharmacy::openFile("subor.txt");                                      //v˝pis inform·ciÌ o prihl·senom zamestnancovi poisùovne
	ui.name->setText(loggedInsuranceEm[0]);
	if (loggedInsuranceEm[1] == "Dovera")
		ui.employer->setText(QStringLiteral("DÙvera"));
	else ui.employer->setText(loggedInsuranceEm[1]);

	QString imageName = "C:/Users/benco/Documents/icons/" + loggedInsuranceEm[1] + ".png";    //nastavenie loga lek·rne
	QPixmap pix(imageName.toStdString().c_str());
	ui.image->setPixmap(pix);

	QStringList patients = pharmacy::openFile("patientsInfo.txt");                            //v˝pis pacietov s danou poisùovÚou
	for (int i = 0;i < patients.length();i+=6) {
		if (patients[i + 5] == ui.employer->text()) {
			ui.listWidget->addItem(patients[i]);
		}
	}
}

void insurance::patientClicked(QListWidgetItem* item) {
	ui.listWidget_2->clear();
	QStringList patientsInfo = pharmacy::openFile("prescribedMed.txt");
	for (int i = 0;i < patientsInfo.length();i++) {
		if (patientsInfo[i] == item->text()) {
			QStringList patientsmed = patientsInfo[i + 1].split(";");
			patientsmed.removeLast();
			ui.listWidget_2->addItems(patientsmed);
		}
	}
}
void insurance::on_save_clicked() {
	bool ok;
	QString text = QInputDialog::getText(this, tr("Blok"), tr("Nazov suboru:"), QLineEdit::Normal,
		QDir::home().dirName(), &ok);
	QString text1 = text + ".txt";
	QFile file(text1);                                                                            //vypÌsanie txt s liekmi danÈho pacienta
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);
	out.setCodec("UTF-8");

	struct tm * ti;
	time_t tt;
	time(&tt);
	ti = localtime(&tt);

	out << asctime(ti);
	out << "\nPacient: " << ui.listWidget->currentItem()->text() << "\n\n";
	out << "Lieky:\n";
	for (int i = 0;i < ui.listWidget_2->count();i++) 
		out << ui.listWidget_2->item(i)->text() << "\n";

	out << "\n" << QStringLiteral("Zhotovil pracovnÌk poisùovne ") << ui.employer->text() << "\nZamestnanec: " << ui.name->text();
	file.close();
}

void insurance::on_addPatient_clicked() {
	bool ok;
	QString text = QInputDialog::getText(this, tr("Pridanie"), tr("Meno pacienta:"), QLineEdit::Normal,
		QDir::home().dirName(), &ok);

	if (!ok)	
		return;

	ui.listWidget->addItem(text);
	QFile file("patientsInfo.txt");
	if (!file.open(QIODevice::ReadWrite | QIODevice::Append))
		return;

	QTextStream out(&file);
	out.setCodec("UTF-8");

	out << "\n" << text;
	for (int i = 0;i < 4;i++) {
		out << "\n" << "0";
	}
	text = ui.employer->text();
	out << "\n" << text;
	file.close();
}

insurance::~insurance(){}

void insurance::on_signOff_clicked() {
	this->close();
	pharmacy* login = new pharmacy();
	login->show();
}
