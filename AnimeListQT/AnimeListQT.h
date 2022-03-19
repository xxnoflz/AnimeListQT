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
#include <QComboBox>

#include "aboutQT.h"
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
signals:
    void items_changed();
private slots:
    void add_item();
    void delete_item();

    void enable_deletion(int);

    void action_save();
    void action_open(bool);
    void action_about();

    void change_checker(int,int);
    void change_handler();
private:
    Ui::AnimeListQTClass ui;
    const QString version{ "v.0.01" };
    QString m_path;
    int m_row;
};
