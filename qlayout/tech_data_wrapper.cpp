#include "tech_data_wrapper.hpp"

#include <typeinfo>

TechDataWrapper::TechDataWrapper(QString s)
{
	std::string layer_name;
	tempFile = tmp.filePath("tech.toml");
	QFile::copy(s, tempFile);
	QFileInfo d(tempFile);
	if(d.exists()) {
		pr = toml::parse(d.absoluteFilePath().toStdString());
	} else {
		qDebug() << d.absoluteFilePath() << " doesn't exist";
	}
}

QStringList TechDataWrapper::getLayers()
{
	toml::Table t;
	toml::Array l;
	QStringList ret;
	l = toml::get<toml::Array>(pr.at("layer"));
	for(toml::value i : l) {
		t = toml::get<toml::Table>(i);
		auto lv = t["name"];
		ret << toml::get<std::string>(lv).c_str();
	}
	return ret;
}

QColor TechDataWrapper::getLayerColor(QString s)
{
	toml::Table t;
	toml::Array l;
	l = toml::get<toml::Array>(pr.at("layer"));
	for(toml::value i : l) {
		t = toml::get<toml::Table>(i);
		auto lv = t["name"];
		if(QString(toml::get<std::string>(lv).c_str())==s) {
			auto lc = t["color"];
			auto lcc = toml::get<toml::Array>(lc);
			if(toml::get<std::string>(lcc[0])=="RGB") {
				return QColor(QString(toml::get<std::string>(lcc[1]).c_str()));
			}
		}
	}
	return QColor(Qt::black);
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
