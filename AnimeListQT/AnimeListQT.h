#pragma once

#include <QtWidgets/QMainWindow>
#include <QDateTime>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QFileDialog>
#include <QSettings>

#include "aboutQT.h"
#include "askboxQT.h"
#include "ui_AnimeListQT.h"

class AnimeListQT : public QMainWindow
{
    Q_OBJECT

public:
    AnimeListQT(QWidget *parent = Q_NULLPTR);
    void setup();
    enum Columns {
        Title,
        Status,
        Date
    };
private slots:
    void add_item();
    void delete_item();
    void edit_item();

    void enable_deletion(QTreeWidgetItem*);

    void action_save();
    void action_open(bool);
    void action_about();
private:
    Ui::AnimeListQTClass ui;
    const QString version{ "v.0.01" };
    QString m_path;
    QTreeWidgetItem* m_item;
};
