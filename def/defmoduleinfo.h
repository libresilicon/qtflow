#ifndef DEFMODULEINFO_H
#define DEFMODULEINFO_H

#include <QString>

namespace def {
	class DEFModuleInfo {
	public:
		DEFModuleInfo();
		QString macro_name;
		QString instance_name;
		double x, y;
	};
}

#endif // DEFMODULEINFO_H
