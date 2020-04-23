#include"Object.h"
#pragma once
class Buff
{
public:
	enum buff_type {none,invincible,addHP, power, acceleration};
	Buff(Tank* _owner,buff_type type,int lasting);//give buff to 
	~Buff();
	bool timeout() const;
	Tank* get_owner() const;
	xy owner_position() const;
	buff_type get_type() const;
private:
	Tank* owner;
	buff_type myType;
	int end_time;
	int old_value;
};
extern int cur_time;
