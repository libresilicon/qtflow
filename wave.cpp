#include "wave.h"
#include "ui_wave.h"

#include "vcd_parser/location.hh"
#include "vcd/vcd_loader.hpp"

Wave::Wave(QWidget *parent) :
	QDockWidget(parent),
    ui(new Ui::Wave),
	scene(new QGraphicsScene),
	tree(NULL),
	signalTree(NULL),
	signalViewTree(NULL)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

	ui->treeSelectionView->setDragEnabled(true);
	ui->treeSelectionView->setAcceptDrops(true);
	ui->treeSelectionView->setDropIndicatorShown(true);

	ui->treeSignalView->setDragEnabled(true);
	ui->treeSignalView->setAcceptDrops(true);
	ui->treeSignalView->setDropIndicatorShown(true);

	//ui->treeSelectionView->setDragEnabled(true);
	//ui->treeSelectionView->setDragDropMode(QAbstractItemView::DragOnly);
	//ui->treeSelectionView->setDefaultDropAction(Qt::CopyAction);

	//ui->treeSignalView->setAcceptDrops(true);
	//ui->treeSignalView->setDragDropMode(QAbstractItemView::DragDrop);
	//ui->treeSignalView->setDefaultDropAction(Qt::MoveAction);
	//ui->treeSignalView->setDragDropOverwriteMode(true);
	//ui->treeSignalView->setItemDelegate(new VcdListDelegate(ui->treeSignalView));

    ui->graphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    scene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

	connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(onSelectScope(QModelIndex)));
	connect(ui->treeSelectionView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onSelectSignal(QModelIndex)));
}

Wave::~Wave()
{
    delete ui;
    delete scene;
}

void Wave::loadVcd(QString path)
{
	QFile file(path);

	if (file.exists()) {
		vcd::Loader loader;
		if(loader.load(path.toStdString())) {
			vcd_data = loader.get_vcd_data();

			if(tree) delete tree;
			tree = new VcdTreeModel(vcd_data);
			ui->treeView->setModel(tree);
			signalTree = new VcdSignalTreeModel(vcd_data,QVector<QString>(),this);
			ui->treeSelectionView->setModel(signalTree);
			signalViewTree = new VcdSignalViewTreeModel(vcd_data,QVector<QString>(),this);
			ui->treeSignalView->setModel(signalViewTree);

			//ui->listView->setModel(list);
			//connect(list, SIGNAL(signalsChanged()), this, SLOT(onSignalsChanged()));
			scene->clear();
		}
	}
}

void Wave::onSignalsChanged()
{
    scene->clear();
    drawSignals();
}

void Wave::onSelectScope(QModelIndex i)
{
	QVector<QString> par;
	while(i.isValid()) {
		par.prepend(i.data().toString());
		i=i.parent();
	}
	if(signalTree) delete signalTree;
	signalTree = new VcdSignalTreeModel(vcd_data,par,this);
	ui->treeSelectionView->setModel(signalTree);
}

void Wave::onSelectSignal(QModelIndex i)
{
	signalViewFilter.append(i.data().toString());
	if(signalViewTree) delete signalViewTree;
	signalViewTree = new VcdSignalViewTreeModel(vcd_data,signalViewFilter,this);
	ui->treeSignalView->setModel(signalViewTree);
}

void Wave::drawSignals()
{
	//QList<int> sig = list->getSignals();
	//for (int i = 0; i < sig.size(); ++i)
	//{
		/*vcd_changes_t::iterator it;
        vcd_changes_t changes = tree->getValues(sig.at(i));

        int high_ = 1;
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
		int h = i * ui->dockWidgetContents->height();
		double hs = ui->dockWidgetContents->height() / high_;
		double ws = tree->vcd().timescale * ui->dockWidgetContents->height() / 1000000;
		int margin = ui->dockWidgetContents->width() / 10;
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
				scene->addLine(time * ws - margin, h + margin, time * ws + margin, h + ui->dockWidgetContents->height() - margin, green);
				scene->addLine(time * ws + margin, h + ui->dockWidgetContents->height() - margin, it->first * ws - margin, h + ui->dockWidgetContents->height() - margin, green);
				scene->addLine(time * ws - margin, h + ui->dockWidgetContents->height() - margin, time * ws + margin, h + margin, green);

                QGraphicsTextItem* ann = new QGraphicsTextItem;
                ann->setPos(time * ws, h);
                ann->setDefaultTextColor(QColor(Qt::white));
                ann->setPlainText(QString::number(it->second));

                scene->addItem(ann);
            }

            time = it->first;
            state = it->second;
		}*/
	//}
}
