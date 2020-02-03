#pragma once

#include <QtWidgets>
#include "ui_pharmacy.h"

class user {
private:
	QString name;
	QString password;

public:
	user(){}
	user(QString name, QString password) {
		this->name = name;
		this->password = password;
	}

	QString GetName() { return name; }
	QString GetPassword() { return password; }
};

class doct :public user {
private:
	QString specification;
	QStringList patients;

public:
	doct(){}
	doct(QString name, QString password, QString specification, QStringList patients) :user(name, password) {
		this->specification = specification;
		this->patients = patients;
	}

	QStringList GetPatients() { return patients; }
	QString GetSpecification() { return specification; }
};

class insuranceEmployee :public user {
private:
	QString Employer;
public:
	insuranceEmployee(QString name, QString password, QString Employer) : user(name, password) {
		this->Employer = Employer;
	}

	QString GetEmployer() { return Employer; }
};

class pharmacyEmployee :public user {
private:
	QString Employer;
public:
	pharmacyEmployee(QString name, QString password, QString Employer) : user(name, password) {
		this->Employer = Employer;
	}

	QString GetEmployer() { return Employer; }
};

class pharmacy : public QMainWindow
{
	Q_OBJECT

public:
	pharmacy(QWidget *parent = Q_NULLPTR);
	QStringList static openFile(const char *str);
	QString static encryptdata(QString text);
	doct loggedDoctor;

private:
	Ui::pharmacyClass ui;
	QStringList listOfDoctors;
	QStringList listOfInsuranceEmployees;
	QStringList listOfPharmacyEmployees;
	QList<doct> doctors;
	QList<insuranceEmployee> insuranceEmployees;
	QList<pharmacyEmployee> pharmacyEmployees;

private slots:
	void on_login_clicked();
	void passwordEntered();
}; 