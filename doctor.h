#pragma once

#include <QWidget>
#include "ui_doctor.h"
#include "pharmacy.h"

class doctor : public QWidget
{
	Q_OBJECT

public:
	doctor(QWidget *parent = Q_NULLPTR);
	~doctor();
	QStringList loggedDoctor;
	QStringList listOfPatients;
	QStringList medicineDrMax;
	QStringList medicineBenu;
	QStringList medicineVasa;
	QStringList employers;
	void editFile(const char *str);
	QString hashed;

private:
	Ui::doctor ui;

private slots:
	void on_addPatient_clicked();
	void on_deletePatient_clicked();
	void itemSelected(QListWidgetItem *item);
	void medicineChosen(QListWidgetItem *item);
	void on_prescribe_clicked();
	void on_deleteMedicine_clicked();
	void on_signOff_clicked();
	void on_save_clicked();
	void on_addCoworker_clicked();
	void on_deleteCoworker_clicked();
};