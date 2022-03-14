#include "AnimeListQT.h"

AnimeListQT::AnimeListQT(QWidget *parent)
    : QMainWindow(parent), m_path("")
{
    ui.setupUi(this);
    setup();
}

void AnimeListQT::setup() {
    setWindowIcon(QIcon(":/AnimeListQT/WindowIcon.png"));
    QSettings settings{ qApp->applicationDirPath() + "/config.ini", QSettings::IniFormat };
    m_path = settings.value("path").toString();
    action_open(true);

    connect(ui.add_button, &QPushButton::pressed, this, &AnimeListQT::add_item);
    connect(ui.delete_button, &QPushButton::pressed, this, &AnimeListQT::delete_item);
    connect(ui.edit_button, &QPushButton::pressed, this, &AnimeListQT::edit_item);
    connect(ui.titles_widget, &QTreeWidget::itemClicked, this, &AnimeListQT::enable_deletion);

    ui.edit_button->setVisible(false);

    connect(ui.actionSave, &QAction::triggered, this, &AnimeListQT::action_save);
    ui.actionSave->setShortcut(QKeySequence(tr("Ctrl+S")));
    connect(ui.actionOpen, &QAction::triggered, this, &AnimeListQT::action_open);
    ui.actionOpen->setShortcut(QKeySequence(tr("Ctrl+O")));
    connect(ui.actionAbout, &QAction::triggered, this, &AnimeListQT::action_about);
    connect(ui.actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    ui.actionQuit->setShortcut(QKeySequence(tr("Alt+F4")));
} // Setting everything up

void AnimeListQT::add_item() {
    askboxQT dialog{ this };
    dialog.exec();

    if (!dialog.result())
        return;

    if (ui.titles_widget->topLevelItemCount() == 0) {
        ui.titles_widget->setSortingEnabled(true);
        ui.titles_widget->sortByColumn(Date, Qt::DescendingOrder);
    }

    QTreeWidgetItem* add{new QTreeWidgetItem};
    add->setText(Title, dialog.get_title());
    add->setText(Status, dialog.get_status());
    add->setText(Date, QDateTime::currentDateTime().toString("hh:mm, d MMMM yyyy"));

    ui.titles_widget->insertTopLevelItem(0,add);
} // Get the item from dialog and add it

void AnimeListQT::enable_deletion(QTreeWidgetItem* item) {
    m_item = item;
    ui.edit_button->setVisible(true);
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

    delete m_item;
    if (ui.titles_widget->topLevelItemCount() == 0) {
        ui.delete_button->setEnabled(false);
        ui.titles_widget->setSortingEnabled(false);
        ui.edit_button->setVisible(false);
    }
    m_item = ui.titles_widget->topLevelItem( Title );
} // Confirm the deletion process and do it

void AnimeListQT::edit_item() {
    QTreeWidgetItem* edit{ ui.titles_widget->currentItem()};
    askboxQT dialog{ this , askboxQT::Edit, edit->text(Title), edit->text(Status)};
    dialog.exec();

    if (!dialog.result())
        return;

    edit->setText(Title, dialog.get_title());
    edit->setText(Status, dialog.get_status());
    edit->setText(Date, QDateTime::currentDateTime().toString("hh:mm, d MMMM yyyy"));
} // Get the edited item data and edit it

void AnimeListQT::action_save() {
    QList<QTreeWidgetItem*> items;
    for (QTreeWidgetItemIterator iter{ ui.titles_widget }; *iter; ++iter) {
        items.append(*iter);
    }

    QJsonObject save_object;
    for (int it{}; it < items.size(); ++it) {
        QJsonArray tmp;
        for (int iter{ Status }; iter <= Date; ++iter) {
            tmp.append(items[it]->text(iter));
        }
        save_object[items[it]->text(Title)] = tmp;
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

    QList<QTreeWidgetItem*> items;
    for (QJsonObject::iterator iterator{ read_object.begin() }; iterator != read_object.end(); ++iterator) {
        QTreeWidgetItem* item_tmp{ new QTreeWidgetItem };
        item_tmp->setText(Title, iterator.key());

        QJsonValue value_tmp{ read_object.value(iterator.key()) };
        QJsonArray array_tmp{ value_tmp.toArray() };

        item_tmp->setText(Status, array_tmp[Status - 1].toString());
        item_tmp->setText(Date, array_tmp[Date - 1].toString());

        items.append(item_tmp);
    }
    ui.titles_widget->clear();
    ui.titles_widget->addTopLevelItems(items);

    QSettings settings{qApp->applicationDirPath() + "/config.ini", QSettings::IniFormat};
    settings.setValue("path", m_path);
} // Read the selected json file and add it to the window

void AnimeListQT::action_about() {
    aboutQT* dialog{ new aboutQT( Q_NULLPTR , version) };
    dialog->show();
} // Open the about widget
