#include "wave.h"
#include "ui_wave.h"

#include "vcdparser.h"

#include <QDebug>
#include <QFile>
#include <QTreeView>
#include <QListView>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QMimeData>

Wave::Wave(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Wave),
    tree(new VcdTreeModel),
    list(new VcdListModel),
    scene(new QGraphicsScene)
{
    ui->setupUi(this);
    ui->treeView->setModel(tree);
    ui->listView->setModel(list);
    ui->graphicsView->setScene(scene);

    connect(list, SIGNAL(signalsChanged()), this, SLOT(onSignalsChanged()));

    ui->treeView->setDragEnabled(true);
    ui->treeView->setDragDropMode(QAbstractItemView::DragOnly);
    ui->treeView->setDefaultDropAction(Qt::CopyAction);
    ui->listView->setAcceptDrops(true);
    ui->listView->setDragDropMode(QAbstractItemView::DropOnly);
    ui->listView->setDefaultDropAction(Qt::CopyAction);

    ui->listView->setItemDelegate(new VcdListDelegate(ui->listView));

    ui->graphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    scene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
}

Wave::~Wave()
{
    delete ui;
    delete tree;
    delete list;
    delete scene;
}

void Wave::loadVcd(QString path)
{
    QFile file(path);

    if (!file.exists())
        return;

    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray content(file.readAll());
    VcdParser parser(content);

    file.close();

    list->reset();
    tree->setVcd(parser.getVcd());
    scene->clear();
}

void Wave::onSignalsChanged()
{
    scene->clear();
    drawSignals();
}

void Wave::drawSignals()
{
    QList<int> sig = list->getSignals();
    for (int i = 0; i < sig.size(); ++i)
    {
        vcd_changes_t::iterator it;
        vcd_changes_t changes = tree->getValues(sig.at(i));

        int high_ = 0;
        int long_ = 0;
        for (it = changes.begin(); it != changes.end(); ++it)
        {
            if (it->first > long_)
                long_ = it->first;
            if (it->second > high_)
                high_ = it->second;
        }

        int time = 0;
        int state = 0;
        int h = i * WAVE_ITEM_HEIGHT;
        double hs = WAVE_ITEM_HEIGHT / high_;
        double ws = tree->vcd().timescale * WAVE_STRETCH / 1000000;
        int margin = WAVE_STRETCH / 10;
        QPen green(Qt::green);
        for (it = changes.begin(); it != changes.end(); ++it)
        {
            // two state
            if (high_ < 2)
            {
                scene->addLine(time * ws, h + state * hs, it->first * ws, h + state * hs, green);
                scene->addLine(it->first * ws, h + state * hs, it->first * ws, h + it->second * hs, green);
            }
            // more than two states
            else
            {
                scene->addLine(time * ws + margin, h + margin, it->first * ws - margin, h + margin, green);
                scene->addLine(time * ws - margin, h + margin, time * ws + margin, h + WAVE_ITEM_HEIGHT - margin, green);
                scene->addLine(time * ws + margin, h + WAVE_ITEM_HEIGHT - margin, it->first * ws - margin, h + WAVE_ITEM_HEIGHT - margin, green);
                scene->addLine(time * ws - margin, h + WAVE_ITEM_HEIGHT - margin, time * ws + margin, h + margin, green);

                QGraphicsTextItem* ann = new QGraphicsTextItem;
                ann->setPos(time * ws, h);
                ann->setDefaultTextColor(QColor(Qt::white));
                ann->setPlainText(QString::number(it->second));

                scene->addItem(ann);
            }

            time = it->first;
            state = it->second;
        }
    }
}
