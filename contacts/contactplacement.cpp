#include "contactplacement.h"

ContactPlacement::ContactPlacement(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ContactPlacement),
	scene(NULL),
	view(NULL),
	m_tableComplete(false)
{
	ui->setupUi(this);
	connect(ui->tablePins,SIGNAL(cellChanged(int,int)),this,SLOT(updatePreview()));
	view = new ContactsView(ui->framePreview);
	ui->framePreview->layout()->addWidget(view);
	scene = new QGraphicsScene(view);
	view->setScene(scene);
	//scene->setSceneRect(0,0,400,400);
	m_baseRect = QRectF(0,0,400,400);
}

void ContactPlacement::setProject(Project *p)
{
	project = p;
	refreshTables();
}

void ContactPlacement::on_buttonBox_accepted()
{
	QTableWidgetItem* item;
	QString pinName;
	QString side;
	QComboBox* sideSelection;
	QMap<QString,QStringList> sides;

	if(project && m_tableComplete) {
		scene->clear();
		sides.clear();
		for(int i=0; i<ui->tablePins->rowCount(); i++) {
			item = ui->tablePins->item(i,0);
			pinName = item->text();
			sideSelection = (QComboBox*)ui->tablePins->cellWidget(i, 3);
			if(sideSelection) {
				side = sideSelection->currentData().toString();
				sides[side].append(pinName);
			}
		}

		foreach(QString k, sides.keys()) {
			qDebug() << "Storing";
			project->setPadSide(k,sides[k]);
		}
	}
}

void ContactPlacement::updatePreview()
{
	QGraphicsRectItem* rect;
	QGraphicsRectItem* pad;
	QGraphicsSimpleTextItem* padLabel;
	qreal x,y,w,h;

	QTableWidgetItem* item;
	QString pinName;
	QString side;
	QComboBox* sideSelection;
	QMap<QString,QStringList> sides;
	qreal longestSide = 0;

	if(scene && m_tableComplete) {
		scene->clear();
		sides.clear();
		for(int i=0; i<ui->tablePins->rowCount(); i++) {
			item = ui->tablePins->item(i,0);
			pinName = item->text();
			sideSelection = (QComboBox*)ui->tablePins->cellWidget(i, 3);
			if(sideSelection) {
				side = sideSelection->currentData().toString();
				sides[side].append(pinName);
			}
		}

		foreach(QString k, sides.keys()) {
			if(longestSide<sides[k].count()) longestSide = sides[k].count();
		}

		// drawing the data:
		rect = new QGraphicsRectItem(m_baseRect);

		w = m_baseRect.width();
		w /= 2;
		w /= longestSide;

		h = m_baseRect.width();
		h /= longestSide;

		foreach(QString k, sides.keys()) {
			y=0;
			x=0;

			if(k=="T") {
				x=(longestSide-sides["T"].count())/2;
				y=-h;
			}
			if(k=="B") {
				x=(longestSide-sides["B"].count())/2;
				y=m_baseRect.width();
			}
			if(k=="L") {
				x=-h;
				y=(longestSide-sides["L"].count())/2;
			}
			if(k=="R") {
				x=m_baseRect.width();
				y=(longestSide-sides["R"].count())/2;
			}
			foreach(QString p, sides[k]) {
				if((k=="T")||(k=="B")) {
					pad = new QGraphicsRectItem(x*(w*2+(w/sides[k].count())),y,w,h,rect);
					padLabel = new QGraphicsSimpleTextItem(p,pad);
					padLabel->setScale(2*h/padLabel->boundingRect().width());
					padLabel->setRotation(90);
					padLabel->setPos(x*(w*2+(w/sides[k].count())),y);
					x++;
				}
				if((k=="L")||(k=="R")) {
					pad = new QGraphicsRectItem(x,y*(w*2+(w/sides[k].count())),h,w,rect);
					padLabel = new QGraphicsSimpleTextItem(p,pad);
					padLabel->setScale(2*h/padLabel->boundingRect().width());
					padLabel->setPos(x,y*(w*2+(w/sides[k].count())));
					y++;
				}
			}
		}

		// left clamps
		pad = new QGraphicsRectItem(-h,-h,h,h,rect);
		pad = new QGraphicsRectItem(-h,m_baseRect.width(),h,h,rect);

		// right clamps
		pad = new QGraphicsRectItem(m_baseRect.width(),-h,h,h,rect);
		pad = new QGraphicsRectItem(m_baseRect.width(),m_baseRect.width(),h,h,rect);

		scene->addItem(rect);
		scene->update();
	}
}

void ContactPlacement::refreshTables()
{
	QStringList m_TableHeader;
	QTableWidgetItem *w;
	QStringList pins;
	QString blifPath;
	QString pinName;
	QComboBox* sideSelection;
	blif::BLIFData* blifdata;
	int lastCount = 0;

	ui->tablePins->clear();

	QMap<QString,QString> sides;
	sides["T"] = "Top";
	sides["B"] = "Bottom";
	sides["L"] = "Left";
	sides["R"] = "Right";

	QMap<QString,QStringList> sidesMapping;
	foreach(QString k, sides.keys())
		sidesMapping[k] = project->getPadSide(k);

	m_TableHeader << "Name";
	m_TableHeader << "Direction";
	m_TableHeader << "Pad cell";
	m_TableHeader << "Side";
	m_TableHeader << "Order";

	ui->tablePins->setColumnCount(m_TableHeader.count());
	ui->tablePins->setHorizontalHeaderLabels(m_TableHeader);

	if(project) {
		blifPath = QDir(project->getSynthesisDir()).filePath(project->getTopLevel()+".blif");
		if(QFileInfo(blifPath).exists()) {
			blifdata = new blif::BLIFData(blifPath);
			pins = blifdata->getPadPinsInput();
			ui->tablePins->setRowCount(ui->tablePins->rowCount()+pins.count());
			for(int i = 0; i<pins.count();i++) {
				pinName = pins.at(i);
				w = new QTableWidgetItem(pinName);
				w->setFlags(w->flags()&~Qt::ItemIsEditable);
				ui->tablePins->setItem(i+lastCount, 0, w); // signal pin
				w = new QTableWidgetItem("INPUT");
				w->setFlags(w->flags()&~Qt::ItemIsEditable);
				ui->tablePins->setItem(i+lastCount, 1, w); // direction INPUT
				w = new QTableWidgetItem(project->getInPutPadCell());
				w->setFlags(w->flags()&~Qt::ItemIsEditable);
				ui->tablePins->setItem(i+lastCount, 2, w); // direction INPUT

				sideSelection = new QComboBox(ui->tablePins);

				foreach(QString k, sides.keys())
					sideSelection->addItem(sides[k],k);

				connect(sideSelection,SIGNAL(currentTextChanged(QString)),this,SLOT(updatePreview()));
				ui->tablePins->setCellWidget(i+lastCount, 3, sideSelection);

				sideSelection->setCurrentIndex(0);
				foreach(QString k, sidesMapping.keys())
					if(sidesMapping[k].contains(pinName)) {
						qDebug() << "Side defined " << k << " " << pinName;
						sideSelection->setCurrentText(sides[k]);
					}
			}

			lastCount = pins.count();
			pins = blifdata->getPadPinsOutput();
			ui->tablePins->setRowCount(ui->tablePins->rowCount()+pins.count());
			for(int i = 0; i<pins.count();i++) {
				pinName = pins.at(i);
				w = new QTableWidgetItem(pinName);
				w->setFlags(w->flags()&~Qt::ItemIsEditable);
				ui->tablePins->setItem(i+lastCount, 0, w); // signal pin
				w = new QTableWidgetItem("OUTPUT");
				w->setFlags(w->flags()&~Qt::ItemIsEditable);
				ui->tablePins->setItem(i+lastCount, 1, w); // direction OUTPUT
				w = new QTableWidgetItem(project->getOutPutPadCell());
				w->setFlags(w->flags()&~Qt::ItemIsEditable);
				ui->tablePins->setItem(i+lastCount, 2, w); // direction OUTPUT

				sideSelection = new QComboBox(ui->tablePins);
				foreach(QString k, sides.keys())
					sideSelection->addItem(sides[k],k);

				connect(sideSelection,SIGNAL(currentTextChanged(QString)),this,SLOT(updatePreview()));
				ui->tablePins->setCellWidget(i+lastCount, 3, sideSelection);

				sideSelection->setCurrentIndex(0);
				foreach(QString k, sidesMapping.keys()) {
					if(sidesMapping[k].contains(pinName)) {
						qDebug() << "Side defined " << k << " " << pinName;
						sideSelection->setCurrentText(sides[k]);
					}
				}
			}

			m_tableComplete = true;
			updatePreview();
		}
	}
}
