#include "AnimeListQT.h"

AnimeListQT::AnimeListQT(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setup();
}

void AnimeListQT::setup() {
    setWindowIcon(QIcon(":/AnimeListQT/WindowIcon.png"));

    connect(this, &AnimeListQT::items_changed, this, &AnimeListQT::change_handler);

    connect(ui.add_button, &QPushButton::pressed, this, &AnimeListQT::add_item);
    connect(ui.delete_button, &QPushButton::pressed, this, &AnimeListQT::delete_item);
    connect(ui.titles_widget, &QTableWidget::cellClicked, this, &AnimeListQT::enable_deletion);

    connect(ui.actionSave, &QAction::triggered, this, &AnimeListQT::action_save);
    ui.actionSave->setShortcut(QKeySequence(tr("Ctrl+S")));
    connect(ui.actionOpen, &QAction::triggered, this, &AnimeListQT::action_open);
    ui.actionOpen->setShortcut(QKeySequence(tr("Ctrl+O")));
    connect(ui.actionAbout, &QAction::triggered, this, &AnimeListQT::action_about);
    connect(ui.actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    ui.actionQuit->setShortcut(QKeySequence(tr("Alt+F4")));

    QSettings settings{ qApp->applicationDirPath() + "/config.ini", QSettings::IniFormat };
    m_path = settings.value("path").toString();
    action_open(true);
} // Setting everything up

void AnimeListQT::change_checker(int row, int column) {
    if (column == Title || column == Status) {
        ui.titles_widget->item(row, Date)->setText(QDateTime::currentDateTime().toString("hh:mm, d MMMM yyyy"));
    }
}

void AnimeListQT::change_handler() {
    if (ui.titles_widget->rowCount() == 0) {
        ui.delete_button->setEnabled(false);
        ui.titles_widget->setSortingEnabled(false);
    }
    else if (ui.titles_widget->rowCount() != 0) {
        ui.titles_widget->setSortingEnabled(true);
        ui.titles_widget->sortByColumn(Title, Qt::AscendingOrder);
    }
    connect(ui.titles_widget, &QTableWidget::cellChanged, this, &AnimeListQT::change_checker);
}

void AnimeListQT::add_item() {
    ui.titles_widget->insertRow(0);
    auto model{ ui.titles_widget->model() };
    model->setData(ui.titles_widget->model()->index(0, Date), QDateTime::currentDateTime().toString("hh:mm, d MMMM yyyy"));
    emit items_changed();
} // Get the item from dialog and add it

void AnimeListQT::enable_deletion(int row) {
    m_row = row;
    ui.delete_button->setEnabled(true);
} // If the selection is made, enable deletion

void AnimeListQT::delete_item() {
    QMessageBox* get_Answer{ new QMessageBox };

    get_Answer->setWindowTitle("Warning!");
    get_Answer->setText("Are you sure to delete all data of this title?");
    get_Answer->setIcon( QMessageBox::Warning );
    get_Answer->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    int answer{ get_Answer->exec() };

    if (QMessageBox::Cancel == answer)
        return;

    ui.titles_widget->removeRow(m_row);
    m_row = 0;
    emit items_changed();
} // Confirm the deletion process and do it

void AnimeListQT::action_save() {
    QJsonObject save_object;
    for (int iter_out{}; iter_out < ui.titles_widget->rowCount(); ++iter_out) {
        QJsonArray tmp;
        for (int iter_in{ Status }; iter_in < ui.titles_widget->columnCount(); ++iter_in) {
            tmp.append(ui.titles_widget->item(iter_out, iter_in)->text());
        }
        save_object[ui.titles_widget->item(iter_out, Title)->text()] = tmp;
    }

    if(m_path.isEmpty())
        m_path = QFileDialog::getSaveFileName(this, tr("Save as a file"), "", tr("Json File(*.json)"));
    QFile save{ m_path };
    if (!save.open(QFile::WriteOnly | QFile::Text))
        return;

    save.write(QJsonDocument(save_object).toJson());

} // Save the items to the json file

void AnimeListQT::action_open(bool Saved_Settings) {
    if (!Saved_Settings) {
        QString tmp_path{ QFileDialog::getOpenFileName(this, tr("Open a file"), "", tr("Json File(*.json)")) };
        if (tmp_path.isNull())
            return;
        m_path = tmp_path;
    }
    
    QFile open{ m_path };

    if (!open.open(QFile::ReadOnly | QFile::Text))
        return;

    QByteArray read{open.readAll()};
    QJsonObject read_object{ QJsonDocument::fromJson(read).object() };

    int count{};
    ui.titles_widget->setRowCount(0);
    for (QJsonObject::iterator iterator{ read_object.begin() }; iterator != read_object.end(); ++iterator, ++count) {
        ui.titles_widget->insertRow(count);
        auto model{ ui.titles_widget->model() };
        model->setData(ui.titles_widget->model()->index(count, Title), iterator.key());
        
        QJsonValue value_tmp{ read_object.value(iterator.key()) };
        QJsonArray array_tmp{ value_tmp.toArray() };

        model->setData(ui.titles_widget->model()->index(count, Status), array_tmp[Status - 1].toString());
        model->setData(ui.titles_widget->model()->index(count, Date), array_tmp[Date - 1].toString());
    }
    
    emit items_changed();

    QSettings settings{qApp->applicationDirPath() + "/config.ini", QSettings::IniFormat};
    settings.setValue("path", m_path);
} // Read the selected json file and add it to the window

void AnimeListQT::action_about() {
    aboutQT* dialog{ new aboutQT( Q_NULLPTR , version) };
    dialog->show();
} // Open the about widget
