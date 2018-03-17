#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values 
// of the controls from the user interface.
enum SampleModelControls
{ 
	XPOS, YPOS, ZPOS, HEIGHT, ROTATE, R_DEPTH, B_ANGLE,  B_BEND_ANGLE, SYMMETRY, 
	S_ANGLE, B_COLOR, L_COLOR, B_WIDTH, L_SIZE, STOCH, SHOW_DIR, PERCEPTION,
	FLOCK_D, ADD_WIND, CIRCLE_PLANT, FLOCK_RANGE, FLOCK_SPEED, BOID_COLOR, CAN_PERCH, 
	FRAMERATE, ALT_PLANT, NUMCONTROLS
};

// Colors
#define COLOR_RED		      1.0f, 0.0f, 0.0f
#define COLOR_GREEN		      0.0f, 1.0f, 0.0f
#define COLOR_BLUE		      0.0f, 0.0f, 1.0f
#define COLOR_PLUM            0.867f, 0.627f, 0.867f
#define COLOR_FOREST_GREEN    0.133f, 0.545f, 0.133f
#define COLOR_TURQUOISE       0.251f, 0.878f, 0.816f
#define COLOR_GOLDENROD       0.855f, 0.647f, 0.125f
#define COLOR_WHITE_ROSE      1.000f, 0.894f, 0.882f
#define COLOR_PURPLE_BLUE     0.482f, 0.408f, 0.933f
#define COLOR_SIENNA          0.627f, 0.322f, 0.176f
#define COLOR_LAVENDER        0.902f, 0.902f, 0.980f
#define COLOR_HOT_PINK        1.000f, 0.412f, 0.706f
#define COLOR_CORAL           1.000f, 0.498f, 0.314f
#define COLOR_GHOST_WHITE     0.973f, 0.973f, 1.000f
#define COLOR_MAROON          0.502f, 0.000f, 0.000f
#define COLOR_ROYAL_BLUE      0.255f, 0.412f, 0.882f
#define COLOR_MIDNIGHT_BLUE   0.098f, 0.098f, 0.439f
#define COLOR_OLIVE           0.502f, 0.502f, 0.000f
#define COLOR_GOLD            1.000f, 0.843f, 0.000f
#define COLOR_SPRING_GREEN    0.000f, 1.000f, 0.498f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif