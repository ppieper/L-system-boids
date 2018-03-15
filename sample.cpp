#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <string>
#include <iostream>

#include "modelerglobals.h"

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView 
{
public:
    SampleModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) 
	{ 
		m_framerate = 40 * (40/(int (VAL(FRAME_RATE) + 0.5)));
	}

    virtual void draw();
	void setColor(int);
	std::string generateGrammar(int);
private:
	std::string m_rules;
	int m_r_depth;
	int m_framerate;
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
	// only draw every ((VAL(FRAME_RATE)) frames
	if (!m_framerate)
	{
		m_framerate = ((int (VAL(FRAME_RATE) + 0.5)));
		return;
	}
	else
		m_framerate--;

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
		m_r_depth = r_depth; // remember setting for the next draw() call
	}

	// convert color from float to int
	int leaf_color = int (VAL(L_COLOR) + 0.5);
	int branch_color = int (VAL(B_COLOR) + 0.5);

	// draw the floor
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_GHOST_WHITE);
	glPushMatrix();
	glTranslated(-5,0,-5);
	drawBox(10,0.01f,10);
	glPopMatrix();

	// draw the plant model
	//glPushMatrix();
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
				if(!VAL(SYMMETRY))
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
	for(int i = 0; i < r_depth - 1; i++)
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

/** @brief setColor - Set the color of the current model
 *
 * @param int color - the color setting from the control value
 *
 **/
void SampleModel::setColor(int color)
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

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
    controls[HEIGHT] = ModelerControl("Height", .05f, .5f, 0.01f, .2f);
	controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);
	controls[R_DEPTH] = ModelerControl("Recursion Depth", 1, 5, 1, 5);
	controls[B_ANGLE] = ModelerControl("Branch Angle", -100, 135, 0.1f, 45);
	controls[B_BEND_ANGLE] = ModelerControl("Branch Bend Angle", -100, 135, 0.1f, -57);
	controls[SYMMETRY] = ModelerControl("Branch Bend Symmetry", 0, 1, 1, 0);
	controls[S_ANGLE] = ModelerControl("Stem Twist", -90, 90, 0.1f, 0);
	controls[B_COLOR] = ModelerControl("Branch Color", 1, 16, 1, 9);
	controls[L_COLOR] = ModelerControl("Leaf Color", 1, 16, 1, 12);
	controls[B_WIDTH] = ModelerControl("Branch Width", .01f, 0.15f, .01f, .05f);
	controls[L_SIZE] = ModelerControl("Leaf Size", .05f, 0.2f, 0.01f, 0.1f);
	controls[STOCH] = ModelerControl("Stochastic", 0, 1, 1, 0);
    controls[FRAME_RATE] = ModelerControl("Framerate", 40, 0, 1, 40);


    ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}
