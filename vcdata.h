#ifndef VCDATA_H
#define VCDATA_H

#include <QStack>
#include <QHash>

#include "vcdscanner.h"

struct vcd_scopes_t
{
	QString name;
	QHash<int, QString> alias;
	QList<vcd_scopes_t> scopes;
};

typedef QVector<QPair<int, int>> vcd_changes_t;

struct vcd_t
{
	QString date;
	QString version;
	int timescale;
	vcd_scopes_t scopes;
	QHash<int, vcd_changes_t> changes;
};

namespace vcd {
	class VCData
	{
	public:
		VCData();
		VCDScanner *getLexer();
		void setDate(std::string*);
		void setVersion(std::string*);
		void addTimescale(std::string*);
		void addScope(std::string*);

	private:
		QStack<vcd_scopes_t*> parsedScopes;
		QStack<QChar> vcdstring;
		vcd_t parsedVcd;
		VCDScanner *lexer;
	};
}

#endif // VCDATA_H
