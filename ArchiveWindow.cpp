#include "ArchiveWindow.hpp"
#include "ui_ArchiveWindow.h"
#include "ArchiveItem.hpp"
#include <QMessageBox>

const int ArchiveWindow::WindowWidth = 1024;
const int ArchiveWindow::WindowHeight = 576;

ArchiveWindow::ArchiveWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ArchiveWindow)
{
    ui->setupUi(this);

    this -> setFixedSize(WindowWidth, WindowHeight);
    this -> setWindowTitle("The Pig Farm");
    this -> setWindowIcon(QIcon(":/Resources/Picture/PigFace.png"));

    ui -> pushButton_create -> setFont(QFont("Minecraft", 15));
    ui -> pushButton_delete -> setFont(QFont("Minecraft", 15));
    ui -> pushButton_enter -> setFont(QFont("Minecraft", 15));

    create_window = new CreateWindow(this);

    ConfigueArchiveUpdate();
    // Update once at first.
    file_manager -> StatisticsExistingArchives();
    ui -> pushButton_enter -> setDisabled(true);
    ui -> pushButton_delete -> setDisabled(true);
    connect(ui -> listWidget_archive_display, MyListWidget::ItemSelected, this, [ = ](bool selected)
    {
        ui -> pushButton_enter -> setDisabled(!selected);
        ui -> pushButton_delete -> setDisabled(!selected);
    });
    ConfigueButtonEnter();
    connect(ui -> pushButton_back, QPushButton::clicked, this, [ = ]()
    {
        this -> hide();
        emit Back();
    });
}
ArchiveWindow::~ArchiveWindow()
{
    delete ui;
}

void ArchiveWindow::Start()
{

    this -> show();
}

void ArchiveWindow::CreateArchive()
{
    create_window -> show();
}

void ArchiveWindow::ConfigueArchiveUpdate()
{
    // Confirm the `current_existing_archive` is up-to-date when creating the archive.
    connect(ui -> pushButton_create, QPushButton::clicked, this, [ = ]()
    {
        create_window -> SetCurrntExsistingArchiveList(current_existing_archive);
        create_window -> Start();
    });
    // Receive the current existing archive list.
    connect(file_manager, FileManager::SendExistingArchivesList, this, [ = ](QVector<QString> result)
    {
        this -> current_existing_archive = result;
        emit RequestAllArchiveInfo();
    });
    // Update the current existing archive list when new archive is created.
    connect(file_manager, FileManager::NewArchiveCreated, file_manager, FileManager::StatisticsExistingArchives);
    connect(this, ArchiveWindow::RequestAllArchiveInfo, file_manager, FileManager::ReadAllArchiveData);
    connect(file_manager, FileManager::SendAllArchiveInfo, this, UpdateArchiveItem);
    connect(this, ArchiveWindow::RequestDeleteArchive, file_manager, FileManager::DeleteArchive);
    connect(this, ArchiveWindow::RequestDeleteArchive, file_manager, FileManager::StatisticsExistingArchives);
    connect(ui -> pushButton_delete, QPushButton::clicked, this, [ = ]()
    {
        QList<QListWidgetItem*> list = ui -> listWidget_archive_display -> selectedItems();

        if (list.length() > 1)
        {
            QMessageBox::warning(this, "!", QString("<font face = Minecraft size = 3>Please only select one archive!</font>"));
        }
        else if (list.length() == 1)
        {
            QMessageBox::StandardButton choice =
                QMessageBox::question(this, "Comfirm", QString("<font face = Minecraft size = 3>Are you sure to delete this archive permanently?</font>"),
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

            if (choice == QMessageBox::Yes)
            {
                emit RequestDeleteArchive((list.at(0) -> data(Qt::UserRole)).toString());
                delete list.at(0);
                UpdateButtonEnable();
            }
        }
    });
}

void ArchiveWindow::UpdateArchiveItem(QVector<FileManager::GameData> data)
{
    current_existing_archive_data = data;
    ui -> listWidget_archive_display -> clear();

    for (int i = 0; i < data.length(); i++)
    {
        ArchiveItem * archive_item = new ArchiveItem;
        archive_item -> SetEditDate(data[i].edit_time);
        archive_item -> SetArchiveName(data[i].name);
        archive_item -> SetDifficultyDisplay(data[i].difficulty);
        QListWidgetItem * ListWidgetItem_temp = new QListWidgetItem;
        ListWidgetItem_temp -> setSizeHint(archive_item -> size());
        ListWidgetItem_temp -> setData(Qt::UserRole, data[i].name);
        ui -> listWidget_archive_display -> addItem(ListWidgetItem_temp);
        ui -> listWidget_archive_display -> setItemWidget(ListWidgetItem_temp, archive_item);
    }
}

void ArchiveWindow::ConfigueButtonEnter()
{
    connect(ui -> pushButton_enter, QPushButton::clicked, this, [ = ]()
    {
        QList<QListWidgetItem*> list = ui -> listWidget_archive_display -> selectedItems();

        if (list.length() == 1)
        {
            this -> hide();
            emit GameStart((list.at(0) -> data(Qt::UserRole)).toString());
        }
        else
        {
            // pass;
        }
    });

}

void ArchiveWindow::mousePressEvent(QMouseEvent *event)
{
    QMainWindow::mousePressEvent(event);

    if (!(ui -> listWidget_archive_display -> underMouse()))
    {
        ui -> listWidget_archive_display -> clearSelection();
    }

    auto flag = (ui -> listWidget_archive_display -> selectedItems()).length() == 0;
    ui -> pushButton_enter -> setDisabled(flag);
    ui -> pushButton_delete -> setDisabled(flag);

}

void ArchiveWindow::UpdateButtonEnable()
{
    if ((ui -> listWidget_archive_display -> selectedItems()).length() == 0)
    {
        ui -> pushButton_enter -> setDisabled(true);
        ui -> pushButton_delete -> setDisabled(true);
    }
}
