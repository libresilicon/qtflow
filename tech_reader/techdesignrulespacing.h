#ifndef TECHDESIGNRULESPACING_H
#define TECHDESIGNRULESPACING_H

#include <QString>

class TechDesignRuleSpacing
{
public:
	TechDesignRuleSpacing();
	void setTouchingOK(bool);
	void setMaterial(QString m);
	void setSpacing(int i);
	int getSpacing();
	bool getTouchingOK();
	QString getMaterial();
private:
	bool b_touchingOK;
	int m_spacing;
	QString m_material;
};

#endif // TECHDESIGNRULESPACING_H
