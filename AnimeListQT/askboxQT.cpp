#include "askboxQT.h"

askboxQT::askboxQT(QWidget* parent, int type, QString title, QString status)
	: QDialog(parent), m_type(type), m_title(title), m_status(status)
{
	ui.setupUi(this);
	setWindowIcon(QIcon(":/AnimeListQT/WindowIcon.png"));
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &askboxQT::accepted_input);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &askboxQT::rejected_input);
	if (m_type == Add)
		setWindowTitle("Create new data");
	else if (m_type == Edit) {
		QMap<QString, int> map;

		map["Complete"] = 0;
		map["Watching"] = 1;
		map["Plan to watch"] = 2;

		setWindowTitle("Edit data");
		ui.title_text->setText(m_title);
		ui.status_box->setCurrentIndex(map.value(status));
	}
	
}

void askboxQT::accepted_input() {
	m_title = ui.title_text->text();
	m_status = ui.status_box->currentText();
	accept();
}

void askboxQT::rejected_input() {
	reject();
}

QString askboxQT::get_title() {
	return m_title;
}

QString askboxQT::get_status() {
	return m_status;
}