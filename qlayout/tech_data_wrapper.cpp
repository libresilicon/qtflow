#include "tech_data_wrapper.hpp"

TechDataWrapper::TechDataWrapper(QString s)
{
	QString errorStr;
	int errorLine;
	int errorColumn;
	QFile file(s);
	if(file.open(QIODevice::ReadOnly)) {
		doc = QDomDocument("xml");
		if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
			qDebug()
			<< "Error: Parse error at line " << errorLine << ", "
			<< "column " << errorColumn << ": "
			<< qPrintable(errorStr);
		}
		file.close();
	}
}

QStringList TechDataWrapper::getLayers()
{
	QStringList ret;
	QDomNode n;
	QDomElement e;
	QDomNodeList ll = doc.elementsByTagName("layer"); // layer list
	for(int i=0; i<ll.count(); i++) {
		n = ll.at(i);
		e = n.toElement().firstChildElement("name"); // name (only one entry by specification and logic!)
		ret << e.text();
	}
	return ret;
}

QColor TechDataWrapper::getLayerColor(QString s)
{
	QString colorType, colorCode;
	QColor ret = QColor(Qt::black);
	QDomNode n;
	QDomElement e;
	QDomNodeList ll = doc.elementsByTagName("layer"); // layer list
	for(int i=0; i<ll.count(); i++) {
		n = ll.at(i);
		e = n.toElement().firstChildElement("name"); // name (only one entry by specification and logic!)
		if(e.text()==s) {
			e = n.toElement().firstChildElement("color").firstChildElement("type"); // type (only one entry by specification and logic!)
			colorType = e.text();
			e = n.toElement().firstChildElement("color").firstChildElement("code"); // type (only one entry by specification and logic!)
			colorCode = e.text();
			if(colorType=="RGB") ret = QColor(colorCode);
			break; // we've found a color. save time.
		}
	}
	return ret;
}

QString TechDataWrapper::getLambdaUnit()
{
	return doc.documentElement().firstChildElement("lambda").firstChildElement("unit").text();
}

qreal TechDataWrapper::getLambdaValue()
{
	qreal ret = 1;
	QString s = doc.documentElement().firstChildElement("lambda").firstChildElement("factor").text();
	if(s.toInt()) {
		ret=s.toInt();
	}
	return ret;
}

QIcon TechDataWrapper::getLayerIcon(QString n)
{
	QPixmap pm;
	QIcon ico;
	pm = QPixmap(100,100);
	pm.fill(getLayerColor(n));
	ico = QIcon(pm);
	return ico;
}
