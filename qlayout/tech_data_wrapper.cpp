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
