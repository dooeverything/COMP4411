// The sample model.  You should build a file
// very similar to this for when you make your model.
#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning(disable : 4786)

#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "particleSystem.h"
#include "force.h"

#include <math.h>
#include <FL/gl.h>
#include <stdlib.h>

#include "camera.h"

#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))
\
enum myModelControls
{
	LEVEL, 
	XPOS, YPOS, ZPOS, 
	LEFTARMROTATEY, RIGHTARMROTATE, HEADROTATE, 
	ROTATESPINNINGTOP, 
	UMBRELLA, 
	TAIL, 
	ANIMATE, 
	NUMCONTROLS
};



//#include "marchingCube.h"
// To make a SampleModel, we inherit off of ModelerView
class myModel : public ModelerView
{
public:
	myModel(int x, int y, int w, int h, char* label)
		: ModelerView(x, y, w, h, label) { }

	Mat4d cameraViewMatrix;
	Mat4d modelViewMatrix;
	Mat4d projectionMatrix;

	virtual void draw();
	
	// My model structure functions
	void drawBody();
	void drawHead(int level);
	void drawLeftLeg();
	void drawRightLeg();
	void drawLeftArm(int level);
	void drawRightArm();
	void drawTail();
	void breathParticles(Camera* camera); 
	Mat4d getModelViewMatrix();
	Mat4d getProjectionMatrix();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createMyModel(int x, int y, int w, int h, char* label)
{
	return new myModel(x, y, w, h, label);
}


void myModel::drawBody()
{
	// Draw Main Body
	setAmbientColor(0.223, 0.219, 0.176);
	setDiffuseColor(0.223, 0.219, 0.176);
	glScaled(1.3, 1.6, 1.23);
	glTranslated(0.0, 1.15, 0.0);
	drawSphere(1);

	// Brown Belly --------------------------------------------------------------------------//
	glPushMatrix();
	// Brown Part - Belly
	setAmbientColor(0.745, 0.749, 0.290);
	setDiffuseColor(0.745, 0.749, 0.290);
	glScaled(0.88, 0.8, 0.8);
	glTranslated(0.0, -0.1, 0.45);
	drawSphere(1.0);
	glPopMatrix();
	//Brown Belly ---------------------------------------------------------------------------//
}

void myModel::breathParticles(Camera* camera)
{
	Vec4d trans = { 0, 0.2, 0.9, 1 };
	Mat4d worldMatrix = cameraViewMatrix.inverse() * modelViewMatrix;

	Vec4d worldPos = worldMatrix* trans;

	ParticleSystem* breath = ModelerApplication::Instance()->GetParticleSystem();
	Vec3d particlePos = { worldPos[0], worldPos[1], worldPos[2] };
	breath->createBreathParticles(particlePos, 1);
	breath->setCamera(camera);
}

void myModel::drawHead(int level)
{
	//------------------------ Head starts --------------------------------------------------------------------------// 
	glPushMatrix();

		setAmbientColor(0.223, 0.219, 0.176);
		setDiffuseColor(0.223, 0.219, 0.176);

		glTranslated(0, 0.65, 0);
		modelViewMatrix = getModelViewMatrix();

		// Draw a Head
		glPushMatrix();
			glScaled(0.75 / 1.3, 1.3 / 1.6, 0.73 / 1.23);
			drawTotoroHead(1.0);
		glPopMatrix();
		// End

		glScaled(1.25, 1.2, 1.3);
		glRotated(VAL(HEADROTATE), 0.0, 1.0, 0.0);

	//--------------------------------------------Ear Starts-----------------------------------------------------//
	if (level >= 3) {
		
		// Right Ear
		glPushMatrix(); 
			glTranslated(-0.3, 0.4, 0.0);
			glRotated(-90, 1.0, 0.0, 0.0);
			glRotated(-10, 0.0, 1.0, 0.0);
			glScaled(0.5, 0.5, 0.7);
			drawCylinder(0.3, 0.1, 0.1);

			glTranslated(0.0, 0.0, 0.2);
			drawCylinder(0.1, 0.1, 0.2);
			glTranslated(0.0, 0.0, 0.1);

			drawCylinder(0.4, 0.2, 0.0);
		glPopMatrix(); 
		// Right ear ends

		//Left ear
		glPushMatrix(); 
			glTranslated(0.3, 0.4, 0.0);
			glRotated(-90, 1.0, 0.0, 0.0);
			glRotated(10, 0.0, 1.0, 0.0);
			glScaled(0.5, 0.5, 0.7);
			drawCylinder(0.3, 0.1, 0.1);

			glTranslated(0.0, 0.0, 0.2);
			drawCylinder(0.1, 0.1, 0.2);
			glTranslated(0.0, 0.0, 0.1);

			drawCylinder(0.4, 0.2, 0.0);
		glPopMatrix(); 
		// Left ear ends
		
	}
	//------------------------------------------------Ear Ends--------------------------------------------------//

	//----------------------------------------------- Nose START -----------------------------------------------// 
	if (level >= 4) 
	{

		glPushMatrix();
		{
			glTranslated(0.0, 0.2, 0.5);
			glScaled(1.0, 0.5, 0.5);
			glRotated(70, 1.0, 0.0, 0.0);
			drawHalfSphere(0.3);

			// ---------------------- Cat whiskers START ------------------------------------------------------------//
			// Left Whiskers 1
			glPushMatrix();
				setAmbientColor(0, 0, 0);
				setDiffuseColor(0, 0, 0);
				glScaled(0.18, 0.1, 0.1);
				glTranslated(0.0, 1.0, 0.0);
				glRotated(90, 0.0, 1.0, 0.0);
				drawCylinder(4.0, 0.1, 0.1);
			glPopMatrix();

			// Left Whiskers 2
			glPushMatrix();
				glScaled(0.15, 0.1, 0.1);
				glTranslated(0.0, 1.0, 0.0);
				glRotated(70, 0.0, 1.0, 0.0);
				drawCylinder(4.0, 0.1, 0.1);
			glPopMatrix();
		
			// Left Whiskers 3
			glPushMatrix();
				glScaled(0.15, 0.1, 0.1);
				glTranslated(0.0, 1.0, 0.0);
				glRotated(110, 0.0, 1.0, 0.0);
				drawCylinder(4.0, 0.1, 0.1);
			glPopMatrix();

			// Right Whiskers 1
			glPushMatrix();
				glScaled(0.18, 0.1, 0.1);
				glTranslated(0.0, 1.0, 0.0);
				glRotated(-90, 0.0, 1.0, 0.0);
				drawCylinder(4.0, 0.1, 0.1);
			glPopMatrix();

			// Right Whiskers 2
			glPushMatrix();
				glScaled(0.15, 0.1, 0.1);
				glTranslated(0.0, 1.0, 0.0);
				glRotated(-70, 0.0, 1.0, 0.0);
				drawCylinder(4.0, 0.1, 0.1);
			glPopMatrix();

			// Right Whiskers 2
			glPushMatrix();
				glScaled(0.15, 0.1, 0.1);
				glTranslated(0.0, 1.0, 0.0);
				glRotated(-110, 0.0, 1.0, 0.0);
				drawCylinder(4.0, 0.1, 0.1);
			glPopMatrix();
			// ---------------------- Cat whiskers Ends ------------------------------------------------------------//

			// Nose Hole
			glPushMatrix();
			//setDiffuseColor(0.0, 0.0, 0.0);
				glScaled(0.5, 0.5, 0.5);
				glTranslated(0.0, 0.5, -0.2);
				drawSphere(0.13);
			glPopMatrix();

			// Mouth	
			glPushMatrix();
			//setDiffuseColor(0.0, 0.0, 0.0);
				glTranslated(0.0, 0.05, 0.3);
				glScaled(0.2, 0.1, 0.2);
				glRotated(-20, 1.0, 0.0, 0.0);
				drawSphere(0.1);
			glPopMatrix();
		}
		glPopMatrix();
		//----------------------------------------------- Nose END --------------------------------------------------// 
	}

	//-------------------------------------------------Eyes------------------------------------------------------//
	if (level >= 5) 
	{
		// Left Eye starts
		glPushMatrix(); 
			setAmbientColor(1.0, 1.0, 1.0);
			setDiffuseColor(1.0, 1.0, 1.0);
			glScaled(0.5, 0.4, 0.5);
			glTranslated(0.5, 0.85, 0.6);

			glRotated(70, 1.0, 0.0, 0.0);
			drawHalfSphere(0.2);

			// Left Eye's Pupil
			setAmbientColor(0.0, 0.0, 0.0);
			setDiffuseColor(0.0, 0.0, 0.0);
			glTranslated(0.02, 0.13, 0.0);
			drawSphere(0.08);
		glPopMatrix(); 
		// Left Eye ends

		// Right Eye starts
		glPushMatrix(); 
			setAmbientColor(1.0, 1.0, 1.0);
			setDiffuseColor(1.0, 1.0, 1.0);
			glScaled(0.5, 0.4, 0.5);
			glTranslated(-0.5, 0.85, 0.6);

			glRotated(70, 1.0, 0.0, 0.0);
			drawHalfSphere(0.2);

			// Right  Eye's Pupil
			setAmbientColor(0.0, 0.0, 0.0);
			setDiffuseColor(0.0, 0.0, 0.0);
			glTranslated(-0.02, 0.13, 0.0);
			drawSphere(0.08);
		glPopMatrix(); 
		// Right Eye ends
		//--------------------------------------Eyes Ends----------------------------------//
	}
	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE); //GL_ONE_MINUS_SRC_ALPHA GL_ONE

	//glDisable(GL_BLEND);

	//---------------------------- Head END -------------------------------------------------------------------------//
	glPopMatrix();
}

void myModel::drawLeftLeg() {
	//------------------------ Left Leg starts ----------------------------------------------------------------------//  
	glPushMatrix();
		setAmbientColor(0.223, 0.219, 0.176);
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

		glPushMatrix();
			setAmbientColor(0, 0, 0);
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

	glPopMatrix();
	//-------------------------------------------Left Leg ends ------------------------------------------------------------------------// 
}

void myModel::drawRightLeg()
{
	//------------------------------------------- Right Leg Start ----------------------------------------------------------------------//
	glPushMatrix();// Right Leg starts 
	setAmbientColor(0.223, 0.219, 0.176);
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
	setAmbientColor(0, 0, 0);
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

void myModel::drawLeftArm(int level)
{
	//---------------------------------------------------------Left Arm --------------------------------------------------------------------//
	glPushMatrix();
	setAmbientColor(0.223, 0.219, 0.176);
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
	glPushMatrix();
	setAmbientColor(0, 0, 0);
	setDiffuseColor(0, 0, 0);
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
		glPushMatrix();
		//setAmbientColor(0, 0, 0);
		glTranslated(-0.1, 0.0, 0.3);
		glRotated(90, 1.0, 0.0, 0.0);
		drawHalfTorus(0.1, 0.05);

		glTranslated(0.1, 0.0, 0.0);
		glRotated(-180, 1.0, 0.0, 0.0);
		drawCylinder(2.8, 0.05, 0.05);

		setAmbientColor(0.023, 0.035, 0.137);
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

void myModel::drawRightArm()
{
	//------------------------------------------------------Right Arm----------------------------------------------------------------------------//
	glPushMatrix();
	setAmbientColor(0.223, 0.219, 0.176);
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
	setAmbientColor(0.0, 0.0, 0.0);
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

void myModel::drawTail() {
	//---------------------------------------------------------------------Tail-----------------------------------------------------------------------//
	glPushMatrix();
	setAmbientColor(0.223, 0.219, 0.176);
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

Mat4d myModel::getModelViewMatrix()
{
	GLdouble m[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, m);
	Mat4d matMV(m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);

	return matMV.transpose(); // convert to row major
}

Mat4d myModel::getProjectionMatrix()
{
	GLdouble m[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, m);
	Mat4d matP(m[0], m[1], m[2], m[3], 
			   m[4], m[5], m[6], m[7],
			   m[8], m[9], m[10], m[11],
			   m[12], m[13], m[14], m[15]);

	return matP.transpose();
}

//static GLfloat ambientLight[] = { 0.25f, 0.25f, 0.0f, 1.0f };    
static GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };    
static GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };       
static GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

static GLfloat lightPosition0[] = { 0.0, 10, 3.0, 0 };
static GLfloat lightPosition1[] = { -5, 6, -3.0, 0 };
static GLfloat lightDiffuse0[] = { 1,1,1,1 };
static GLfloat spot_direction0[] = { 1.0, -1.0, 0.0 };
static GLfloat spot_direction1[] = { 1.0, -1.0, 1.0 };
static double height = 0;
static double angle = 0;
static bool up = true;

void lightSetUp()
{
	/*glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction0);
	glEnable(GL_LIGHT0);*/

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
	//glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	//glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction1);

}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void myModel::draw()
{
	// This call takes care of a lot of the nasty projection 
	// matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
	ModelerView::draw();

	cameraViewMatrix = getModelViewMatrix();

	glEnable(GL_LIGHTING);

	// draw the floor
	glPushMatrix();
		setAmbientColor(0.415, 0.305, 0.066);
		setDiffuseColor(0.415, 0.305, 0.066);
		//glColor3d(1, 1, 0);
		glTranslated(-7.5, 0.0, -3.0);
		glRotated(90, 1.0, 0.0, 0.0);
		drawBox(15, 8, 0);
		//drawBackground("textures/img1.bmp");
	glPopMatrix();

	// draw my own character 
	drawBezierSurface();

	if (VAL(ANIMATE) == 1) {
		std::cout << "Animating" << std::endl;
		if (height >= 5) {
			up = false;
		}
		else if (height <= 0) {
			up = true;
		}

		if (up) {
			height += (double)1 / 12;
		}
		else {
			height -= (double)1 / 12;
		}

		glTranslated(0, height, 0);
	}
	else {
		glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
	}

	int level = VAL(LEVEL);
	
	//draw Totoro starts --------------------------------------------------------------------------------//
	glPushMatrix();

		if (VAL(ANIMATE) == 1) {
			if (up) {
				angle += 6;
			}
			else {
				angle -= 6;
			}
			glRotated(angle, 0.0, 1.0, 0.0);
		}
		else {
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
				drawLeftArm(level); // Left arm and Umbrella
			}

			if (level >= 10) {
				drawRightArm();
			}

			if (level == 11) {
				drawTail();
			}
		glPopMatrix();
		// Body Ends ------------------------------------------------------------------//

	glPopMatrix();
	// MyModel -------------------------------------------------------------------------------------------------- Ends // 
	
	// Draw Breath Particle
	breathParticles(m_camera);
	
	ParticleSystem* ps = ModelerApplication::Instance()->GetParticleSystem();

	if (ps != NULL) {
		ps->drawBreathParticles(t);
	}

	endDraw();
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
	controls[LEFTARMROTATEY] = ModelerControl("Left Arm", -5, 90, 1, 10);
	controls[RIGHTARMROTATE] = ModelerControl("Right Arm", -30, 30, 1, -30);
	controls[HEADROTATE] = ModelerControl("Head", -90, 90, 1, 0);
	controls[ROTATESPINNINGTOP] = ModelerControl("Whole Body(Anti-Clockwise Rotate)", 0, 360, 1, 0);
	controls[UMBRELLA] = ModelerControl("Umbrella", -80, 0, 1, 0);
	controls[TAIL] = ModelerControl("Tail", 30, 90, 1, 90);
	controls[ANIMATE] = ModelerControl("Animate", 0, 1, 1, 0);


	// Create and set up particles
	ParticleSystem* ps = new ParticleSystem(10);
	Gravity* g = new Gravity( ( Vec3d(0, -0.08, 0) ) );
	Wind* w = new Wind( Vec3d(0.02, 0, 0) );
	Viscous* v = new Viscous(0.01);

	ps->addForce(g);
	ps->addForce(w);
	ps->addForce(v);


	Wind* w2 = new Wind(Vec3d(0.002, 0, 0));
	Viscous* v2 = new Viscous(0.1);

	ps->addBreathForce(w2);
	ps->addBreathForce(v2);

	ModelerApplication::Instance()->SetParticleSystem(ps);
	ModelerApplication::Instance()->Init(&createMyModel, controls, NUMCONTROLS);
	
	return ModelerApplication::Instance()->Run();
}
