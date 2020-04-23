#include "Buff.h"

Buff::Buff(Tank* _owner, buff_type type, int lasting)
	:owner(_owner), myType(type),end_time(cur_time + lasting) {
	switch (type)
	{
	case Buff::invincible:
		old_value = owner->set_hp(0x7fffffff);
		break;
	case Buff::addHP:
		old_value = owner->get_hp() + 1;
		old_value = owner->set_hp(0x7fffffff);
		end_time = cur_time + 100;
		break;
	case Buff::power:
		old_value = owner->set_attack(owner->get_attack() + 1);
		break;
	case Buff::acceleration:
		old_value = owner->set_interval(owner->get_interval() - 1);
		break;
	default:
		break;
	}
}

Buff::~Buff(){
	switch (myType)
	{
	case Buff::invincible:
		owner->set_hp(old_value);
		break;
	case Buff::addHP:
		owner->set_hp(old_value);
		break;
	case Buff::power:
		owner->set_attack(old_value);
		break;
	case Buff::acceleration:
		owner->set_interval(old_value);
		break;
	default:
		break;
	}
}

bool Buff::timeout() const{
	return cur_time == end_time;
}

xy Buff::owner_position() const{
	return owner->position();
}

Buff::buff_type Buff::get_type() const {
	return myType;
}

Tank* Buff::get_owner() const {
	return owner;
}