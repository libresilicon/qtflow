#include "wave.h"
#include "ui_wave.h"

#include "vcd_parser/location.hh"
#include "vcd/vcd_loader.hpp"

Wave::Wave(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::Wave),
	scene(new QGraphicsScene),
	tree(NULL),
	signalTree(NULL)
{
	ui->setupUi(this);

	signalView = new VcdSignalView(ui->viewerWidget);
	ui->viewerWidget->layout()->addWidget(signalView);

	ui->treeSelectionView->setDragDropMode(QAbstractItemView::DragDrop);
	ui->treeSelectionView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui->treeSelectionView->setDefaultDropAction(Qt::MoveAction);
	ui->treeSelectionView->setDragEnabled(true);
	ui->treeSelectionView->setAcceptDrops(true);
	ui->treeSelectionView->setDropIndicatorShown(true);

	/*ui->treeSignalView->setDragDropMode(QAbstractItemView::DragDrop);
	ui->treeSignalView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui->treeSignalView->setDefaultDropAction(Qt::MoveAction);
	ui->treeSignalView->setDragEnabled(true);
	ui->treeSignalView->setAcceptDrops(true);
	ui->treeSignalView->setDropIndicatorShown(true);*/

	//ui->treeSelectionView->setDragEnabled(true);
	//ui->treeSelectionView->setDragDropMode(QAbstractItemView::DragOnly);
	//ui->treeSelectionView->setDefaultDropAction(Qt::CopyAction);

	//ui->treeSignalView->setAcceptDrops(true);
	//ui->treeSignalView->setDragDropOverwriteMode(true);
	//ui->treeSignalView->setItemDelegate(new VcdListDelegate(ui->treeSignalView));

	//ui->graphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
	scene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
	//ui->graphicsView->setScene(scene);

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
			if(signalTree) delete signalTree;
			signalTree = new VcdSignalTreeModel(vcd_data,QVector<QString>(),this);
			ui->treeSelectionView->setModel(signalTree);

			signalView->setVCD(vcd_data);
			//if(signalViewTree) delete signalViewTree;
			//signalViewTree = new VcdSignalViewTreeModel(vcd_data,QVector<QString>(),this);
			//ui->treeSignalView->setModel(signalViewTree);

			//ui->listView->setModel(list);
			//connect(list, SIGNAL(signalsChanged()), this, SLOT(onSignalsChanged()));
			scene->clear();
		}
	}
}

void Wave::onSignalsChanged()
{
    scene->clear();
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
	signalView->append(i.data().toString());
}

