#include "pharmacist.h"
#include "pharmacy.h"
#include "doctor.h"

pharmacist::pharmacist(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	loggedPharmacist = pharmacy::openFile("subor.txt");                                     //výpis informácií o prihlásenom zamestnancovi lekárne
	ui.name->setText(loggedPharmacist[0]);
	
	if(loggedPharmacist[1] == "Vasa")
		ui.employer->setText(QStringLiteral("Vaša Lekáreò"));
	else ui.employer->setText(loggedPharmacist[1]);
	filename = loggedPharmacist[1] + ".txt";

	QStringList medicine = pharmacy::openFile(filename.toStdString().c_str());             //výpis všetkých liekov lekárne
	for (int i = 0;i < medicine.length();i+=2)
		ui.listWidget->insertItem(i, medicine[i]);

	filename1 = "prescribed/" + loggedPharmacist[1] + ".txt";
	medicine1 = pharmacy::openFile(filename1.toStdString().c_str());                       //výpis objednaných liekov lekárne
	for (int i = 1;i < medicine1.length();i+=2)
		ui.listWidget_2->insertItem(i, medicine1[i]);

	QString imageName = "C:/Users/benco/Documents/icons/" + loggedPharmacist[1] + ".png";  //nastavenie loga lekárne
	QPixmap pix(imageName.toStdString().c_str());
	ui.image->setPixmap(pix);
}

pharmacist::~pharmacist(){}

void pharmacist::on_signOff_clicked() {
	this->close();
	pharmacy* login = new pharmacy();
	login->show();
}

void pharmacist::on_addMedicine_clicked() {
	bool ok;

	QString text = QInputDialog::getText(this, tr("Pridanie"),
		tr("Nazov lieku:"), QLineEdit::Normal,
		QDir::home().dirName(), &ok);

	if (!ok)
		return;

	double price = QInputDialog::getDouble(this, tr("Pridanie"),
		tr("Cena:"), 10.00, -10000, 10000, 2, &ok);

	if (!ok)
		return;

	QFile file(filename.toStdString().c_str());
	if (!file.open(QIODevice::ReadWrite | QIODevice::Append))
		return;

	QTextStream out(&file);
	out.setCodec("UTF-8");

	out << "\n" << text;
	out << "\n" << price;
	file.close();
	ui.listWidget->addItem(text);
}

void pharmacist::on_editMedicine_clicked() {
	bool ok;
	QString newPrice = QInputDialog::getText(this, tr("Pridanie"), tr("Nova cena lieku:"), QLineEdit::Normal,
		QDir::home().dirName(), &ok);

	if (!ok)
		return;

	QFile file(filename.toStdString().c_str());
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
		return;
	QTextStream in(&file);
	QTextStream out(&file);
	in.setCodec("UTF-8");
	out.setCodec("UTF-8");

	QStringList line;
	while (!in.atEnd())
		line.append(in.readLine());

	for (int i = 0;i < line.size();i++) {
		if (ui.listWidget->currentItem()->text() == line[i]) {
			line.replace(i+1, newPrice);
		}
	}
	file.resize(0);
	for (int i = 0;i < line.size();i++) {
		if (i != line.size() - 1)
			out << line[i] << "\n";
		else out << line[i];
	}
	file.close();
}

void pharmacist::on_deleteMedicine_clicked() {
	QFile file(filename.toStdString().c_str());
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
		return;
	QTextStream in(&file);
	QTextStream out(&file);
	in.setCodec("UTF-8");
	out.setCodec("UTF-8");

	QStringList line;
	while (!in.atEnd()) 
		line.append(in.readLine());
	
	for (int i = 0;i < line.size();i++) {
		if (ui.listWidget->currentItem()->text() == line[i]) {
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

	ui.listWidget->takeItem(ui.listWidget->currentRow());
}

void pharmacist::medicineChosen(QListWidgetItem* item) {
	QMessageBox msgBox;                                                                 //výpis informácií o danom lieku
	QStringList priceList = pharmacy::openFile(filename.toStdString().c_str());
	for (int i = 0;i < priceList.length();i+=2) {
		if (item->text() == priceList[i]) {
			QString basicInfo = item->text() + "\n\n" + "Cena lieku: " + priceList[i + 1] + QStringLiteral("€");
			msgBox.setText(basicInfo);
			break;
		}
	}                                   
	QString image = "C:/Users/benco/Documents/medicineimages/" + item->text() + ".jpg";
	msgBox.setIconPixmap(QPixmap(image));
	QString filename = "C:/Users/benco/Documents/medicine/" + item->text() + ".txt";
	QStringList medicineInfo = pharmacy::openFile(filename.toStdString().c_str());
	msgBox.setInformativeText(medicineInfo[0]);
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}

void pharmacist::seeDoctor(QListWidgetItem* item) {
	QMessageBox msgBox; 
	for (int i = 1;i < medicine1.length();i += 2) {
		if (item->text() == medicine1[i]) {
			QString basicInfo = item->text() + "\n\n" + medicine1[i -1];
			msgBox.setText(basicInfo);
			break;
		}
	}
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}

void pharmacist::itemClicked(QListWidgetItem* item) {
	ui.editMedicine->setEnabled(true);
	ui.deleteMedicine->setEnabled(true);
}

void pharmacist::on_save_clicked() {
	bool ok;
	QString text = QInputDialog::getText(this, tr("Blok"), tr("Nazov suboru:"), QLineEdit::Normal,
		QDir::home().dirName(), &ok);
	QString text1 = text + ".txt";
	QFile file(text1);                          //vypísanie txt s objednanymi liekmi
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);
	out.setCodec("UTF-8");

	struct tm * ti;
	time_t tt;
	time(&tt);
	ti = localtime(&tt);

	out << asctime(ti);
	out << "Objednané lieky do lekárne Lieky: " << ui.employer->text() << "\n\n";
	for (int i = 0;i < ui.listWidget_2->count();i++)
		out << ui.listWidget_2->item(i)->text() << "\n";

	out << "\n" << QStringLiteral("Zhotovil pracovník lekárne ") << ui.employer->text() << "\nZamestnanec: " << ui.name->text();
	file.close();
}
	