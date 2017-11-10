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

		project->buildPadFrame();
	}
}

void ContactPlacement::open()
{
	updatePreview();
	QDialog::open();
}

void ContactPlacement::updatePreview()
{
	QPen pen;
	QGraphicsRectItem* rect;
	QGraphicsLineItem* line;
	QGraphicsRectItem* pad;
	QGraphicsSimpleTextItem* padLabel;
	qreal x,y,w,h;
	qreal countT, countB, countL, countR;
	qreal padBorder = 1;
	qreal scale;
	qreal rw,rh;

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
		w /= longestSide;

		h = w*2;

		countT = sides["T"].count();
		countB = sides["B"].count();
		countL = sides["L"].count();
		countR = sides["R"].count();

		countT = countT?countT:1;
		countB = countB?countB:1;
		countL = countL?countL:1;
		countR = countR?countR:1;

		foreach(QString k, sides.keys()) {
			if(k=="T") {
				x=0;
				y=-h;
			}
			if(k=="B") {
				x=0;
				y=m_baseRect.width();
			}
			if(k=="L") {
				x=-h;
				y=0;
			}
			if(k=="R") {
				x=m_baseRect.width();
				y=0;
			}
			foreach(QString p, sides[k]) {
				if((k=="T")||(k=="B")) {
					pad = new QGraphicsRectItem(x*w,y,w,h,rect);
					padLabel = new QGraphicsSimpleTextItem(p,pad);
					rw = padLabel->boundingRect().width();
					rh = padLabel->boundingRect().height();
					scale = h/rw;
					if(scale*rh>w) scale = w/rh;
					padLabel->setScale(scale);
					padLabel->setRotation(90);
					padLabel->setPos(x*w+w,y);
					x++;
				}
				if((k=="L")||(k=="R")) {
					pad = new QGraphicsRectItem(x,y*w,h,w,rect);
					padLabel = new QGraphicsSimpleTextItem(p,pad);
					rw = padLabel->boundingRect().width();
					rh = padLabel->boundingRect().height();
					scale = h/rw;
					if(scale*rh>w) scale = w/rh;
					padLabel->setScale(scale);
					padLabel->setPos(x,y*w);
					padLabel->setPos(x,y*w);
					y++;
				}
				pen = pad->pen();
				pen.setWidth(padBorder);
				pad->setPen(pen);
			}

			if((k=="T")||(k=="B")) {
				for(;x<longestSide;x++) {
					pad = new QGraphicsRectItem(x*w,y,w,h,rect);
					pad->pen();
					line = new QGraphicsLineItem(x*w,y,x*w+w,y+h,pad);
					line = new QGraphicsLineItem(x*w+w,y,x*w,y+h,pad);

					pen = pad->pen();
					pen.setWidth(padBorder);
					pad->setPen(pen);
				}
			}
			if((k=="L")||(k=="R")) {
				for(;y<longestSide;y++) {
					pad = new QGraphicsRectItem(x,y*w,h,w,rect);
					line = new QGraphicsLineItem(x,y*w,x+h,y*w+w,pad);
					line = new QGraphicsLineItem(x+h,y*w,x,y*w+w,pad);

					pen = pad->pen();
					pen.setWidth(padBorder);
					pad->setPen(pen);
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

		if(!QFileInfo(blifPath).exists()) // generate BLIF file
			project->synthesis();

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
