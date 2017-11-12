#include "contactplacement.h"

ContactPlacement::ContactPlacement(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ContactPlacement),
	scene(NULL),
	view(NULL),
	project(NULL),
	m_padInfo(NULL),
	m_tableComplete(false),
	m_padNames(NULL),
	m_sideLength(NULL),
	m_blifdata(NULL)
{
	ui->setupUi(this);
	view = new ContactsView(ui->framePreview);
	ui->framePreview->layout()->addWidget(view);
	scene = new QGraphicsScene(view);
	view->setScene(scene);
	m_baseRect = QRectF(0,0,400,400);
}

void ContactPlacement::setProject(Project *p)
{
	project = p;
	if(project) {
		m_tableComplete = false;
		if(m_padInfo) delete m_padInfo;
		m_padInfo = new PadInfo(project->getPadInfoFile());
		if(m_blifdata) delete m_blifdata;
		m_blifdata = new blif::BLIFData(QDir(project->getSynthesisDir()).filePath(project->getTopLevel()+".blif"));
		addTables();
		refreshNameTable();
		refreshTables();
		m_tableComplete = true;
		updatePreview();
	}
}

void ContactPlacement::padNames_select_changed(int i)
{
	storeNameTable();
	refreshTables();
	updatePreview();
}

void ContactPlacement::m_padNames_cellChanged(int i, int j)
{
	storeNameTable();
	refreshTables();
	updatePreview();
}

void ContactPlacement::sideLength_changed(QString s)
{
	if(s==QString()) return;
	if(!m_padInfo) return;
	if(s.toDouble()>1) {
		m_padInfo->setSideLength(s.toDouble());
		storeNameTable();
		refreshNameTable();
		refreshTables();
		updatePreview();
	}
}

void ContactPlacement::on_buttonBox_accepted()
{
	if(project && m_tableComplete && m_padInfo) {
		m_padInfo->setSideLength(m_sideLength->text().toDouble());
		storeNameTable();
		m_padInfo->sync();
		//project->buildPadFrame();
	}
}

void ContactPlacement::open()
{
	updatePreview();
	QDialog::open();
}

void ContactPlacement::updatePreview()
{
	if(!m_padInfo) return;

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
	QString padname;

	qreal longestSide;

	if(scene && m_tableComplete && m_padInfo) {
		longestSide = m_padInfo->getSideLength();
		scene->clear();

		// drawing the data:
		rect = new QGraphicsRectItem(m_baseRect);

		w = m_baseRect.width();
		w /= longestSide;

		h = w*2;

		QStringList sides;
		sides << "T";
		sides << "B";
		sides << "L";
		sides << "R";

		foreach(QString k, sides) {
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
			for(int j=0;j<longestSide;j++) {
				padname=k+QString::number(j+1);
				if((k=="T")||(k=="B")) {
					pad = new QGraphicsRectItem(x*w,y,w,h,rect);

					padLabel = new QGraphicsSimpleTextItem(padname,pad);
					rw = padLabel->boundingRect().width();
					rh = padLabel->boundingRect().height();
					scale = h/rw;
					if(scale*rh>w) {
						scale = w/rh;
					}
					scale /= 2;
					padLabel->setScale(scale);
					padLabel->setRotation(90);
					padLabel->setPos(x*w+w,y);

					//padLabel = new QGraphicsSimpleTextItem("Test",pad);
					padLabel = new QGraphicsSimpleTextItem(m_padInfo->getPadName(padname),pad);
					rw = padLabel->boundingRect().width();
					rh = padLabel->boundingRect().height();
					scale = h/rw;
					if(scale*rh>w) {
						scale = w/rh;
					}
					scale/=2;
					padLabel->setScale(scale);
					padLabel->setRotation(90);
					padLabel->setPos(x*w+w-rh*scale,y);

					x++;
				}
				if((k=="L")||(k=="R")) {
					pad = new QGraphicsRectItem(x,y*w,h,w,rect);

					padLabel = new QGraphicsSimpleTextItem(padname,pad);
					rw = padLabel->boundingRect().width();
					rh = padLabel->boundingRect().height();
					scale = h/rw;
					if(scale*rh>w) {
						scale = w/rh;
					}
					scale/=2;
					padLabel->setScale(scale);
					padLabel->setPos(x,y*w);

					//padLabel = new QGraphicsSimpleTextItem("Test",pad);
					padLabel = new QGraphicsSimpleTextItem(m_padInfo->getPadName(padname),pad);
					rw = padLabel->boundingRect().width();
					rh = padLabel->boundingRect().height();
					scale = h/rw;
					if(scale*rh>w) {
						scale = w/rh;
					}
					scale/=2;
					padLabel->setScale(scale);
					padLabel->setPos(x,y*w+rh);

					y++;
				}
				pen = pad->pen();
				pen.setWidth(padBorder);
				pad->setPen(pen);
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

void ContactPlacement::addTables()
{
	if(!m_padInfo) return;

	QWidget* qgb;
	QWidget* fields;
	QLabel* label;
	QTableWidget* tbl;
	QVBoxLayout* lay;
	QHBoxLayout* lay2;
	QToolBox* tb;
	if(project) {
		tb = new QToolBox(ui->padTables);
		ui->padTables->layout()->addWidget(tb);

		// adding dimension parameters
		qgb = new QWidget(tb);
		tb->addItem(qgb,"Basic values");
		lay = new QVBoxLayout(qgb);
		qgb->setLayout(lay);

		fields = new QWidget(qgb);
		m_sideLength = new QLineEdit(fields);
		m_sideLength->setText(QString::number(m_padInfo->getSideLength()));
		connect(m_sideLength,SIGNAL(textChanged(QString)),this,SLOT(sideLength_changed(QString)));
		lay2 = new QHBoxLayout(fields);
		label = new QLabel("Side length");
		fields->setLayout(lay2);
		lay2->addWidget(label);
		lay2->addWidget(m_sideLength);

		m_padNames = new QTableWidget(qgb);
		connect(m_padNames,SIGNAL(cellChanged(int,int)),this,SLOT(m_padNames_cellChanged(int,int)));

		lay->addWidget(fields);
		lay->addWidget(m_padNames);

		foreach(QString s, project->getPadCells()) {
			qgb = new QWidget(tb);
			tb->addItem(qgb,s);
			lay = new QVBoxLayout(qgb);
			qgb->setLayout(lay);
			tbl = new QTableWidget(qgb);
			lay->addWidget(tbl);
			m_tables[s]=tbl;
			tbl->setRowCount(1);
		}
	}
}

void ContactPlacement::storeNameTable()
{
	if(!m_padInfo) return;

	QTableWidgetItem* w;
	QComboBox* typeSelection;
	QString padName;
	QString padSignalName;
	QString cellName;
	if(m_padNames && m_padInfo) {
		for(int i=0; i < m_padNames->rowCount(); i++) {
			w = m_padNames->item(i,0);
			if(w) {
				padName = w->text();
				typeSelection = (QComboBox*)m_padNames->cellWidget(i, 2);
				if(typeSelection) {
					cellName = typeSelection->currentText();
					m_padInfo->setPadCell(padName,cellName);
				}
				w = m_padNames->item(i,1);
				if(w) {
					padSignalName = w->text();
					m_padInfo->setPadName(padName,padSignalName);
				}
			}
		}
		m_padInfo->sync();
	}
}

void ContactPlacement::refreshNameTable()
{
	if(!m_padInfo) return;

	QMap<QString,QString> sides;
	QTableWidgetItem* w;
	QStringList m_TableHeader;
	QComboBox* typeSelection;
	QString padCellName;

	sides["T"] = "Top";
	sides["B"] = "Bottom";
	sides["L"] = "Left";
	sides["R"] = "Right";

	m_TableHeader.clear();
	m_TableHeader << "Pad";
	m_TableHeader << "Name";
	m_TableHeader << "Type";

	m_padNames->clear();
	m_padNames->setRowCount(4*m_padInfo->getSideLength());
	m_padNames->setColumnCount(m_TableHeader.count());
	m_padNames->setHorizontalHeaderLabels(m_TableHeader);

	int i=0;
	foreach(QString bank, sides.keys()) {
		for(int j=0;j<m_padInfo->getSideLength();j++) {
			w = new QTableWidgetItem(bank+QString::number(j+1));
			w->setFlags(w->flags()&~Qt::ItemIsEditable);
			m_padNames->setItem(i, 0, w); // signal pin

			w = new QTableWidgetItem(m_padInfo->getPadName(bank+QString::number(j+1)));
			m_padNames->setItem(i, 1, w); // signal pin

			typeSelection = new QComboBox(m_padNames);
			connect(typeSelection,SIGNAL(currentIndexChanged(int)),this,SLOT(padNames_select_changed(int)));

			foreach(QString k, project->getPadCells())
				typeSelection->addItem(k);

			padCellName = m_padInfo->getPadCell(bank+QString::number(j+1));
			typeSelection->setCurrentText(padCellName);

			connect(typeSelection,SIGNAL(currentTextChanged(QString)),this,SLOT(updatePreview()));
			m_padNames->setCellWidget(i, 2, typeSelection);

			i++;
		}
	}

}

void ContactPlacement::refreshTables()
{
	if(!m_padInfo) return;

	lef::LEFMacro* m;
	QComboBox* signalBox;
	QTableWidgetItem* w;
	QTableWidget* table;
	QString cellType;
	QString pad;
	QStringList m_TableHeader;
	int i;
	QStringList cells;
	QStringList sides;
	sides << "T" << "B" << "L" << "R";
	QMap<QString,QStringList> cellMapping;
	if(m_padNames && m_padInfo) {
		for(i=0; i<m_padInfo->getSideLength(); i++) {
			foreach(QString p, sides) {
				p+=QString::number(i+1);
				cellMapping[m_padInfo->getPadCell(p)].append(p) ;
			}
		}

		foreach(cellType, m_tables.keys()) {
			table = m_tables[cellType];

			table->clear();
			m_TableHeader.clear();

			i = 0;
			m_TableHeader << "Pad";
			i++;
			m_TableHeader << "Name";
			i++;

			m = project->getMacro(cellType);
			if(m) {
				foreach(QString pin, m->getPinNames()) {
					m_TableHeader << pin;
					m_tablesPinMapping[cellType][pin]=i;
					i++;
				}
			}

			cells = cellMapping[cellType];
			table->setColumnCount(m_TableHeader.count());
			table->setHorizontalHeaderLabels(m_TableHeader);
			table->setRowCount(cells.count());

			i = 0;
			int j = 0;
			foreach(pad, cells) {
				w = new QTableWidgetItem(pad);
				w->setFlags(w->flags()&~Qt::ItemIsEditable);
				table->setItem(i, 0, w);
				if(m_blifdata) {
					foreach(QString pin, m_tablesPinMapping[cellType].keys()) {
						signalBox = new QComboBox(table);
						foreach(QString k, m_blifdata->getPadPinsInput())
							signalBox->addItem(k);
						foreach(QString k, m_blifdata->getPadPinsOutput())
							signalBox->addItem(k);
						table->setCellWidget(i, m_tablesPinMapping[cellType][pin], signalBox);
					}
				}
				i++;
			}
		}
	}
}
