// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>

#include "modelerglobals.h"
#include "camera.h"

#include "marchingCube.h"
// To make a SampleModel, we inherit off of ModelerView
class myModel : public ModelerView
{
public:
	myModel(int x, int y, int w, int h, char* label)
		: ModelerView(x, y, w, h, label) { }

	virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createMyModel(int x, int y, int w, int h, char* label)
{
	return new myModel(x, y, w, h, label);
}

void drawBody()
{
	// Draw Main Body
	glScaled(1.3, 1.6, 1.23);
	glTranslated(0.0, 1.7, 0.0);
	drawSphere(1);

	// Brown Belly --------------------------------------------------------------------------//
	glPushMatrix();
	// Brown Part - Belly
	setDiffuseColor(0.745, 0.749, 0.290);
	glScaled(0.88, 0.8, 0.8);
	glTranslated(0.0, -0.1, 0.45);
	drawSphere(1.0);
	glPopMatrix();
	//Brown Belly ---------------------------------------------------------------------------//
}

void drawHead(int level)
{
	//------------------------ Head starts --------------------------------------------------------------------------// 
	setDiffuseColor(0.223, 0.219, 0.176);
	glPushMatrix();

	glTranslated(0, 0.65, 0);


	// Draw a Head
	glPushMatrix();
	glScaled(0.75 / 1.3, 1.3 / 1.6, 0.73 / 1.23);
	drawTotoroHead(1.0);
	glPopMatrix();

	glScaled(1.25, 1.2, 1.3);
	glRotated(VAL(HEADROTATE), 0.0, 1.0, 0.0);

	//--------------------------------------------Ear Starts-----------------------------------------------------//
	if (level >= 3) {
		glPushMatrix(); // Right Ear
			glTranslated(-0.3, 0.4, 0.0);
			glRotated(-90, 1.0, 0.0, 0.0);
			glRotated(-10, 0.0, 1.0, 0.0);
			glScaled(0.5, 0.5, 0.7);
			drawCylinder(0.3, 0.1, 0.1);

			glTranslated(0.0, 0.0, 0.2);
			drawCylinder(0.1, 0.1, 0.2);
			glTranslated(0.0, 0.0, 0.1);

			drawCylinder(0.4, 0.2, 0.0);
			glPopMatrix(); // Right ear ends

			glPushMatrix(); //  Left ear
			glTranslated(0.3, 0.4, 0.0);
			glRotated(-90, 1.0, 0.0, 0.0);
			glRotated(10, 0.0, 1.0, 0.0);
			glScaled(0.5, 0.5, 0.7);
			drawCylinder(0.3, 0.1, 0.1);

			glTranslated(0.0, 0.0, 0.2);
			drawCylinder(0.1, 0.1, 0.2);
			glTranslated(0.0, 0.0, 0.1);

			drawCylinder(0.4, 0.2, 0.0);
		glPopMatrix(); // Right ear ends
		//------------------------------------------------Ear Ends--------------------------------------------------//
	}

	if (level >= 4) {	
		//----------------------------------------------- Nose START -----------------------------------------------// 
		glPushMatrix();
		marchingCube* temp = new marchingCube;
			setDiffuseColor(0.223, 0.219, 0.176);
			glTranslated(0.0, 0.2, 0.5);
			glScaled(1.0, 0.5, 0.5);
			glRotated(70, 1.0, 0.0, 0.0);
			drawHalfSphere(0.3);

			// ---------------------- Cat whiskers START ------------------------------------------------------------//
			// Left Whiskers 1
			glPushMatrix();
				setDiffuseColor(0, 0, 0);
				// Draw a line
				glScaled(0.18, 0.1, 0.1);
				glTranslated(0.0, 1.0, 0.0);

				glRotated(90, 0.0, 1.0, 0.0);
				drawCylinder(4.0, 0.1, 0.1);
			glPopMatrix();
			// Left Whiskers 2
			glPushMatrix();
				// Draw a line
				glScaled(0.15, 0.1, 0.1);
				glTranslated(0.0, 1.0, 0.0);

				glRotated(70, 0.0, 1.0, 0.0);
				drawCylinder(4.0, 0.1, 0.1);
			glPopMatrix();
			// Left Whiskers 3
			glPushMatrix();
				// Draw a line
				glScaled(0.15, 0.1, 0.1);
				glTranslated(0.0, 1.0, 0.0);

				glRotated(110, 0.0, 1.0, 0.0);
				drawCylinder(4.0, 0.1, 0.1);
			glPopMatrix();

			// Right Whiskers 1
			glPushMatrix();
				// Draw a line
				glScaled(0.18, 0.1, 0.1);
				glTranslated(0.0, 1.0, 0.0);

				glRotated(-90, 0.0, 1.0, 0.0);
				drawCylinder(4.0, 0.1, 0.1);
			glPopMatrix();

			// Right Whiskers 1
			glPushMatrix();
				// Draw a line
				glScaled(0.15, 0.1, 0.1);
				glTranslated(0.0, 1.0, 0.0);

				glRotated(-70, 0.0, 1.0, 0.0);
				drawCylinder(4.0, 0.1, 0.1);
			glPopMatrix();

			// Right Whiskers 1
			glPushMatrix();
				// Draw a line
				glScaled(0.15, 0.1, 0.1);
				glTranslated(0.0, 1.0, 0.0);

				glRotated(-110, 0.0, 1.0, 0.0);
				drawCylinder(4.0, 0.1, 0.1);
			glPopMatrix();
			// ---------------------- Cat whiskers Ends ------------------------------------------------------------//

			// Nose Hole
			glPushMatrix();
				setDiffuseColor(0.0, 0.0, 0.0);
				glScaled(0.5, 0.5, 0.5);
				glTranslated(0.0, 0.5, -0.2);
				drawSphere(0.13);
			glPopMatrix();

			// Mouth	
			glPushMatrix();
				setDiffuseColor(1.0, 1.0, 1.0);
				glTranslated(0.0, 0.05, 0.3);
				glScaled(0.2, 0.1, 0.2);
				glRotated(-20, 1.0, 0.0, 0.0);
				drawSphere(0.1);
			glPopMatrix();
		glPopMatrix();
		//----------------------------------------------- Nose END --------------------------------------------------// 
	}

	if (level >= 5) {	
		//--------------------------------------Eyes----------------------------------//
		glPushMatrix(); // Left Eye starts
			setDiffuseColor(1.0, 1.0, 1.0);
			glScaled(0.5, 0.4, 0.5);
			glTranslated(0.5, 0.85, 0.6);

			glRotated(70, 1.0, 0.0, 0.0);
			drawHalfSphere(0.2);

			// Left Eye's Pupil
			setDiffuseColor(0.0, 0.0, 0.0);
			glTranslated(0.02, 0.13, 0.0);
			drawSphere(0.08);
		glPopMatrix(); // Left Eye ends

		glPushMatrix(); // Right Eye starts
			setDiffuseColor(1.0, 1.0, 1.0);
			glScaled(0.5, 0.4, 0.5);
			glTranslated(-0.5, 0.85, 0.6);

			glRotated(70, 1.0, 0.0, 0.0);
			drawHalfSphere(0.2);

			// Left Eye's Pupil
			setDiffuseColor(0.0, 0.0, 0.0);
			glTranslated(-0.02, 0.13, 0.0);
			drawSphere(0.08);
		glPopMatrix(); // Right Eye ends
		//--------------------------------------Eyes Ends----------------------------------//
	}
	glPopMatrix();
	//---------------------------- Head END -------------------------------------------------------------------------//
}

void drawLeftLeg() {
	//------------------------ Left Leg starts ----------------------------------------------------------------------//  
	glPushMatrix();
	setDiffuseColor(0.223, 0.219, 0.176);


	//glTranslated(0.8, 0.0, -0.6);
	glScaled(0.77, 0.4, 0.81);


	glTranslated(0.6, -1.5, 0.0);
	//drawHalfSphere(0.5);
	glRotated(90, 1.0, 0.0, 0.0);
	glRotated(-20, 0.0, 1.0, 0.0);

	drawCylinder(1.0, 0.5, 0.5);

	// Left Feet		
	drawSphere(0.5);

	glTranslated(0.0, 0.0, 1.0);
	drawSphere(0.5);


	setDiffuseColor(0, 0, 0);
	glTranslated(0.2, 0.33, 0.25);
	glScaled(0.4, 0.2, 0.5);
	glRotated(-90, 1.0, 0.0, 0.0);
	drawCylinder(1.0, 0.2, 0.05);

	glTranslated(-0.3, 0.0, 0.1);
	drawCylinder(1.0, 0.2, 0.05);

	glTranslated(-0.3, 0.0, -0.1);
	drawCylinder(1.0, 0.2, 0.05);

	glPopMatrix();
	//-------------------------------------------Left Leg ends ------------------------------------------------------------------------// 
}

void drawRightLeg() 
{
	//------------------------------------------- Right Leg Start ----------------------------------------------------------------------//
	glPushMatrix();// Right Leg starts 
	setDiffuseColor(0.223, 0.219, 0.176);
	//glTranslated(0.8, 0.0, -0.6);
	glScaled(0.77, 0.4, 0.81);

	glTranslated(-0.6, -1.5, 0.0);
	//drawHalfSphere(0.5);
	glRotated(90, 1.0, 0.0, 0.0);
	glRotated(20, 0.0, 1.0, 0.0);

	drawCylinder(1.0, 0.5, 0.5);

	// Left Feet		
	drawSphere(0.5);

	glTranslated(0.0, 0.0, 1.0);
	drawSphere(0.5);

	// Feet Claws
	setDiffuseColor(0, 0, 0);
	glTranslated(-0.2, 0.33, 0.25);
	glScaled(0.4, 0.2, 0.5);
	glRotated(-90, 1.0, 0.0, 0.0);
	drawCylinder(1.0, 0.2, 0.05);

	glTranslated(0.3, 0.0, 0.1);
	drawCylinder(1.0, 0.2, 0.05);

	glTranslated(0.3, 0.0, -0.1);
	drawCylinder(1.0, 0.2, 0.05);

	glPopMatrix();
	//-------------------------------------------------Right Leg End ------------------------------------------------------------------------//
}

void drawLeftARM(int level)
{
	//---------------------------------------------------------Left Arm --------------------------------------------------------------------//
	glPushMatrix();
		setDiffuseColor(0.223, 0.219, 0.176);
		glScaled(1 / 1.3, 1 / 1.6, 1 / 1.23);
		//glScaled(0.6, .2, 0.4);
		glTranslated(0.84, 0.5, 0.0);
		drawSphere(0.5);
		//	
		glPushMatrix();
			glScaled(1.0, 1.0, 0.8);
			glRotated(VAL(LEFTARMROTATEY), 0.0, 1.0, 0.0);
			drawCylinder(1.7, 0.5, 0.3);

			glTranslated(0.0, 0.0, 1.7);
			drawSphere(0.3);

			// Claw1 - Thumb
			setDiffuseColor(0, 0, 0);
			glPushMatrix();
				glTranslated(-0.1, 0.15, -0.1);
				glScaled(0.4, 0.5, 1.2);
				glRotated(20, 0.0, 1.0, 0.0);
				glRotated(-20, 0.0, 0.0, 1.0);
				drawCylinder(0.5, 0.2, 0.05);
			glPopMatrix();

			glPushMatrix();
				glTranslated(0.1, 0.15, -0.1);
				glScaled(0.4, 0.5, 1.2);
				glRotated(-15, 0.0, 1.0, 0.0);
				drawCylinder(0.5, 0.2, 0.05);

				glTranslated(0.1, -0.2, 0.0);
				drawCylinder(0.5, 0.2, 0.05);

				glTranslated(0.0, -0.2, 0.0);
				drawCylinder(0.5, 0.2, 0.05);

				glTranslated(-0.1, -0.2, 0.0);
				glRotated(-15, 0.0, 1.0, 0.0);

				drawCylinder(0.5, 0.2, 0.05);
			glPopMatrix();
	
			if (level >= 9) {			
				// Umbrella
				setDiffuseColor(0, 0, 0);
				glPushMatrix();
					glTranslated(-0.1, 0.0, 0.3);
					glRotated(90, 1.0, 0.0, 0.0);
					drawHalfTorus(0.1, 0.05);

					glTranslated(0.1, 0.0, 0.0);
					glRotated(-180, 1.0, 0.0, 0.0);
					drawCylinder(2.8, 0.05, 0.05);

					setDiffuseColor(0.023, 0.035, 0.137);
					glTranslated(0.0, 0.0, 2.2);
					//glScaled(1.0, 1.2, 1.0);
					glRotated(90, 1.0, 0.0, 0.0);
					drawUmbrella(2.0, VAL(UMBRELLA));
				glPopMatrix();
			}

		glPopMatrix();
	glPopMatrix();
	//---------------------------------------------------Left Arm Ends---------------------------------------------------------------------------//
}

void drawRightArm() 
{
	//------------------------------------------------------Right Arm----------------------------------------------------------------------------//
	glPushMatrix();
		setDiffuseColor(0.223, 0.219, 0.176);
		glScaled(1 / 1.3, 1 / 1.6, 1 / 1.23);
		//glScaled(0.6, .2, 0.4);
		glTranslated(-0.83, 0.5, 0.0);
		drawSphere(0.5);

		glPushMatrix();
			glScaled(1.0, 1.0, 0.8);
			glRotated(-90, 0.0, 1.0, 0.0);
			glRotated(-90, 0.0, 0.0, 1.0);
			glRotated(VAL(RIGHTARMROTATE), 0.0, 1.0, 0.0);
			drawCylinder(1.7, 0.5, 0.3);

			glTranslated(0.0, 0.0, 1.7);
			drawSphere(0.3);

			// Claw1 - Thumb
			setDiffuseColor(0, 0, 0);
			glPushMatrix();
				glTranslated(0.1, 0.15, -0.2);
				glScaled(0.4, 0.5, 1.2);
				glRotated(20, 0.0, 1.0, 0.0);
				glRotated(-20, 0.0, 0.0, 1.0);
				drawCylinder(0.5, 0.2, 0.05);
			glPopMatrix();
			
			// Claw2
			glPushMatrix();
				// Claw2-1
				glTranslated(-0.1, 0.15, -0.2);
				glScaled(0.4, 0.5, 1.2);
				drawCylinder(0.5, 0.2, 0.05);

				// Claw2-2
				glTranslated(-0.1, -0.2, 0.0);
				drawCylinder(0.5, 0.2, 0.05);

				// Claw2-3
				glTranslated(-0.1, -0.2, 0.0);
				drawCylinder(0.5, 0.2, 0.05);
				
				// Claw2-4
				glTranslated(0.1, -0.2, 0.0);
				glRotated(-15, 0.0, 1.0, 0.0);
				drawCylinder(0.5, 0.2, 0.05);
			glPopMatrix();

		glPopMatrix();
	glPopMatrix(); // Right Arm Ends
	//------------------------------------------------------Right Arm Ends----------------------------------------------------------------------------//
}

void drawTail() {
	//---------------------------------------------------------------------Tail-----------------------------------------------------------------------//
	glPushMatrix();
		setDiffuseColor(0.223, 0.219, 0.176);
		glScaled(1 / 1.3, 1 / 1.6, 1 / 1.23);
		glTranslated(0.0, -1.0, -0.5);
		glRotated(VAL(TAIL), 1.0, 0.0, 0.0);
		drawHalfSphere(0.3);

		glScaled(1.3, 2.0, 1.23);
		glTranslated(0.0, -0.4, 0.0);
		drawSphere(0.5);
	glPopMatrix();
	//---------------------------------------------------------------------Tail Ends-------------------------------------------------------------------//

}

static GLfloat lightPosition0[] = { -5, 10, 0.0, 0 };
static GLfloat lightPosition1[] = { -5, 6, -3.0, 0 };
static GLfloat lightDiffuse0[] = { 1,1,1,1 };
static GLfloat spot_direction0[] = { 1.0, -1.0, 0.0 };
static GLfloat spot_direction1[] = { 1.0, -1.0, 1.0 };
static double height = 0;
static double angle = 0;
static bool up = true;

void lightSetUp()
{
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction0);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse0);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction1);

}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void myModel::draw()
{
	// This call takes care of a lot of the nasty projection 
	// matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
	ModelerView::draw();
	lightSetUp();


	// I managed to create a metaball, 
	// but I didnot want to add it for designing my character :)
	marchingCube* temp = new marchingCube;

	glPushMatrix();
		glTranslated(-4, 5, 0);
		setDiffuseColor(0.015, 0.031, 0.086);
		setSpecularColor(1.0, 1.0, 1.0);
		setShininess(32.0);
		temp->drawMetaBall();
	glPopMatrix();
	
	
	setShininess(64.0);

	// draw the moon
	glPushMatrix();
		setAmbientColor(1.0f, 1.0f, 1.0f);
		setDiffuseColor(1, 1, 1);
		glTranslated(-4.2, 4.8, -4.5);
		drawMoon();
	glPopMatrix();
	
	// draw the background
	glPushMatrix();
		setDiffuseColor(1, 1, 1);
		glTranslated(-5.0, 0.0, -5.0);
		drawBackground("textures/img2.bmp");
	glPopMatrix();
	
	// draw the floor
	glPushMatrix();
		setDiffuseColor(1, 1, 1);
		glTranslated(-5.0, 0.0, -5.0);
		glRotated(90, 1.0, 0.0, 0.0);
		drawBackground("textures/img1.bmp");
	glPopMatrix();
	
	// draw my own character 
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(0.015, 0.031, 0.086);

	if (VAL(ANIMATE) == 1) {
		std::cout << "Animating" << std::endl;
		if (height >= 5) {
			up = false;
		}else if (height <= 0) {
			up = true;
		}
		
		if (up) {
			height += (double)1 / 12;
		}else {
			height -= (double)1/12;
		}
		
		glTranslated(0, height, 0);
	}else {
		glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
	}

	
	// Draw a spinning top
	glPushMatrix();	
		glPushMatrix();
			//glTranslated(0.0, 0.0, 0.0);
			glRotated(-90, 1.0, 0.0, 0.0);
			//glRotated(VAL(ROTATESPINNINGTOP), 0.0, 0.0, 1.0);
			drawCylinder(0.5, 0.0, 0.5);
			//glPopMatrix();


			//glPushMatrix();
			glTranslated(0.0, 0.0, 0.5);
			drawCylinder(0.3, 0.5, 0.5);


			glTranslated(0.0, 0.0, 0.3);
			drawCylinder(0.2, 0.1, 0.1);

		glPopMatrix();
	glPopMatrix();

	int level = VAL(LEVEL);

	//draw Totoro starts --------------------------------------------------------------------------------//
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(0.223, 0.219, 0.176);
	glPushMatrix(); 

		if (VAL(ANIMATE) == 1) {
			if (up) {
				angle += 6;
			}else {
				angle -= 6;
			}
			glRotated(angle, 0.0, 1.0, 0.0);
		}else {
			glRotated(VAL(ROTATESPINNINGTOP), 0.0, 1.0, 0.0);
		}
		// Body Starts ------------------------------------------------------------------------------//
		glPushMatrix(); 
			
			if (level >= 1) {
				drawBody();
			}
			
			if (level >= 2) {
				drawHead(level); // Head, Ear, Nose 
			}
			
			if (level >= 6) {
				drawLeftLeg();
			}

			if (level >= 7) {
				drawRightLeg();
			}

			if (level >= 8) {
				drawLeftARM(level); // Left arm and Umbrella
			}

			if (level >= 10) {
				drawRightArm();
			}
		
			if (level == 11) {
				drawTail();
			}
		glPopMatrix();  
		//-------------------------------------------------------------------------Body Ends ------------------------------------------------------------------//
		
	glPopMatrix(); 
	// MyModel -------------------------------------------------------------------------------------------------- Ends // 
}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
	ModelerControl controls[NUMCONTROLS];
	controls[LEVEL] = ModelerControl("Levels of detail", 0, 11, 1, 11);
	controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
	controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
	controls[ZPOS] = ModelerControl("Z Position", -2, 5, 0.1f, 0);
	controls[LEFTARMROTATEY ] = ModelerControl("Left Arm", -5, 90, 1, 10);
	controls[RIGHTARMROTATE] = ModelerControl("Right Arm", -30, 30, 1, -30);
	controls[HEADROTATE] = ModelerControl("Head", -90, 90, 1, 0); 
	controls[ROTATESPINNINGTOP] = ModelerControl("Whole Body(Anti-Clockwise Rotate)", 0, 360, 1, 0); 
	controls[UMBRELLA] = ModelerControl("Umbrella", -80, 0, 1, 0); 
	controls[TAIL] = ModelerControl("Tail", 30, 90, 1, 90); 
	controls[ANIMATE] = ModelerControl("Animate", 0, 1, 1, 0);

	ModelerApplication::Instance()->Init(&createMyModel, controls, NUMCONTROLS);
	return ModelerApplication::Instance()->Run();
}
