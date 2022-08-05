#ifndef STATE_CPP
#define STATE_CPP

struct state
{
public:
	float X_position = 0;
	float Y_position = 0;
	float Z_position = 0;

	float X_velocity = 0;
	float Y_velocity = 0;
	float Z_velocity = 0;

	float X_acceleration = 0;
	float Y_acceleration = 0;
	float Z_acceleration = 0;
		
	float X_angular_position = 0;
	float Y_angular_position = 0;
	float Z_angular_position = 0;

	float X_angular_velocity = 0;
	float Y_angular_velocity = 0;
	float Z_angular_velocity = 0;

	float X_angular_acceleration = 0;
	float Y_angular_acceleration = 0;
	float Z_angular_acceleration = 0;
};

#endif // !STATE_CPP