#ifndef VCD_DATA_H
#define VCD_DATA_H

#include <QStack>
#include <QHash>

#include <fstream>
#include <istream>
#include <iostream>

namespace vcd {
	class TimeValue;
	class VCDScanner;
	class VCDParser;
	class Header;
	typedef std::vector<TimeValue> TimeValues;
	class VCData
	{
	public:
		VCData(QString s);
		void newHeader(const Header new_header, std::vector<Var> new_vars, TimeValues new_time_values);
		TimeValues getTimeValues();
		std::vector<std::string> getCurrentScope();

		int getChangeCount();
		void incrementChanges();

		class VCDScanner *getLexer();

	private:
		QString streamname;
		VCDScanner *lexer;
		VCDParser *parser;
		bool trace_scanning;
		bool trace_parsing;
		bool rootSet;
		std::vector<std::string> current_scope_;
	};
}

#endif
