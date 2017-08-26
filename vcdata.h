#ifndef VCDATA_H
#define VCDATA_H

#include <QStack>
#include <QHash>

#include "vcdscanner.h"

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
