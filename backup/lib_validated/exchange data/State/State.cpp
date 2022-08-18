#include "State.h"

State::State()
{
	position = Vector(3);
	velocity = Vector(3);
	acceleration = Vector(3);
	orientation = Quaternion();
	angular_velocity = Vector(3);
	angular_acceleration = Vector(3);

	position.set_description("Position");
	velocity.set_description("Velocity");
	acceleration.set_description("Acceleration");
	orientation.set_description("Orientation");
	angular_velocity.set_description("Angular Velocity");
	angular_acceleration.set_description("Angular Acceleration");
	description = "State";
}

State::~State()
{
}

void State::set_description(string description)
{
	this->description=description;
}

string State::to_str()
{
	string str;
	str+="description:"+description+'\n';
	str+= '\t' + position.to_str()+'\n';
	str+= '\t' + velocity.to_str()+'\n';
	str+= '\t' + acceleration.to_str()+'\n';
	str+= '\t' + orientation.to_str()+'\n';
	str+= '\t' + angular_velocity.to_str()+'\n';
	str+= '\t' + angular_acceleration.to_str()+'\n';
	return str;
}