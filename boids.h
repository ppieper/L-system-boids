#include "modelerdraw.h"
#include "modelerapp.h"
#include "vec.h"
#include <vector>

#include "modelerglobals.h"

const int PERCH_TIME = 100;
const int WIND_TIME = 50;
const double WIND_SPEED = 0.1;
const double BOID_SIZE = 0.10;

class Boid
{
public:
    Boid(Vec3d pos, Vec3d v) 
	{ 
		m_pos = pos;
		m_velocity = v;
		perching = false;
	}

	Vec3d getPosition() const {return m_pos;}
	Vec3d getVelocity() const {return m_velocity;}

	void setPosition(Vec3d pos) {m_pos = pos;}
	void setVelocity(Vec3d v) {m_velocity = v;}

	void boundPosition();
	void limitVelocity();
	void drawDirectionLine();

	bool isNoticed(Boid*);

	Vec3d flyTowardsCenterOfMass(vector<Boid*>);
	Vec3d keepDistance(vector<Boid*>);
	Vec3d matchVelocity(vector<Boid*>);
	Vec3d flyTowardsPlant();
	Vec3d straightenPath();
	Vec3d addWind();
	void perch();

	int getPerchTimer() const {return m_perch_time;}
	void decrPerchTimer() {m_perch_time--;}

	bool perching;

private:
	Vec3d m_pos;
	Vec3d m_velocity;
	int m_perch_time;
};

// these are defined in boids.cpp
extern vector<Boid*> initializeBoids(std::vector<Boid*>, int);
extern void drawBoids(std::vector<Boid*>);
extern void moveBoids(std::vector<Boid*>);
extern double getRandomSpeed(double);
extern void handleWind();

/** @brief setColor - Set the diffuse color of subsequently drawn models
 *
 * @param int color - the color setting from the control value
 *
 **/
static void setColor(int color)
{
	switch(color)
	{
		case 1: setDiffuseColor(COLOR_TURQUOISE); break;
		case 2: setDiffuseColor(COLOR_ROYAL_BLUE); break;
		case 3: setDiffuseColor(COLOR_MIDNIGHT_BLUE); break;
		case 4: setDiffuseColor(COLOR_OLIVE); break;
		case 5: setDiffuseColor(COLOR_PLUM); break;
		case 6: setDiffuseColor(COLOR_FOREST_GREEN); break;
		case 7: setDiffuseColor(COLOR_LAVENDER); break;
		case 8: setDiffuseColor(COLOR_GOLDENROD); break;
		case 9: setDiffuseColor(COLOR_WHITE_ROSE); break;
		case 10: setDiffuseColor(COLOR_LAVENDER); break;
		case 11: setDiffuseColor(COLOR_SIENNA); break;
		case 12: setDiffuseColor(COLOR_HOT_PINK); break;
		case 13: setDiffuseColor(COLOR_CORAL); break;
		case 14: setDiffuseColor(COLOR_MAROON); break;
		case 15: setDiffuseColor(COLOR_GOLD); break;
		case 16: setDiffuseColor(COLOR_SPRING_GREEN); break;
		default: break;
	}
}

/** @brief getRandomVector - Gets a random vector with its x, y and z values somewhere
 *                           in the range of -4.0 to 4.0
 *
 * @return Vec3d - a randomized position vector
 *
 **/
static Vec3d getRandomPositionVector()
{
	Vec3d v = Vec3d(((rand() % 801 + (-400)) / 100.0), 
		            ((rand() % 801 + (-400)) / 100.0), 
					((rand() % 801 + (-400)) / 100.0));
	return v;
}

/** @brief getRandomVelocityVector - Gets a random vector with its x and z values somewhere
 *                                  in the range of -.16 to -.16
 *
 * @return Vec3d - a randomized velocity vector
 *
 *
 **/
static Vec3d getRandomVelocityVector()
{
	Vec3d v = Vec3d(((rand() % 33 + (-16)) / 100.0), 
		            ((rand() % 33 + (-16)) / 100.0), 
					((rand() % 33 + (-16)) / 100.0));
	return v;
}

/** @brief getRandomSpeed - Generate a non-zero speed based on the input
 *                          multiplier in the range (-20 to 20) * (speed/10)
 *
 * @param double speed - the input speed multiplier
 * @return double - a random position speed 
 *
 **/
static double getRandomSpeed(double speed)
{
	double random_speed;
	do {
		random_speed = (rand() % 41 + (-20)) * (speed/10);
	} while(random_speed == 0);
	return random_speed;
}