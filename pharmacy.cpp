#include "pharmacy.h"
#include "doctor.h"
#include "md5.h"
#include "pharmacist.h"
#include "insurance.h"

enum Mode {
	DOCTOR,
	INSURANCE_EMPLOEE,
	PHARMACY_EMPLOEE,
	PRESENTATION
};

void setCredentials(Ui::pharmacyClass ui, Mode mode) {
	QString name = "";
	QString pass = "";
	switch (mode) {
		case DOCTOR:
			name = QStringLiteral("Andrášová Andrea");
			pass = "heslo1";
			break;
		case INSURANCE_EMPLOEE:
			name = QStringLiteral("Ragasová Helena");
			pass = "heslo1";
			break;
		case PHARMACY_EMPLOEE:
			name = QStringLiteral("Miklošková Marta");
			pass = "heslo1";
			break;
		default:
			break;
	}

	ui.name->setText(name);
	ui.password->setText(pass);
}

pharmacy::pharmacy(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	insuranceEmployees.clear();
	doctors.clear();
	pharmacyEmployees.clear();
	listOfDoctors = openFile("doctors.txt");
	listOfInsuranceEmployees = openFile("insuranceemployees.txt");
	listOfPharmacyEmployees = openFile("pharmacyemployees.txt");
	int i;

	for (i = 0;i < listOfDoctors.length();i += 4)
		doctors.append(doct(listOfDoctors[i], listOfDoctors[i + 1], listOfDoctors[i + 3],
			openFile(listOfDoctors[i + 2].toStdString().c_str())));

	for (i = 0;i < listOfInsuranceEmployees.length();i += 3)
		insuranceEmployees.append(insuranceEmployee(listOfInsuranceEmployees[i], listOfInsuranceEmployees[i + 1],
			listOfInsuranceEmployees[i + 2]));

	for (i = 0;i < listOfPharmacyEmployees.length();i += 3)
		pharmacyEmployees.append(pharmacyEmployee(listOfPharmacyEmployees[i], listOfPharmacyEmployees[i + 1],
			listOfPharmacyEmployees[i + 2]));

	setCredentials(ui, PRESENTATION);
}

QString pharmacy::encryptdata(QString text) {
	std::string data = text.toStdString();
	std::string data_hex_digest;

	md5 hash;
	hash.update(data.begin(), data.end());
	hash.hex_digest(data_hex_digest);
	
	QString str = QString::fromUtf8(data_hex_digest.c_str());
	return str;
}

QStringList pharmacy::openFile(const char *str) {
	QFile file(str);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return QStringList();

	QTextStream in(&file);
	in.setCodec("UTF-8");
	QString allLine;

	allLine = in.readAll();
	QStringList parts = allLine.split("\n");
	return parts;
}

void pharmacy::on_login_clicked() {
	ui.warningName->setText("");
	ui.warningPassword->setText("");
	ui.incorrect->setText("");

	if (ui.name->text() == "") {
		ui.warningName->setText(QStringLiteral("zadajte prihlasovacie meno"));
	}
	if (ui.password->text() == "") {
		ui.warningPassword->setText(QStringLiteral("zadajte heslo"));
	}

	if (ui.name->text() != "" && ui.password->text() != "") {
		ui.warningName->setText("");
		ui.warningPassword->setText("");
		
		QString hashedPassword = encryptdata(ui.password->text());
		bool correct = false;

		QFile file("subor.txt");
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;
		QTextStream out(&file);
		out.setCodec("UTF-8");

		int i;
		for (i = 0;i < doctors.length();i++) {
			if (ui.name->text() == doctors[i].GetName() && hashedPassword == doctors[i].GetPassword()) {
				out << doctors[i].GetName() << "\n" << doctors[i].GetSpecification() << "\n" << listOfDoctors[i*4 + 2];
				file.close();
				ui.incorrect->setText("");
				correct = true;
				this->close();
				doctor* doctor1 = new doctor();
				doctor1->show();
			}
		}
		for (i = 0;i < insuranceEmployees.length();i++) {
			if (ui.name->text() == insuranceEmployees[i].GetName() && hashedPassword == insuranceEmployees[i].GetPassword()) {
				out << insuranceEmployees[i].GetName() << "\n" << insuranceEmployees[i].GetEmployer();
				file.close();
				ui.incorrect->setText("");
				correct = true;
				this->close();
				insurance *insurance1 = new insurance();
				insurance1->show();
			}
		}
		for (i = 0;i < pharmacyEmployees.length();i++) {
			if (ui.name->text() == pharmacyEmployees[i].GetName() && hashedPassword == pharmacyEmployees[i].GetPassword()) {
				out << pharmacyEmployees[i].GetName() << "\n" << pharmacyEmployees[i].GetEmployer();
				file.close();
				ui.incorrect->setText("");
				correct = true;
				this->close();
				pharmacist* pharmacist1 = new pharmacist();
				pharmacist1->show();
			}
		}

		if (correct == false) {
			file.close();
			ui.incorrect->setText(QStringLiteral("Nesprávne prihlasovacie meno alebo heslo"));
			ui.password->setText("");
		}
	}
}

void pharmacy::passwordEntered() {
	if (ui.visibility->isChecked() != true) {
		ui.password->setEchoMode(QLineEdit::Password);
		ui.visibility->setIcon(QIcon("C:/Users/benco/Documents/icons/notvisible.png"));
	}
	
	else {
		ui.password->setEchoMode(QLineEdit::Normal);
		ui.visibility->setIcon(QIcon("C:/Users/benco/Documents/icons/visible.png"));
	}
}