#include "wave.h"
#include "ui_wave.h"

#include "vcd_parser/location.hh"
#include "vcd/vcd_loader.hpp"

Wave::Wave(QWidget *parent) :
	QDockWidget(parent),
    ui(new Ui::Wave),
	scene(new QGraphicsScene),
	tree(NULL),
	list(NULL),
	signalTree(NULL)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    ui->treeView->setDragEnabled(true);
    ui->treeView->setDragDropMode(QAbstractItemView::DragOnly);
    ui->treeView->setDefaultDropAction(Qt::CopyAction);
    ui->listView->setAcceptDrops(true);
    ui->listView->setDragDropMode(QAbstractItemView::DragDrop);
    ui->listView->setDefaultDropAction(Qt::MoveAction);
    ui->listView->setDragDropOverwriteMode(true);

    ui->listView->setItemDelegate(new VcdListDelegate(ui->listView));

    ui->graphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    scene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

	connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(onSelectScope(QModelIndex)));
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
			if(tree) delete tree;
			tree = new VcdTreeModel(loader.get_vcd_data());
			ui->treeView->setModel(tree);
			signalTree = new VcdSignalTreeModel(loader.get_vcd_data());
			ui->treeSelectionView->setModel(signalTree);

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
	QStringList par;
	while(i.isValid()) {
		par.prepend(i.data().toString());
		i=i.parent();
	}
	if(signalTree) {
		signalTree->showSignals(par);
		ui->treeSelectionView->setModel(signalTree);
	}
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
