#include "ticket.h"
#include "okno.h"

okno::okno(QWidget *parent) : QWidget(parent){
    this->setStyleSheet("background: #C800C8");
    mainlayout = new QVBoxLayout();

    topwidget = new QWidget();
    toplayout = new QHBoxLayout();
    topwidget->setLayout(toplayout);

    bottomwidget = new QWidget();
    bottomlayout = new QVBoxLayout(bottomwidget);


    topwidget->setStyleSheet("background: gray");
    bottomwidget->setStyleSheet("background: #0000D0");
    mainlayout->addWidget(topwidget);
    mainlayout->addWidget(bottomwidget);

    mainlayout->setStretch(0, 1);
    mainlayout->setStretch(1, 5);


    //toplayout
    QSpinBox *but =  new QSpinBox(this);
    but->setMaximum(50);
    but->setMinimum(0);
    connect(but, &QSpinBox::valueChanged, this, &okno::numtickets);

    QPushButton *prev =  new QPushButton(this);
    QPushButton *next =  new QPushButton(this);
    toplayout->addWidget(but, Qt::AlignLeft);
    toplayout->addWidget(prev);
    toplayout->addWidget(next);

    connect(prev, &QPushButton::clicked, this, &okno::prevbutton);
    connect(next, &QPushButton::clicked, this, &okno::nextbutton);
    but->setStyleSheet("background: yellow");
    prev->setText("предыдущий");
    next->setText("следующий");
    prev->setStyleSheet("background: grey");
    next->setStyleSheet("background: gray");



    //bottomlayout
    list = new QListWidget(this);
    bottomlayout->addWidget(list);
    list->setStyleSheet("background: yellow");
    connect(list, &QListWidget::itemClicked, [&](){tickets[list->currentRow()]->cl(true);});
    connect(list, &QListWidget::itemDoubleClicked, [&](){tickets[list->currentRow()]->dcl();});

    //progressbar
    totalbar = new QProgressBar;
    mainlayout->addWidget(totalbar);
    totalbar->setRange(0, 100);
    totalbar->setValue(42);
    greenbar = new QProgressBar;
    mainlayout->addWidget(greenbar);
    greenbar->setRange(0, 100);
    greenbar->setValue(42);
    greenbar->setStyleSheet("background: green");

    setLayout(mainlayout);
}

void okno::updateticket(int i, QString nm){
    if (nm == "") nm = tickets[i]->name;
    ticketbuttons[i]->setText(nm);
    if(tickets[i]->status == 0) ticketbuttons[i]->setBackground(QBrush(Qt::gray));
    if(tickets[i]->status == 1) ticketbuttons[i]->setBackground(QBrush(Qt::yellow));
    if(tickets[i]->status == 2) ticketbuttons[i]->setBackground(QBrush(Qt::green));
}

void okno::addbilet(int n){
    if (lasttickets.empty() || lasttickets.back()!=n) lasttickets.push_back(n);
    if (lasttickets.size()>4613) lasttickets.pop_front();
}

void okno::updatebars(){
    if (kolbil==0){
        totalbar->setRange(0, 100);
        totalbar->setValue(42);
        greenbar->setRange(0, 100);
        greenbar->setValue(42);
        return;
    }
    int koly=0;
    int kolg=0;
    for (auto u : tickets){
        if (u->status==1) koly++;
        if (u->status==2) kolg++;
    }
    totalbar->setRange(0, kolbil*2);
    totalbar->setValue(koly+kolg*2);
    greenbar->setRange(0, kolbil);
    greenbar->setValue(kolg);
}

void okno::closeEvent(QCloseEvent *event){
    for (ticket* t : tickets) {
        t->close();
        t->deleteLater();
    }
    QWidget::closeEvent(event);
}

void okno::prevbutton(){
    if (lasttickets.size() == 0) return;
    list->setCurrentRow(lasttickets.back());
    if (lasttickets.size()>=2) lasttickets.pop_back();
    tickets[list->currentRow()]->cl(false);
}

void okno::numtickets(int kol){
    if (kolbil == kol) return;
    kolbil = kol;
    lasttickets.clear();
    tickets.clear();
    ticketbuttons.clear();
    list->clear();
    for (int i=0;i<kol;i++){
        tickets.push_back(new ticket(i+1, this, nullptr));
        ticketbuttons.push_back(new QListWidgetItem(list));
        updateticket(i);
    }
    updatebars();
    setLayout(mainlayout);
}

void okno::nextbutton(){
    int rnd;
    bool flag=1;
    for (auto u:tickets) flag&=(u->status==2);
    if (flag) return;
    do {
        rnd = QRandomGenerator::global()->bounded(kolbil);
    } while (tickets[rnd]->status == 2);
    list->setCurrentRow(rnd);
    tickets[list->currentRow()]->cl(true);
}
