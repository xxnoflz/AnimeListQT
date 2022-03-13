#pragma once

#include <QWidget>
#include "ui_askboxQT.h"


class askboxQT : public QDialog
{
	Q_OBJECT

public:
	askboxQT(QWidget *parent = Q_NULLPTR, int type = Add, QString title = "", QString status= "");
	QString get_title();
	QString get_status();
	enum type {
		Add,
		Edit
	};
private slots:
	void accepted_input();
	void rejected_input();
private:
	Ui::askboxQT ui;
	int m_type;
	QString m_title;
	QString m_status;
};
