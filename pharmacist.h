#pragma once
#include "pharmacy.h"
#include "doctor.h"
#include <QWidget>
#include "ui_pharmacist.h"

class pharmacist : public QWidget
{
	Q_OBJECT

public:
	pharmacist(QWidget *parent = Q_NULLPTR);
	~pharmacist();
	QStringList loggedPharmacist;
	QString filename;
	QString filename1;
	QStringList medicine1;

private:
	Ui::pharmacist ui;

private slots:
	void on_signOff_clicked();
	void on_addMedicine_clicked();
	void on_editMedicine_clicked();
	void on_deleteMedicine_clicked();
	void medicineChosen(QListWidgetItem* item);
	void seeDoctor(QListWidgetItem* item);
	void itemClicked(QListWidgetItem* item);
	void on_save_clicked();
};

