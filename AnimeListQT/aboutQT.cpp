#include "aboutQT.h"

aboutQT::aboutQT(QWidget *parent, const QString version)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowIcon(QIcon(":/AnimeListQT/WindowIcon.png"));
	ui.version_label->setText(version);
	connect(ui.close_button, &QPushButton::pressed, this, &aboutQT::hide);
}
