#include "wave.h"

Wave::Wave(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::Wave),
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

	connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(onSelectScope(QModelIndex)));
	connect(ui->treeSelectionView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onSelectSignal(QModelIndex)));

	connect(ui->pushZoomIn,SIGNAL(clicked(bool)),signalView,SLOT(onZoomIn()));
	connect(ui->pushZoomOut,SIGNAL(clicked(bool)),signalView,SLOT(onZoomOut()));
	connect(ui->pushZoomFitWidth,SIGNAL(clicked(bool)),signalView,SLOT(onZoomFitWidth()));
	//connect(ui->pushMoveLeft,SIGNAL(clicked(bool)),signalView,SLOT(onMoveLeft()));
	//connect(ui->pushMoveRight,SIGNAL(clicked(bool)),signalView,SLOT(onMoveRight()));
}

Wave::~Wave()
{
    delete ui;
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
		}
	}
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

