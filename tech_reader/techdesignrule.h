#ifndef TECHDESIGNRULE_H
#define TECHDESIGNRULE_H

#include <QString>

class TechDesignRule
{
public:
	TechDesignRule();
	void setName(QString);
	void setMinimumWidth(int i);
	void setWidthMessage(QString m);

private:
	int m_minWidth;
	QString m_name;
	QString m_widthMessage;
};

#endif // TECHDESIGNRULE_H
