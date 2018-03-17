#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "boids.h"
#include <FL/gl.h>
#include <string>

#include "modelerglobals.h"

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView 
{
public:
    SampleModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) 
	{ 
		m_framerate = 20;
	}

    virtual void draw();
	std::string generateGrammar(int);
	std::string generateAltGrammar(int);
private:
	std::string m_rules;
	std::string m_alt_rules;
	int m_r_depth;
	double m_framerate;
	std::vector<Boid*> m_boids;
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{ 
    return new SampleModel(x,y,w,h,label); 
}

/** @brief SampleModel::draw() - Overridden draw call; draw our sample model
 *
 **/
void SampleModel::draw()
{
	// if low-fps mode is enabled
	if(VAL(FRAMERATE))
	{
		//skip every VAL(FRAMERATE) frames
		if (m_framerate != 0)
		{
			m_framerate--;
			return;
		}
		else
			m_framerate = 20;
	}

    // This call takes care of a lot of the nasty projection 
    // matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
    ModelerView::draw();

	// generate the grammar rules based on our recursion depth setting
	// (only need to do this if the settings have changed)
	int r_depth = int (VAL(R_DEPTH) + 0.5);
	if(r_depth != m_r_depth)
	{
		m_rules = generateGrammar(r_depth); 
		m_alt_rules = generateAltGrammar(r_depth);
		m_r_depth = r_depth; // remember setting for the next draw() call
	}

	// convert color from float to int
	int leaf_color = int (VAL(L_COLOR) + 0.5);
	int branch_color = int (VAL(B_COLOR) + 0.5);
	int boid_color = int (VAL(BOID_COLOR) + 0.5);

	// initialize our boids if we haven't already
	if(m_boids.empty())
		m_boids = initializeBoids(m_boids, 8);

	// move & draw the boids
	glPushMatrix();
		setColor(boid_color);
		moveBoids(m_boids);
		drawBoids(m_boids);
		setColor(branch_color);
	glPopMatrix();

	// draw the floor
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_GHOST_WHITE);
	glPushMatrix();
		glTranslated(-5,0,-5);
		drawBox(10,0.01f,10);
	glPopMatrix();

	// draw the plant model
	glPushMatrix();
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
	glRotated(VAL(ROTATE), 0.0, 1.0, 0.0);
	glRotated(-90, 1.0, 0.0, 0.0);
	setColor(branch_color);

	// we draw the plant model based on our grammar string
	for(unsigned int i = 0; i < m_rules.length(); i++)
	{
		switch(m_rules[i])
		{
			// draw a branch with a leaf
			case '0':
				drawCylinder(VAL(HEIGHT), VAL(B_WIDTH), VAL(B_WIDTH)); 
				glTranslated(0, 0, VAL(HEIGHT));
				setColor(leaf_color);
				drawSphere(VAL(L_SIZE)); 
				setColor(branch_color);
				break;
			// draw a segment of the main 'stem'
			case '1': 
				glRotated(VAL(S_ANGLE), 0.0, 1.0, 1.0); 
				drawCylinder(VAL(HEIGHT), VAL(B_WIDTH), VAL(B_WIDTH)); 
				glTranslated(0, 0, VAL(HEIGHT));
				break;
			// push and rotate 
			case '[': 
				glPushMatrix();
				if(VAL(STOCH) && (rand() % 2) == 1)
				{
					glRotated(-VAL(B_BEND_ANGLE), 0.0, .33, 0.0);
					glRotated(-VAL(B_ANGLE), 1.0, 0.0, 1.0); 
				}
				else
				{
					glRotated(VAL(B_BEND_ANGLE), 0.0, 1.0, 0.0);
					glRotated(VAL(B_ANGLE), 1.0, 0.0, 1.0); 
				}
				break;
			// pop and rotate the opposite direction
			case ']': 
				glPopMatrix(); 
				if(!VAL(SYMMETRY) && !VAL(STOCH))
					glRotated(VAL(B_BEND_ANGLE), 0.0, 1.0, 0.0);
				if(VAL(STOCH) && (rand() % 2) == 1)
				{
					glRotated(VAL(B_BEND_ANGLE), 0.0, 1.0, 0.0);
					glRotated(VAL(B_ANGLE), 1.0, 0.0, 1.0); 
				}
				else
				{
					glRotated(-VAL(B_BEND_ANGLE), 0.0, 1.0, 0.0);
					glRotated(-VAL(B_ANGLE), 1.0, 0.0, 1.0); 
				}
				break;
			default: break;
		}
	}
	glPopMatrix();
	// draw the other alternate plant as well, if that's enabled
	if (VAL(ALT_PLANT))
	{
		glPushMatrix();
		glTranslated(VAL(XPOS)+2.0, VAL(YPOS), VAL(ZPOS)+2.0);
		glScaled(0.5,0.5,0.5);
		glRotated(VAL(ROTATE), 0.0, 1.0, 0.0);
		glRotated(-90, 1.0, 0.0, 0.0);
		setColor(4);
		// we draw the plant model based on our grammar string
		for(unsigned int i = 0; i < m_alt_rules.length(); i++)
		{
			switch(m_alt_rules[i])
			{
				// draw a branch with a leaf
				case '0':
					drawCylinder(VAL(HEIGHT), VAL(B_WIDTH), VAL(B_WIDTH)); 
					glTranslated(0, 0, VAL(HEIGHT));
					setColor(5);
					drawSphere(VAL(L_SIZE)); 
					setColor(4);
					break;
				// draw a segment of the main 'stem'
				case '1': 
					glRotated(VAL(S_ANGLE), 0.0, 1.0, 1.0); 
					drawCylinder(VAL(HEIGHT), VAL(B_WIDTH), VAL(B_WIDTH)); 
					glTranslated(0, 0, VAL(HEIGHT));
					break;
				// push and rotate 
				case '[': 
					glPushMatrix();
					if(VAL(STOCH) && (rand() % 2) == 1)
					{
						glRotated(-VAL(B_BEND_ANGLE), 0.0, .33, 0.0);
						glRotated(-VAL(B_ANGLE), 1.0, 0.0, 1.0); 
					}
					else
					{
						glRotated(VAL(B_BEND_ANGLE), 0.0, 1.0, 0.0);
						glRotated(VAL(B_ANGLE), 1.0, 0.0, 1.0); 
					}
					break;
				// pop and rotate the opposite direction
				case ']': 
					glPopMatrix(); 
					if(!VAL(SYMMETRY) && !VAL(STOCH))
						glRotated(VAL(B_BEND_ANGLE), 0.0, 1.0, 0.0);
					if(VAL(STOCH) && (rand() % 2) == 1)
					{
						glRotated(VAL(B_BEND_ANGLE), 0.0, 1.0, 0.0);
						glRotated(VAL(B_ANGLE), 1.0, 0.0, 1.0); 
					}
					else
					{
						glRotated(-VAL(B_BEND_ANGLE), 0.0, 1.0, 0.0);
						glRotated(-VAL(B_ANGLE), 1.0, 0.0, 1.0); 
					}
					break;
				default: break;
			}
		}
		glPopMatrix();
	}
}

/** @brief generateGrammar - Generate the grammar for our L-system
 *
 * @param int r_depth - the depth of recursion
 * @return string output - the rules for generating our plant
 *
 **/
std::string SampleModel::generateGrammar(int r_depth)
{
	// our 'seed' value
	std::string axiom = "0";
	// our instructions on how to draw the shape
	std::string output = axiom;
	for(int i = 0; i < r_depth; i++)
	{
		output.clear();
		for(unsigned int j = 0; j < axiom.length(); j++)
		{
			switch(axiom[j])
			{
				case '0': output.append("1[0]1[0]0"); break;
				case '1': output.append("11"); break;
				case '[': output.append("["); break;
				case ']': output.append("]"); break;
				default: break;
			}
		}
		axiom = output;
	}
	return output;
}

/** @brief generateAltGrammar - Generate the alt grammar for our L-system
 *
 * @param int r_depth - the depth of recursion
 * @return string output - the rules for generating our plant
 *
 **/
std::string SampleModel::generateAltGrammar(int r_depth)
{
	// our 'seed' value
	std::string axiom = "0";
	// our instructions on how to draw the shape
	std::string output = axiom;
	for(int i = 0; i < r_depth; i++)
	{
		output.clear();
		for(unsigned int j = 0; j < axiom.length(); j++)
		{
			switch(axiom[j])
			{
				case '0': output.append("1[0]0"); break;
				case '1': output.append("111[10]"); break;
				case '[': output.append("["); break;
				case ']': output.append("]"); break;
				default: break;
			}
		}
		axiom = output;
	}
	return output;
}


int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
	// L-system controls
    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
    controls[HEIGHT] = ModelerControl("Height", .05f, .5f, 0.01f, .2f);
	controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);
	controls[R_DEPTH] = ModelerControl("Recursion Depth", 0, 5, 1, 4);
	controls[B_ANGLE] = ModelerControl("Branch Angle", -100, 135, 0.1f, 45);
	controls[B_BEND_ANGLE] = ModelerControl("Branch Bend Angle", -100, 135, 0.1f, -57);
	controls[SYMMETRY] = ModelerControl("Branch Bend Symmetry", 0, 1, 1, 0);
	controls[S_ANGLE] = ModelerControl("Stem Twist", -90, 90, 0.1f, 0);
	controls[B_COLOR] = ModelerControl("Branch Color", 1, 16, 1, 9);
	controls[L_COLOR] = ModelerControl("Leaf Color", 1, 16, 1, 12);
	controls[B_WIDTH] = ModelerControl("Branch Width", .01f, 0.15f, .01f, .05f);
	controls[L_SIZE] = ModelerControl("Leaf Size", .05f, 0.2f, 0.01f, 0.1f);
	controls[STOCH] = ModelerControl("Stochastic", 0, 1, 1, 0);
	// boids controls
	controls[SHOW_DIR] = ModelerControl("View Boids Direction", 0, 1, 1, 0);
	controls[PERCEPTION] = ModelerControl("Boids Perception Distance", .5f, 4.0f, .01f, 1.35f);
	controls[FLOCK_D] = ModelerControl("Boids Min Distance", .2f, 3.5f, 0.1f, 0.6f);
	controls[ADD_WIND] = ModelerControl("Enable Wind", 0, 1, 1, 0);
	controls[CIRCLE_PLANT] = ModelerControl("Boids Circle Plant", 0, 1, 1, 0);
	controls[BOID_COLOR] = ModelerControl("Boid Color", 1, 16, 1, 1);
	controls[FLOCK_RANGE] = ModelerControl("Boid Boundaries", .5f, 5.0f, .1f, 4.5f);
	controls[FLOCK_SPEED] = ModelerControl("Boid Velocity", .05f, .35f, .01f, .16f);
	controls[CAN_PERCH] = ModelerControl("Enable Perching", 0, 1, 1, 0);
	controls[FRAMERATE] = ModelerControl("Low-FPS Mode", 0, 1, 1, 0);
	controls[ALT_PLANT] = ModelerControl("Generate Alt Plant", 0, 1, 1, 0);


    ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}
