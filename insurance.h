#pragma once
#include "pharmacist.h"
#include "pharmacy.h"
#include "doctor.h"
#include <QWidget>
#include "ui_insurance.h"

class insurance : public QWidget
{
	Q_OBJECT

public:
	insurance(QWidget *parent = Q_NULLPTR);
	~insurance();
	QStringList loggedInsuranceEm;

private:
	Ui::insurance ui;

private slots:
	void on_signOff_clicked();
	void patientClicked(QListWidgetItem* item);
	void on_save_clicked();
	void on_addPatient_clicked();
};
