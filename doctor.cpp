#include "doctor.h"
#include "pharmacy.h"

void doctor::editFile(const char *str) {
	QFile file(str);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	
	QTextStream out(&file);
	out.setCodec("UTF-8");

	for (int i = 0;i < ui.listWidget->count();i++) {
		if (ui.listWidget->item(i)->text() == "")
			ui.listWidget->takeItem(i);
		out << ui.listWidget->item(i)->text() << "\n";
	}
	file.close();
}

doctor::doctor(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	loggedDoctor = pharmacy::openFile("subor.txt");                                         //výpis informácií o prihláseno doktorovi
	ui.name->setText(loggedDoctor[0]);
	ui.specification->setText(loggedDoctor[2]);

	listOfPatients = pharmacy::openFile(loggedDoctor[1].toStdString().c_str());             //naèítanie pacientov prihláseného doktora
	for (int i = 0; i < listOfPatients.length(); i++) {
		ui.listWidget->insertItem(i, listOfPatients[i]);
		if (ui.listWidget->item(i)->text() == "")
			ui.listWidget->takeItem(i);
	}
	medicineDrMax = pharmacy::openFile("Dr.Max.txt");                                       //naèítanie liekov do listWidgetov
	for (int i = 0; i < medicineDrMax.length(); i+=2)
		ui.listWidget_2->insertItem(i, medicineDrMax[i]);
	medicineVasa = pharmacy::openFile("Vasa.txt");
	for (int i = 0; i < medicineVasa.length(); i+=2)
		ui.listWidget_3->insertItem(i, medicineVasa[i]);
	medicineBenu = pharmacy::openFile("Benu.txt");
	for (int i = 0; i < medicineBenu.length(); i+=2)
		ui.listWidget_4->insertItem(i, medicineBenu[i]);

	employers = pharmacy::openFile("pharmacyemployees.txt");                               //naèítanie zamestnancov do listWidgetov
	for (int i = 0; i < employers.length(); i += 3) {
		QString info = employers[i] + "," + employers[i + 2];
		ui.listWidget_6->addItem(info);
	}
	employers = pharmacy::openFile("insuranceemployees.txt");
	for (int i = 0; i < employers.length(); i += 3) {
		QString info = employers[i] + "," + employers[i + 2];
		ui.listWidget_7->addItem(info);
	}
}

doctor::~doctor(){}

void doctor::on_addPatient_clicked() {
	bool ok,ok1,ok2,ok3,ok4,ok5;
	QString name, dateOfBirth;
	double weight, height;
	QString insuranceCompany, bloodType;
	
	name = QInputDialog::getText(this, tr("Pridanie"), tr("Meno pacienta:"), QLineEdit::Normal,
		QDir::home().dirName(), &ok);
	if (!ok)
		return;
	dateOfBirth = QInputDialog::getText(this, tr("Pridanie"), tr("Datum narodenia:"), QLineEdit::Normal,
		QDir::home().dirName(), &ok1);
	if (!ok1)
		return;
	weight = QInputDialog::getDouble(this, tr("Pridanie"),
		tr("Vaha:"), 37.56, -10000, 10000, 2, &ok2);
	if (!ok2)
		return;
	height = QInputDialog::getDouble(this, tr("Pridanie"),
		tr("Vyska:"), 37.56, -10000, 10000, 2, &ok3);
	if (!ok3)
		return;

	QStringList items;
	items << tr("A") << tr("B") << tr("AB") << tr("0");

	bloodType = QInputDialog::getItem(this, tr("Pridanie"),
		tr("Krvna skupina:"), items, 0, false, &ok4);
	if (!ok4)
		return;

	QString nameOfInsurance = QStringLiteral("Dôvera");
	items.clear();
	items << tr(nameOfInsurance.toStdString().c_str()) << tr("Union");

	insuranceCompany = QInputDialog::getItem(this, tr("Pridanie"),
		tr("Poistovne:"), items, 0, false, &ok5);
	if (!ok5)
		return;

	QFile file("patientsInfo.txt");
	if (!file.open(QIODevice::ReadWrite | QIODevice::Append))
			return;

	QTextStream out(&file);
	out.setCodec("UTF-8");

	ui.listWidget->addItem(name);
	out << "\n" << name;
	out << "\n" << dateOfBirth;
	out << "\n" << weight;
	out << "\n" << height;
	out << "\n" << bloodType;
	out << "\n" << insuranceCompany;
	
	file.close();
	editFile(loggedDoctor[1].toStdString().c_str());
}

void doctor::on_deletePatient_clicked(){
	QFile file("patientsInfo.txt");
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
		return;
	
	QTextStream in(&file);
	in.setCodec("UTF-8");
	QTextStream out(&file);
	out.setCodec("UTF-8");

	QStringList line;
	while (!in.atEnd())
		line.append(in.readLine());

	for (int i = 0;i < line.size();i+=6) {
		if (line[i] == ui.listWidget->currentItem()->text()) {
			line.removeAt(i + 5);
			line.removeAt(i + 4);
			line.removeAt(i + 3);
			line.removeAt(i + 2);
			line.removeAt(i + 1);
			line.removeAt(i);
			break;
		}
	}

	file.resize(0);
	for (int i = 0;i < line.size();i++) {
		if(i!=line.size()-1)
			out << line[i] << "\n";
		else out << line[i];
	}
	file.close();
	
	ui.listWidget->takeItem(ui.listWidget->currentRow());
	editFile(loggedDoctor[1].toStdString().c_str());

	ui.nameOfSelectedPatient->setText("");
	ui.dateOfBirth->setText("");
	ui.weight->setText("");
	ui.height->setText("");
	ui.bloodType->setText("");
	ui.insurance->setText("");
	ui.prescribe->setEnabled(false);
	ui.save->setEnabled(false);
	ui.deleteMedicine->setEnabled(false);
	ui.deletePatient->setEnabled(false);
}

void doctor::itemSelected(QListWidgetItem* item) {
	ui.listWidget_8->clear();
	ui.listWidget_9->clear();
	ui.listWidget_10->clear();

	ui.nameOfSelectedPatient->setText(item->text());                                  //výpis informácií o pacientovi
	QStringList patientInfo = pharmacy::openFile("patientsInfo.txt");
	for (int i = 0;i < patientInfo.length();i+=6) {
		if (patientInfo[i] == item->text()) {
			ui.dateOfBirth->setText(patientInfo[i + 1]);
			ui.weight->setText(patientInfo[i + 2]);
			ui.height->setText(patientInfo[i + 3]);
			ui.bloodType->setText(patientInfo[i + 4]);
			ui.insurance->setText(patientInfo[i + 5]);
			break;
		}
	}
	ui.prescribe->setEnabled(true);
	ui.save->setEnabled(true);
	ui.deleteMedicine->setEnabled(true);
	ui.deletePatient->setEnabled(true);

	ui.listWidget_5->clear();
	double money = 0;
	ui.money->setText("");     
	QStringList patientInfo1 = pharmacy::openFile("prescribedMed.txt");              //výpis už predpísaných a zaplatených liekov pacienta
	for (int i = 0;i < patientInfo1.length();i += 2) {
		if (patientInfo1[i] == item->text()) {
			QStringList patientsmed = patientInfo1[i + 1].split(";");
			patientsmed.last().remove(QStringLiteral("€"));
			money = money + patientsmed.last().toDouble();
			patientsmed.removeLast();
			ui.listWidget_5->addItems(patientsmed);
		}
	}
	if(money!=0)
		ui.money->setText(QString::number(money));
}

void doctor::medicineChosen(QListWidgetItem * item){
	QMessageBox msgBox;                                                              //výpis informácií o danom lieku
	msgBox.setText(item->text());
	QString image = "C:/Users/benco/Documents/medicineimages/" + item->text() + ".jpg";
	msgBox.setIconPixmap(QPixmap(image));
	QString filename ="C:/Users/benco/Documents/medicine/" + item->text() + ".txt";
	QStringList medicineInfo = pharmacy::openFile(filename.toStdString().c_str());
	msgBox.setInformativeText(medicineInfo[0]);
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}

void doctor::on_prescribe_clicked() {                                                //pripravené na predpis
	if (ui.tabWidget->currentIndex() == 0) {
		ui.listWidget_8->addItem(ui.listWidget_2->currentItem()->text());
		ui.tabWidget_3->setCurrentIndex(0);
	}
		
	if (ui.tabWidget->currentIndex() == 1) {
		ui.listWidget_9->addItem(ui.listWidget_3->currentItem()->text());
		ui.tabWidget_3->setCurrentIndex(1);
	}

	if (ui.tabWidget->currentIndex() == 2) {
		ui.listWidget_10->addItem(ui.listWidget_4->currentItem()->text());
		ui.tabWidget_3->setCurrentIndex(2);
	}
}

void doctor::on_deleteMedicine_clicked() {
	if (ui.tabWidget_3->currentIndex() == 0)
		ui.listWidget_8->takeItem(ui.listWidget_8->currentRow());

	if (ui.tabWidget_3->currentIndex() == 1)
		ui.listWidget_9->takeItem(ui.listWidget_9->currentRow());

	if (ui.tabWidget_3->currentIndex() == 2)
		ui.listWidget_10->takeItem(ui.listWidget_10->currentRow());
}

void doctor::on_save_clicked() {                                                       //predpísané
	double money = 0;
	int i,j;
	bool ok;
	QString text = QInputDialog::getText(this, tr("Blok"), tr("Nazov suboru:"), QLineEdit::Normal,
		QDir::home().dirName(), &ok);
	QString text1 = text + ".txt";
	QFile file(text1);																	//vypísanie txt s predpisom liekov danému pacientovi
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QFile file1("prescribed/Dr.Max.txt");                                               //vypísanie objednaných liekov k danej lekárni
	if (!file1.open(QIODevice::ReadWrite | QIODevice::Append))
		return;
	QFile file2("prescribed/Vasa.txt");
	if (!file2.open(QIODevice::ReadWrite | QIODevice::Append))
		return;
	QFile file3("prescribed/Benu.txt");
	if (!file3.open(QIODevice::ReadWrite | QIODevice::Append))
		return;
	QFile file4("prescribedMed.txt");                                                   //uloženie predpísaných liekov
		if (!file4.open(QIODevice::ReadWrite | QIODevice::Append))
			return;

	QTextStream receipt(&file);
	receipt.setCodec("UTF-8");
	QTextStream medDrMax(&file1);
	medDrMax.setCodec("UTF-8");
	QTextStream medVasa(&file2);
	medVasa.setCodec("UTF-8");
	QTextStream medBenu(&file3);
	medBenu.setCodec("UTF-8");
	QTextStream med(&file4);
	med.setCodec("UTF-8");

	struct tm * ti;
	time_t tt;
	time(&tt);
	ti = localtime(&tt);

	receipt << asctime(ti);
	receipt << "\nPacient: " << ui.nameOfSelectedPatient->text() << "\n\n";
	receipt << "Predpísané lieky: " << "\n";
	med << ui.nameOfSelectedPatient->text() << "\n";

	for (i = 0;i < ui.listWidget_8->count();i++) {
		for (j = 0;j < medicineDrMax.length();j+=2) {
			if (ui.listWidget_8->item(i)->text() == medicineDrMax[j]) {
				money = money + medicineDrMax[j + 1].toDouble();
				break;
			}
		}
		receipt << ui.listWidget_8->item(i)->text() << "  " << medicineDrMax[j + 1] << QStringLiteral("€") << "\n";
		medDrMax << QStringLiteral("Meno lekára: MuDr.") << ui.name->text() << "\n" << ui.listWidget_8->item(i)->text() << "\n";
		med << ui.listWidget_8->item(i)->text() << ";";
	}
	
	for (i = 0;i < ui.listWidget_9->count();i++) {
		for (j = 0;j < medicineVasa.length();j += 2) {
			if (ui.listWidget_9->item(i)->text() == medicineVasa[j]) {
				money = money + medicineVasa[j + 1].toDouble();
				break;
			}
		}
		receipt << ui.listWidget_9->item(i)->text() << "  " << medicineVasa[j + 1] << QStringLiteral("€") << "\n";
		medVasa << QStringLiteral("Meno lekára: MuDr.") << ui.name->text() << "\n" << ui.listWidget_9->item(i)->text() << "\n";
		med << ui.listWidget_9->item(i)->text() << ";";
	}
		
	for (i = 0;i < ui.listWidget_10->count();i++) {
		for (j = 0;j < medicineBenu.length();j += 2) {
			if (ui.listWidget_10->item(i)->text() == medicineBenu[j]) {
				money = money + medicineBenu[j + 1].toDouble();
				break;
			}
		}
		receipt << ui.listWidget_10->item(i)->text() << "  " << medicineBenu[j + 1] << QStringLiteral("€") << "\n";
		medBenu << QStringLiteral("Meno lekára: MuDr.") << ui.name->text() << "\n" << ui.listWidget_10->item(i)->text() << "\n";
		med << ui.listWidget_10->item(i)->text() << ";";
	}
	receipt << "\n" << QStringLiteral("Lieky predpísal: Mudr.") + ui.name->text();
	receipt << "\n" << "Celková cena: " << money << QStringLiteral("€");
	med << money << QStringLiteral("€") << "\n";

	file.close();
	file1.close();
	file2.close();
	file3.close();
	file4.close();

	ui.listWidget_8->clear();
	ui.listWidget_9->clear();
	ui.listWidget_10->clear();
}

void doctor::on_addCoworker_clicked() {
	if (ui.tabWidget_2->currentIndex() == 1) {
		bool ok,ok1,ok2;
		QString text = QInputDialog::getText(this, tr("Pridanie"), tr("Meno zamestnanca poistovne:"), QLineEdit::Normal,
			QDir::home().dirName(), &ok);
		if (!ok)
			return;
		QString pass = QInputDialog::getText(this, tr("Pridanie"), tr("Heslo:"), QLineEdit::Normal,
			QDir::home().dirName(), &ok1);
		if (!ok1)
			return;
		QStringList items;

		items << tr("Dovera") << tr("Union");

		QString insurance = QInputDialog::getItem(this, tr("Pridanie"),
			tr("Poistovne:"), items, 0, false, &ok2);
		if (!ok2)
			return;

		hashed = pharmacy::encryptdata(pass);
		QFile file("insuranceemployees.txt");
		if (!file.open(QIODevice::ReadWrite | QIODevice::Append))
			return;

		QTextStream out(&file);
		out.setCodec("UTF-8");

		out << "\n" << text << "\n";
		out << hashed << "\n";
		out << insurance;
		file.close();

		QString info = text + "," + insurance;
		ui.listWidget_6->addItem(info);
	}

	if (ui.tabWidget_2->currentIndex() == 0) {
		bool ok, ok1, ok2;
		QString text = QInputDialog::getText(this, tr("Pridanie"), tr("Meno zamestnanca lekarne:"), QLineEdit::Normal,
			QDir::home().dirName(), &ok);
		if (!ok)
			return;
		QString pass = QInputDialog::getText(this, tr("Pridanie"), tr("Heslo:"), QLineEdit::Normal,
			QDir::home().dirName(), &ok1);
		if (!ok1)
			return;
		QStringList items;

		items << tr("Vasa") << tr("Benu") << tr("Dr.Max");

		QString insurance = QInputDialog::getItem(this, tr("Pridanie"),
			tr("Lekarne:"), items, 0, false, &ok2);
		if (!ok2)
			return;

		hashed = pharmacy::encryptdata(pass);
		QFile file("pharmacyemployees.txt");
		if (!file.open(QIODevice::ReadWrite | QIODevice::Append))
			return;

		QTextStream out(&file);
		out.setCodec("UTF-8");

		out << "\n" << text << "\n";
		out << hashed << "\n";
		out << insurance;
		file.close();

		QString info = text + "," + insurance;
		ui.listWidget_6->addItem(info);
	}
}

void doctor::on_deleteCoworker_clicked() {
	if (ui.tabWidget_2->currentIndex() == 0) {
		QFile file("pharmacyemployees.txt");
		if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
			return;

		QTextStream in(&file);
		in.setCodec("UTF-8");
		QTextStream out(&file);
		out.setCodec("UTF-8");

		QStringList line;
		while (!in.atEnd())
			line.append(in.readLine());

		for (int i = 0;i < line.size();i += 3) {
			QStringList name = ui.listWidget_6->currentItem()->text().split(",");
			if (line[i] == name[0]) {
				line.removeAt(i + 2);
				line.removeAt(i + 1);
				line.removeAt(i);
				break;
			}
		}

		file.resize(0);
		for (int i = 0;i < line.size();i++) {
			if (i != line.size() - 1)
				out << line[i] << "\n";
			else out << line[i];
		}
		file.close();

		ui.listWidget_6->takeItem(ui.listWidget_6->currentRow());
	}
	if (ui.tabWidget_2->currentIndex() == 1) {
		QFile file("insuranceemployees.txt");
		if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
			return;

		QTextStream in(&file);
		in.setCodec("UTF-8");
		QTextStream out(&file);
		out.setCodec("UTF-8");

		QStringList line;
		while (!in.atEnd())
			line.append(in.readLine());

		for (int i = 0;i < line.size();i += 3) {
			QStringList name = ui.listWidget_6->currentItem()->text().split(",");
			if (line[i] == name[0]) {
				line.removeAt(i + 2);
				line.removeAt(i + 1);
				line.removeAt(i);
				break;
			}
		}

		file.resize(0);
		for (int i = 0;i < line.size();i++) {
			if (i != line.size() - 1)
				out << line[i] << "\n";
			else out << line[i];
		}
		file.close();

		ui.listWidget_7->takeItem(ui.listWidget_7->currentRow());
	}
}

void doctor::on_signOff_clicked() {
	this->close();
	pharmacy* login = new pharmacy();
	login->show();
}