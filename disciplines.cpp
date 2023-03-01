#include "disciplines.h"
#include "ui_disciplines.h"

#include <QAbstractButton>
#include <QPushButton>
#include "mainwindow.h"

Disciplines::Disciplines(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Disciplines)
{
    ui->setupUi(this);
    connect(ui->disciplineButtonGroup1, &QButtonGroup::buttonClicked, this, &Disciplines::dynamicRemoveDots);
    connect(ui->disciplineButtonGroup1, &QButtonGroup::buttonClicked, this, &Disciplines::dynamicPowersCreation);

    connect(ui->lineEdit_25, &QLineEdit::editingFinished, this, &Disciplines::lineEditHandling);
}

Disciplines::~Disciplines()
{
    for(auto wid: listOfDysciplines)
        wid->deleteLater();
    delete ui;
}


void Disciplines::dynamicPowersCreation(QAbstractButton *bt)
{
//    QLayout *lay = bt->parentWidget()->layout()->itemAt(1)->layout();
    int size_ = MainWindow::countDots(bt->group());
    QWidget *disciplineWidget = bt->parentWidget();
    QLayout *lay = disciplineWidget->layout()->itemAt(1)->layout();
    if(lay->count() > size_)
    {
        while(lay->count() != size_)
        {
            lay->itemAt(lay->count() - 1)->widget()->deleteLater();
            lay->removeWidget(lay->itemAt(lay->count() - 1)->widget());
        }
    }
    if(lay->count() < size_)
    {
        while(lay->count() != size_)
        {
            QLineEdit *lineEdit = new QLineEdit();
            lay->addWidget(lineEdit);
            connect(lineEdit, &QLineEdit::editingFinished, this, &Disciplines::lineEditHandling);
        }
    }
}

QWidget *Disciplines::createDysciplineWidget()
{
    QWidget *mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSizeConstraint(ui->Discipline1->layout()->sizeConstraint());
    mainLayout->setAlignment(ui->Discipline1->layout()->alignment());
    mainLayout->setSpacing(ui->Discipline1->layout()->spacing());
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSizeConstraint(ui->Discipline1_0->sizeConstraint());
    horizontalLayout->setAlignment(ui->Discipline1_0->alignment());
    horizontalLayout->setSpacing(ui->Discipline1_0->spacing());
    QCheckBox *checkBox = new QCheckBox(mainWidget);
    checkBox->setStyleSheet(ui->checkBox->styleSheet());
    checkBox->setSizePolicy(ui->checkBox->sizePolicy());
    checkBox->adjustSize();
    QLineEdit *lineEdit = new QLineEdit(mainWidget);
    lineEdit->setSizePolicy(ui->lineEdit_25->sizePolicy());
    lineEdit->setFont(ui->lineEdit_25->font());
    lineEdit->adjustSize();
    horizontalLayout->addWidget(checkBox);
    horizontalLayout->addWidget(lineEdit);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSizeConstraint(ui->DisciplineButtonsLayout_1->sizeConstraint());
    buttonsLayout->setAlignment(ui->DisciplineButtonsLayout_1->alignment());
    buttonsLayout->setSpacing(ui->DisciplineButtonsLayout_1->spacing());
    QButtonGroup *groupBut = new QButtonGroup();
    groupBut->setExclusive(false);
    for(int i = 0; i < 5; ++i)
    {
        QRadioButton *but = new QRadioButton(mainWidget);
        but->setObjectName("Discipline_"+ QString::number(listOfDysciplines.count())+"_Button" + QString::number(i));
        but->setSizePolicy(ui->disciplineButton1_1->sizePolicy());
        but->setStyleSheet(ui->disciplineButton1_1->styleSheet());
        but->setAutoExclusive(false);
        buttonsLayout->insertWidget(i, but);
        groupBut->addButton(but,i);
        but->adjustSize();
    }
    horizontalLayout->addLayout(buttonsLayout);

    QVBoxLayout *powersLayout = new QVBoxLayout();
    powersLayout->setSizeConstraint(ui->DisciplinePowers1->sizeConstraint());
    powersLayout->setAlignment(ui->DisciplinePowers1->alignment());
    powersLayout->setSpacing(ui->DisciplinePowers1->spacing());

    mainLayout->addLayout(horizontalLayout);
    mainLayout->addLayout(powersLayout);

    connect(groupBut, &QButtonGroup::buttonClicked, this, &Disciplines::dynamicRemoveDots);
    connect(groupBut, &QButtonGroup::buttonClicked, this, &Disciplines::dynamicPowersCreation);
    connect(lineEdit, &QLineEdit::editingFinished, this, &Disciplines::lineEditHandling);

    mainWidget->adjustSize();
    return mainWidget;
}

void Disciplines::dynamicRemoveDots(QAbstractButton *bt)//naprawienie sposobem tasmy klejacej
{
    if(bt->group()->id(bt) < -1)//automatycznie
    {
        if(bt->isChecked())
        {
            for(int i = 0; i < bt->group()->buttons().size(); i++)
            {
                if(bt->group()->buttons().at(i)->objectName() == bt->objectName())
                    break;
                bt->group()->buttons().at(i)->setChecked(true);
            }
        }
        else
        {
            int del = 0;
            for(int i = 0; i < bt->group()->buttons().size(); i++)
            {
                if(bt->group()->buttons().at(i)->objectName() == bt->objectName())
                {
                    del = i;
                    break;
                }
            }
            for(int i = del; i < bt->group()->buttons().size(); i++)
            {
                bt->group()->buttons().at(i)->setChecked(false);
            }
        }
    }
    else if(bt->group()->id(bt) == -1)
    {
        qDebug() << "COS POSZLO NIE TAK";
        return;
    }
    else//recznie przydzielione
    {
        qDebug() << "RECZNIE";
        if(bt->isChecked())
        {
            for(int i = 0; i < bt->group()->buttons().size(); i++)
            {
                if(bt->group()->button(i)->objectName() == bt->objectName())
                    break;
                bt->group()->button(i)->setChecked(true);
            }
        }
        else
        {
            int del = 0;
            for(int i = 0; i < bt->group()->buttons().size(); i++)
            {
                if(bt->group()->button(i)->objectName() == bt->objectName())
                {
                    del = i;
                    break;
                }
            }
            for(int i = del; i < bt->group()->buttons().size(); i++)
            {
                bt->group()->button(i)->setChecked(false);
            }
        }
    }
}


void Disciplines::on_addNewWidgetButton_clicked()
{
    QWidget *newDiscipline = createDysciplineWidget();
    listOfDysciplines.append(newDiscipline);
    QVBoxLayout *layout = static_cast<QVBoxLayout *>(this->layout());
    layout->insertWidget(layout->count() - 2, newDiscipline);
}


void Disciplines::on_deleteWidgetButton_clicked()
{
    if(!listOfDysciplines.isEmpty())
    {
        listOfDysciplines.back()->deleteLater();
        listOfDysciplines.pop_back();
    }
}


void Disciplines::on_lockButton_toggled(bool checked)
{
    if(checked)
    {
        ui->lockButton->setIcon(QIcon(QPixmap(":/images/icons/lock20x20.png")));
    }
    else
    {
        ui->lockButton->setIcon(QIcon(QPixmap(":/images/icons/unlock20x20.png")));
    }

    QList<QRadioButton *> allRadioButtons = this->findChildren<QRadioButton *>();
    QList<QLineEdit *> allLineEdits = this->findChildren<QLineEdit *>();
    for(auto line: allLineEdits)
    {
        line->setEnabled(!checked);
    }
    for(auto but: allRadioButtons)
    {
        but->setEnabled(!checked);
    }
    ui->addNewWidgetButton->setEnabled(!checked);
    ui->deleteWidgetButton->setEnabled(!checked);
}

void Disciplines::lineEditHandling()
{
    QLineEdit *line = static_cast<QLineEdit *>(sender());
    line->clearFocus();
}
