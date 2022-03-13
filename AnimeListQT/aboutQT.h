#pragma once

#include <QWidget>
#include "ui_aboutQT.h"

class aboutQT : public QWidget
{
	Q_OBJECT

public:
	aboutQT(QWidget *parent = Q_NULLPTR, const QString version = "");
private:
	Ui::aboutQT ui;
};
