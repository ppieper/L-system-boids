#include "boids.h"

using namespace std;

// event variables
bool wind_active = false;
int wind_timer = 0;
double wind_speed;

/** @brief initializeBoids - Initialize our boids at random positions
 *
 * @param vector<Boid*> boids
 * @returns vector<Boid*> - a vector containing pointers to the initialized boids
 *
 **/
vector<Boid*> initializeBoids(vector<Boid*> boids, int num_boids) 
{
	for(int i = 0; i < num_boids; i++)
	{
		boids.push_back(new Boid(getRandomPositionVector(), getRandomVelocityVector()));
	}
	return boids;
}

/** @brief moveBoids - Move our boids according to the rules
 *
 * @param vector<Boid*> boids
 *
 **/
void moveBoids(vector<Boid*> boids) 
{
	Vec3d v1 = Vec3d();
	Vec3d v2 = Vec3d();
	Vec3d v3 = Vec3d();
	Vec3d v4 = Vec3d();
	Vec3d v5 = Vec3d();
	Vec3d v6 = Vec3d();

	// handle wind in a separate function
	handleWind();

	for(Boid* b : boids)
	{
		v1 = b->flyTowardsCenterOfMass(boids);
		v2 = b->keepDistance(boids);
		v3 = b->matchVelocity(boids);
		v4 = b->flyTowardsPlant();
		v5 = b->straightenPath();
		b->perch();

		// this will cause birds to 'perch' on the ground for a short time
		if (b->perching)
		{
			if(b->getPerchTimer() > 0)
			{
				b->decrPerchTimer();
				continue;
			}
			else 
				b->perching = false;
		}

		// this will create periodic gusts of wind that last a short time
		if(wind_active)
		{
			if(wind_timer > 0)
				v6 = b->addWind();
		}

		b->setVelocity(b->getVelocity() + v1 + v2 + v3 + v4 + v5 + v6);
		b->setPosition(b->getPosition() + b->getVelocity());
		b->boundPosition();
		b->limitVelocity();

	}
	// decrement wind counter
	if (wind_active && wind_timer > 0)
		wind_timer--;
}

/** @brief drawBoids - Draw all of our boids in the sample model space
 *
 * @param vector<Boid*> boids
 *
 **/
void drawBoids(vector<Boid*> boids) 
{
	Vec3d boid_pos = Vec3d();
	int boid_color = int (VAL(BOID_COLOR) + 0.5);
	for(Boid* b : boids)
	{
		glPushMatrix();
			boid_pos = b->getPosition();
			glTranslated(boid_pos[0], boid_pos[1], boid_pos[2]);
			drawSphere(BOID_SIZE);	
			// show direction of velocity with a line, if setting is on
			if(VAL(SHOW_DIR))
			{
				setDiffuseColor(COLOR_RED);
				b->drawDirectionLine();
				setColor(boid_color);
			}
		glPopMatrix();
	}
}

/** @brief Boid::centerOfMass - Rule 1 - boids try to fly towards
 *                              the center of mass of neighboring boids
 *                              (cohesion)
 *
 * @param vector<Boid*> boids
 * @returns Vec3d - a vector that when added to the boid's velocity, incrementally moves the boid
 *                  towards neighbors' centers of mass
 *                - or, a zero vector if there are no neighbors
 *
 **/
Vec3d Boid::flyTowardsCenterOfMass(vector<Boid*> boids)
{
	Vec3d center = Vec3d();
	int boids_nearby = 0;
	// find visible neighbors' center of mass
	for(Boid* b : boids)
	{
		if(b != this)
		{
			if(b->isNoticed(this))
			{
				center = center + b->getPosition();
				boids_nearby++;
			}
		}
	}
	// no neighbors nearby, return zero vector
	if(center.iszero())
		return center;

	// otherwise, start incrementally moving boid towards center of mass
	center = center / boids_nearby;
	return (center - this->getPosition()) / 100.0;
}

/** @brief Boid::keepDistance - Rule 2 - boids try to keep a small distance
 *                              away from other boids
 *                              (separation)
 *
 * @param vector<Boid*> boids
 * @returns Vec3d - a vector that when added to the boid's velocity, moves it away from 
 *                  neighboring boids
 *                - or, a zero vector if there are no neighbors
 *
 **/
Vec3d Boid::keepDistance(vector<Boid*> boids)
{
	Vec3d c = Vec3d();
	// look at visible neighbors and sum the distances between them and this boid
	for(Boid* b : boids)
	{
		if(b != this)
		{
			if(b->isNoticed(this))
			{
			if((b->getPosition() - this->getPosition()).length() < (VAL(FLOCK_D)))
				c = c - (b->getPosition() - this->getPosition());
			}
		}
	}
	// smooth the movement a bit
	return c*VAL(FLOCK_SPEED);
}

/** @brief Boid::matchVelocity - Rule 3 - boids try to match velocity with nearby boids
 *                               (alignment)
 *
 * @param vector<Boid*> boids
 * @returns Vec3d - a vector that when added to the boid's velocity, slowly matches its velocity 
 *                  to that of neighboring boids
 *                - or, a zero vector if there are no neighbors
 *
 **/
Vec3d Boid::matchVelocity(vector<Boid*> boids)
{
	Vec3d velocity = Vec3d();
	int boids_nearby = 0;
	// add up visible neighbors' velocities
	for(Boid* b : boids)
	{
		if(b != this)
		{
			if(b->isNoticed(this))
			{
				velocity = velocity + b->getVelocity();
				boids_nearby++;
			}
			else continue;
		}
	}
	// no neighbors nearby, return zero vector
	if(velocity.iszero()) 
		return velocity;
	// otherwise, calculate average velocity slowly match it
	velocity = velocity / (boids_nearby);
	return (velocity - this->getVelocity()) / 8.0;
}

/** @brief Boid::flyTowardsPlant - This will cause the boid to fly towards the plant in the center
 *
 * @param vector<Boid*> boids
 * @return Vec3d - a vector that when added to the boid's velocity incrementally moves the boid
 *                  towards the plant in the center of the screen
 *
 **/
Vec3d Boid::flyTowardsPlant()
{
	if(VAL(CIRCLE_PLANT))
	{
		Vec3d place = Vec3d(0.0, 3.0, 0.0);
		return (place - this->getPosition()) / 180;
	}
	else return Vec3d();
}

/** @brief Boid::straightenPath - This slightly straightens the path the boid is currently on-
 *                                used in conjunction with flyTowardsPlant() to create a more rounded
 *                                path
 *
 * @param vector<Boid*> boids
 * @return Vec3d - a vector that when added to the boid's velocity makes their path a bit straighter
 *
 **/
Vec3d Boid::straightenPath()
{
	if(VAL(CIRCLE_PLANT))
	{
		Vec3d velocity = this->getVelocity();
		velocity.normalize(); 
		velocity = velocity * VAL(FLOCK_SPEED)/10;
		return velocity;
	}
	else return Vec3d();
}

/** @brief Boid::perch - Try and 'perch' our boid on the ground if they are close 
 *                       enough, and not already perching.
 *
 * @param vector<Boid*> boids
 *
 **/
void Boid::perch()
{
	// check that user has perching enabled
	if(VAL(CAN_PERCH))
		{
		// already perching, or too windy to perch
		if(perching || wind_active)
			return;

		// start perching
		Vec3d b_pos = this->getPosition();
		if(b_pos[1] <= 0)
		{
			b_pos[1] = 0;
			this->setPosition(b_pos);
			perching = true;
			this->m_perch_time = PERCH_TIME;
		}
	}
}

/** @brief Boid::addWind - add some 'wind' to our boids
 *
 * @return Vec3d - a wind vector if there is wind to be added, or a zero vector if not
 *
 **/
Vec3d Boid::addWind()
{
	Vec3d wind = Vec3d();
	if(wind_active && VAL(ADD_WIND))
		wind = Vec3d(wind_speed,0,wind_speed);
	return wind;
}

/** @brief handleWind - Simulate intermittent gusts of wind (if enabled)
 *
 **/
void handleWind()
{	
	if(VAL(ADD_WIND))
	{
		// randomly create a gust every now and then
		if(!wind_active)
		{
			if((rand() % 50) == 42)
			{
				wind_active = true;
				wind_timer = WIND_TIME;
			}
		}
		// get a speed for the current gust
		if(wind_active)
			 wind_speed = getRandomSpeed(WIND_SPEED);

		// turn the wind off if the timer reaches 0
		if(!wind_timer )
			wind_active = false;
	}
}

/** @brief Boid::boundPosition - Bound our boids to a certain range by making
 *                               them change direction and velocity when they reach
 *                               a border
 *
 * @param vector<Boid*> boids
 *
 **/
void Boid::boundPosition() 
{
	double X_MIN, X_MAX, Y_MIN, Y_MAX, Z_MIN, Z_MAX, BOUNCE_V;
	// the boundaries for each axis
	X_MIN = -(VAL(FLOCK_RANGE));
	X_MAX = VAL(FLOCK_RANGE);
	Y_MIN = 0.0;
	Y_MAX = VAL(FLOCK_RANGE);
	Z_MIN = -(VAL(FLOCK_RANGE));
	Z_MAX = VAL(FLOCK_RANGE);
	// the bounce velocity = user boids velocity setting
	BOUNCE_V = VAL(FLOCK_SPEED);

	Vec3d b_pos = this->getPosition();
	Vec3d b_v = this->getVelocity();

	if (b_pos[0] < X_MIN)
		b_v[0] = BOUNCE_V;
	else if (b_pos[0] > X_MAX)
		b_v[0] = -BOUNCE_V;
	if (b_pos[1] < Y_MIN)
		b_v[1] = BOUNCE_V;
	else if (b_pos[1] > Y_MAX)
		b_v[1] = -BOUNCE_V;
	if (b_pos[2] < Z_MIN)
		b_v[2] = BOUNCE_V;
	else if (b_pos[2] > Z_MAX)
		b_v[2] = -BOUNCE_V;
	m_velocity = b_v;
}

/** @brief Boid::limitVelocity - Keep velocity within a certain limit
 *
 * @param vector<Boid*> boids
 *
 **/
void Boid::limitVelocity()
{
	double LIMIT = VAL(FLOCK_SPEED);
	Vec3d b_v = this->getVelocity();
	if(b_v.length() > LIMIT)
	{
	    b_v.normalize();
		b_v = b_v * LIMIT;
		m_velocity = b_v;
	}
}

/** @brief Boid::drawDirectionLine - Draw a red line indicating the current velocity
 *                                   vector for the direction the boid is moving in
 *
 * @param vector<Boid*> boids
 *
 **/
void Boid::drawDirectionLine()
{
	Vec3d normal = m_velocity;
	normal.normalize(); // get a unit vector
	glPushMatrix();
	    // scale so we start from the origin of the boid
		glScaled(.1,.1,.1);
		glBegin( GL_LINES );
		// draw our red line
		glVertex3d(normal[0]*3.5,normal[1]*3.5,normal[2]*3.5);
		glVertex3d(normal[0],normal[1],normal[2]);
		glEnd();
	glPopMatrix();
}

/** @brief Boid::isNoticed - Check that the boid is within perception range
 *
 * @param Boid* b
 * @returns true if within perception range, false if not
 *
 **/
bool Boid::isNoticed(Boid* b)
{
	Vec3d distance = (b->getPosition() - this->getPosition());
	if(distance.length() > VAL(PERCEPTION))
		return false;
	else 
		return true;
}