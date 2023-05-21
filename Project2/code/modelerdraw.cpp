#include "modelerdraw.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include <math.h>
#include <vector>
#include <glm/glm.hpp>
#include "bitmap.h"
#include <iostream>
#include "marchingCube.h"

// ********************************************************
// Support functions from previous version of modeler
// ********************************************************
void _dump_current_modelview( void )
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    if (mds->m_rayFile == NULL)
    {
        fprintf(stderr, "No .ray file opened for writing, bailing out.\n");
        exit(-1);
    }
    
    GLdouble mv[16];
    glGetDoublev( GL_MODELVIEW_MATRIX, mv );
    fprintf( mds->m_rayFile, 
        "transform(\n    (%f,%f,%f,%f),\n    (%f,%f,%f,%f),\n     (%f,%f,%f,%f),\n    (%f,%f,%f,%f),\n",
        mv[0], mv[4], mv[8], mv[12],
        mv[1], mv[5], mv[9], mv[13],
        mv[2], mv[6], mv[10], mv[14],
        mv[3], mv[7], mv[11], mv[15] );
}

void _dump_current_material( void )
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    if (mds->m_rayFile == NULL)
    {
        fprintf(stderr, "No .ray file opened for writing, bailing out.\n");
        exit(-1);
    }
    
    fprintf( mds->m_rayFile, 
        "material={\n    diffuse=(%f,%f,%f);\n    ambient=(%f,%f,%f);\n}\n",
        mds->m_diffuseColor[0], mds->m_diffuseColor[1], mds->m_diffuseColor[2], 
        mds->m_diffuseColor[0], mds->m_diffuseColor[1], mds->m_diffuseColor[2]);
}

// ****************************************************************************

// Initially assign singleton instance to NULL
ModelerDrawState* ModelerDrawState::m_instance = NULL;

ModelerDrawState::ModelerDrawState() : m_drawMode(NORMAL), m_quality(MEDIUM)
{
    float grey[]  = {.5f, .5f, .5f, 1};
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    
    memcpy(m_ambientColor, black, 4 * sizeof(float));
    memcpy(m_diffuseColor, grey, 4 * sizeof(float));
    memcpy(m_specularColor, white, 4 * sizeof(float));
    
    m_shininess = 0.5;
    
    m_rayFile = NULL;
}

// CLASS ModelerDrawState METHODS
ModelerDrawState* ModelerDrawState::Instance()
{
    // Return the singleton if it exists, otherwise, create it
    return (m_instance) ? (m_instance) : m_instance = new ModelerDrawState();
}

// ****************************************************************************
// Modeler functions for your use
// ****************************************************************************
// Set the current material properties

void setAmbientColor(float r, float g, float b)
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    mds->m_ambientColor[0] = (GLfloat)r;
    mds->m_ambientColor[1] = (GLfloat)g;
    mds->m_ambientColor[2] = (GLfloat)b;
    mds->m_ambientColor[3] = (GLfloat)1.0;
    
    if (mds->m_drawMode == NORMAL)
        glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mds->m_ambientColor);
}

void setDiffuseColor(float r, float g, float b)
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    mds->m_diffuseColor[0] = (GLfloat)r;
    mds->m_diffuseColor[1] = (GLfloat)g;
    mds->m_diffuseColor[2] = (GLfloat)b;
    mds->m_diffuseColor[3] = (GLfloat)1.0;
    
    if (mds->m_drawMode == NORMAL)
        glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mds->m_diffuseColor);
    else
        glColor3f(r,g,b);
}

void setSpecularColor(float r, float g, float b)
{	
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    mds->m_specularColor[0] = (GLfloat)r;
    mds->m_specularColor[1] = (GLfloat)g;
    mds->m_specularColor[2] = (GLfloat)b;
    mds->m_specularColor[3] = (GLfloat)1.0;
    
    if (mds->m_drawMode == NORMAL)
        glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mds->m_specularColor);
}

void setShininess(float s)
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    mds->m_shininess = (GLfloat)s;
    
    if (mds->m_drawMode == NORMAL)
        glMaterialf( GL_FRONT, GL_SHININESS, mds->m_shininess);
}

void setDrawMode(DrawModeSetting_t drawMode)
{
    ModelerDrawState::Instance()->m_drawMode = drawMode;
}

void setQuality(QualitySetting_t quality)
{
    ModelerDrawState::Instance()->m_quality = quality;
}

bool openRayFile(const char rayFileName[])
{
    ModelerDrawState *mds = ModelerDrawState::Instance();

	fprintf(stderr, "Ray file format output is buggy (ehsu)\n");
    
    if (!rayFileName)
        return false;
    
    if (mds->m_rayFile) 
        closeRayFile();
    
    mds->m_rayFile = fopen(rayFileName, "w");
    
    if (mds->m_rayFile != NULL) 
    {
        fprintf( mds->m_rayFile, "SBT-raytracer 1.0\n\n" );
        fprintf( mds->m_rayFile, "camera { fov=30; position=(0,0.8,5); direction=(0,-0.8,-5); }\n\n" );
        fprintf( mds->m_rayFile, 
            "directional_light { direction=(-1,-2,-1); color=(0.7,0.7,0.7); }\n\n" );
        return true;
    }
    else
        return false;
}

void _setupOpenGl()
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
	switch (mds->m_drawMode)
	{
	case NORMAL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_SMOOTH);
		break;
	case FLATSHADE:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_FLAT);
		break;
	case WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glShadeModel(GL_FLAT);
	default:
		break;
	}

}

void closeRayFile()
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    if (mds->m_rayFile) 
        fclose(mds->m_rayFile);
    
    mds->m_rayFile = NULL;
}

void drawSphere(double r)
{
    ModelerDrawState *mds = ModelerDrawState::Instance();

	_setupOpenGl();
    
    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, "scale(%f,%f,%f,sphere {\n", r, r, r );
        _dump_current_material();
        fprintf(mds->m_rayFile, "}))\n" );
    }
    else
    {
        int divisions = 0; 
        GLUquadricObj* gluq;
        
        switch(mds->m_quality)
        {
        case HIGH: 
            divisions = 32; break;
        case MEDIUM: 
            divisions = 20; break;
        case LOW:
            divisions = 12; break;
        case POOR:
            divisions = 8; break;
        }
        
        gluq = gluNewQuadric();
        gluQuadricDrawStyle( gluq, GLU_FILL );
        gluQuadricTexture( gluq, GL_TRUE );
        gluSphere(gluq, r, divisions, divisions);
        gluDeleteQuadric( gluq );
    }
}

void drawHalfSphere(float r)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();

    _setupOpenGl();
    
    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, "scale(%f,%f,%f,Half sphere {\n", r, r, r);
        _dump_current_material();
        fprintf(mds->m_rayFile, "}))\n");
    }
    else
    {
        int divisions = 0;
        GLUquadricObj* gluq;

        switch (mds->m_quality)
        {
        case HIGH:
            divisions = 32; break;
        case MEDIUM:
            divisions = 20; break;
        case LOW:
            divisions = 12; break;
        case POOR:
            divisions = 8; break;
        }

        float division = (float)divisions - 1.0f;

        float angle_sector = (2 * M_PI) / division; // 0 to 360
        float angle_stack = (M_PI/2) / division ; // 90 to 0
        
        vector<vec3> stack_vertices;

        // Draw a half sphere 
        for (int i = 0; i < divisions; i++) {
            float angle1 = angle_sector * i;
            for (int j = 0; j < divisions; j++) {
                float angle = (float)angle_stack * (float)j;
                float angle2 = (M_PI / 2) - angle;

                vec3 points;

                float x = r * cos(angle2) * sin(angle1);
                float y = r * sin(angle2);
                float z = r * cos(angle2) * cos(angle1);
            
                points = { x, y, z };
                stack_vertices.push_back(points);
            }
        }
        int count = 0;
        int sector_count = 0;

        glBegin(GL_TRIANGLES);
        
        for (int i = 0; i < divisions; i++) { // Sector Line
            sector_count++;
            for (int j = 0; j < divisions; j++) { // Stack Line
                vec3 vertex1;
                vec3 vertex2;
                vec3 vertex3;
                vec3 vertex4;
                int next_sector = count + divisions;;

                if (sector_count == divisions ){
                    next_sector = j + 1;
                }

                if (j == 0) {
                    vertex1 = stack_vertices[count];
                    vertex2 = stack_vertices[count+1];
                    vertex3 = stack_vertices[next_sector];

                    vec3 v1_2 = vertex2 - vertex1;
                    vec3 v1_3 = vertex3 - vertex1;

                    vec3 norm = cross(v1_2, v1_3);

                    glNormal3f(norm.x, norm.y, norm.z);
                    glVertex3f(vertex1.x, vertex1.y, vertex1.z);
                    glVertex3f(vertex2.x, vertex2.y, vertex2.z);
                    glVertex3f(vertex3.x, vertex3.y, vertex3.z);
                
                    continue;
                }


                // Get 4 coordinate to draw a square
                vertex1 = stack_vertices[count];
                vertex2 = stack_vertices[count + 1];
                vertex3 = stack_vertices[next_sector];
                vertex4 = stack_vertices[next_sector+1];

                // Draw a square

                vec3 v1_2 = vertex2 - vertex1;
                vec3 v1_3 = vertex3 - vertex1;
                vec3 norm = cross(v1_2, v1_3);

                glNormal3f(norm.x, norm.y, norm.z);
               
                // 1. Draw Upper Triangle
                glVertex3f(vertex1.x, vertex1.y, vertex1.z);
                glVertex3f(vertex2.x, vertex2.y, vertex2.z);
                glVertex3f(vertex3.x, vertex3.y, vertex3.z);
               
               
                // 2. Draw Lower Triangle
                glVertex3f(vertex4.x, vertex4.y, vertex4.z);
                glVertex3f(vertex2.x, vertex2.y, vertex2.z);
                glVertex3f(vertex3.x, vertex3.y, vertex3.z);

                count++;
            }
        }
        glEnd();
    }
}

void drawTotoroHead(float r)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();

    _setupOpenGl();

    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, "scale(%f,%f,%f,Totoro Head {\n", r, r, r);
        _dump_current_material();
        fprintf(mds->m_rayFile, "}))\n");
    }
    else
    {
        int divisions = 0;
        GLUquadricObj* gluq;

        switch (mds->m_quality)
        {
        case HIGH:
            divisions = 32; break;
        case MEDIUM:
            divisions = 20; break;
        case LOW:
            divisions = 12; break;
        case POOR:
            divisions = 8; break;
        }
        float division = (float)divisions - 1;
        float angle_sector = (2 * M_PI) / division; // 0 to 360
        float angle_stack = (M_PI / 2) / division; // 90 to 0

        vector<vec3> stack_vertices;
        float half_division = divisions / 2;
        // Draw a half sphere 
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < divisions; i++) {
            float angle1 = angle_sector * i;
            for (int j = 0; j < divisions; j++) {
                float angle2 = angle_stack * j;
                angle2 = (M_PI / 2) - angle2;


                vec3 points;

                float adjustment = ((half_division - j) / half_division) * 0.3;

                if (j == 0) {
                    adjustment = adjustment - 0.06;
                }else if (j == 1) {
                    adjustment = adjustment - 0.03;
                }else if (j == 2) {
                    adjustment = adjustment - 0.02;
                }

                float l = r - adjustment;

                if (angle2 >= 45 && angle2 < 80) {
                    l += 0.3 * j;
                }

                float x = l * cos(angle2) * sin(angle1);
                float y = l * sin(angle2);
                float z = l * cos(angle2) * cos(angle1);

                points = { x, y, z };
                stack_vertices.push_back(points);
            }
        }

        int count = 0;
        int sector_count = 0;
        for (int i = 0; i < divisions; i++) { // Sector Line
            sector_count++;
            for (int j = 0; j < divisions; j++) { // Stack Line
                vec3 vertex1;
                vec3 vertex2;
                vec3 vertex3;
                vec3 vertex4;
                int next_sector = count + divisions;;

                if (sector_count == divisions) {
                    next_sector = j + 1;
                }

                if (j == 0) {
                    vertex1 = stack_vertices[count];
                    vertex2 = stack_vertices[count + 1];
                    vertex3 = stack_vertices[next_sector];

                    vec3 v1_2 = vertex2 - vertex1;
                    vec3 v1_3 = vertex3 - vertex1;

                    vec3 norm = cross(v1_2, v1_3);

                    glNormal3f(norm.x, norm.y, norm.z);
                    glVertex3f(vertex1.x, vertex1.y, vertex1.z);
                    glVertex3f(vertex2.x, vertex2.y, vertex2.z);
                    glVertex3f(vertex3.x, vertex3.y, vertex3.z);
                    continue;
                }


                // Get 4 coordinate to draw a square
                vertex1 = stack_vertices[count];
                vertex2 = stack_vertices[count + 1];
                vertex3 = stack_vertices[next_sector];
                vertex4 = stack_vertices[next_sector + 1];


                // Draw a square

                vec3 v1_2 = vertex2 - vertex1;
                vec3 v1_3 = vertex3 - vertex1;
                vec3 norm = cross(v1_2, v1_3);

                glNormal3f(norm.x, norm.y, norm.z);

                // 1. Draw Upper Triangle
                glVertex3f(vertex1.x, vertex1.y, vertex1.z);
                glVertex3f(vertex2.x, vertex2.y, vertex2.z);
                glVertex3f(vertex3.x, vertex3.y, vertex3.z);


                // 2. Draw Lower Triangle
                glVertex3f(vertex4.x, vertex4.y, vertex4.z);
                glVertex3f(vertex2.x, vertex2.y, vertex2.z);
                glVertex3f(vertex3.x, vertex3.y, vertex3.z);

                count++;
            }
        }
        glEnd();
    }
}

void drawBox( double x, double y, double z )
{
    ModelerDrawState *mds = ModelerDrawState::Instance();

	_setupOpenGl();
    
    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile,  
            "scale(%f,%f,%f,translate(0.5,0.5,0.5,box {\n", x, y, z );
        _dump_current_material();
        fprintf(mds->m_rayFile,  "})))\n" );
    }
    else
    {
        /* remember which matrix mode OpenGL was in. */
        int savemode;
        glGetIntegerv( GL_MATRIX_MODE, &savemode );
        
        /* switch to the model matrix and scale by x,y,z. */
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glScaled( x, y, z );
        
        glBegin( GL_QUADS );
        
        glNormal3d( 0.0, 0.0, -1.0 );
        glVertex3d( 0.0, 0.0, 0.0 ); glVertex3d( 0.0, 1.0, 0.0 );
        glVertex3d( 1.0, 1.0, 0.0 ); glVertex3d( 1.0, 0.0, 0.0 );
        
        glNormal3d( 0.0, -1.0, 0.0 );
        glVertex3d( 0.0, 0.0, 0.0 ); glVertex3d( 1.0, 0.0, 0.0 );
        glVertex3d( 1.0, 0.0, 1.0 ); glVertex3d( 0.0, 0.0, 1.0 );
        
        glNormal3d( -1.0, 0.0, 0.0 );
        glVertex3d( 0.0, 0.0, 0.0 ); glVertex3d( 0.0, 0.0, 1.0 );
        glVertex3d( 0.0, 1.0, 1.0 ); glVertex3d( 0.0, 1.0, 0.0 );
        
        glNormal3d( 0.0, 0.0, 1.0 );
        glVertex3d( 0.0, 0.0, 1.0 ); glVertex3d( 1.0, 0.0, 1.0 );
        glVertex3d( 1.0, 1.0, 1.0 ); glVertex3d( 0.0, 1.0, 1.0 );
        
        glNormal3d( 0.0, 1.0, 0.0 );
        glVertex3d( 0.0, 1.0, 0.0 ); glVertex3d( 0.0, 1.0, 1.0 );
        glVertex3d( 1.0, 1.0, 1.0 ); glVertex3d( 1.0, 1.0, 0.0 );
        
        glNormal3d( 1.0, 0.0, 0.0 );
        glVertex3d( 1.0, 0.0, 0.0 ); glVertex3d( 1.0, 1.0, 0.0 );
        glVertex3d( 1.0, 1.0, 1.0 ); glVertex3d( 1.0, 0.0, 1.0 );
        
        glEnd();
        
        /* restore the model matrix stack, and switch back to the matrix
        mode we were in. */
        glPopMatrix();
        glMatrixMode( savemode );
    }
}

void drawTextureBox( double x, double y, double z )
{
    // NOT IMPLEMENTED, SORRY (ehsu)
}

void drawCylinder( double h, double r1, double r2 )
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    int divisions = 0;

	_setupOpenGl();
    
    switch(mds->m_quality)
    {
    case HIGH: 
        divisions = 32; break;
    case MEDIUM: 
        divisions = 20; break;
    case LOW:
        divisions = 12; break;
    case POOR:
        divisions = 8; break;
    }
    
    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, 
            "cone { height=%f; bottom_radius=%f; top_radius=%f;\n", h, r1, r2 );
        _dump_current_material();
        fprintf(mds->m_rayFile, "})\n" );
    }
    else
    {
        GLUquadricObj* gluq;
        
        /* GLU will again do the work.  draw the sides of the cylinder. */
        gluq = gluNewQuadric();
        gluQuadricDrawStyle( gluq, GLU_FILL );
        gluQuadricTexture( gluq, GL_TRUE );
        gluCylinder( gluq, r1, r2, h, divisions, divisions);
        gluDeleteQuadric( gluq );
        
        if ( r1 > 0.0 )
        {
        /* if the r1 end does not come to a point, draw a flat disk to
            cover it up. */
            
            gluq = gluNewQuadric();
            gluQuadricDrawStyle( gluq, GLU_FILL );
            gluQuadricTexture( gluq, GL_TRUE );
            gluQuadricOrientation( gluq, GLU_INSIDE );
            gluDisk( gluq, 0.0, r1, divisions, divisions);
            gluDeleteQuadric( gluq );
        }
        
        if ( r2 > 0.0 )
        {
        /* if the r2 end does not come to a point, draw a flat disk to
            cover it up. */
            
            /* save the current matrix mode. */	
            int savemode;
            glGetIntegerv( GL_MATRIX_MODE, &savemode );
            
            /* translate the origin to the other end of the cylinder. */
            glMatrixMode( GL_MODELVIEW );
            glPushMatrix();
            glTranslated( 0.0, 0.0, h );
            
            /* draw a disk centered at the new origin. */
            gluq = gluNewQuadric();
            gluQuadricDrawStyle( gluq, GLU_FILL );
            gluQuadricTexture( gluq, GL_TRUE );
            gluQuadricOrientation( gluq, GLU_OUTSIDE );
            gluDisk( gluq, 0.0, r2, divisions, divisions);
            gluDeleteQuadric( gluq );
            
            /* restore the matrix stack and mode. */
            glPopMatrix();
            glMatrixMode( savemode );
        }
    }
    
}

void drawTriangle( float x1, float y1, float z1,
                   float x2, float y2, float z2,
                   float x3, float y3, float z3 , bool smooth)
{
    ModelerDrawState *mds = ModelerDrawState::Instance();

	_setupOpenGl();

    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, 
            "polymesh { points=((%f,%f,%f),(%f,%f,%f),(%f,%f,%f)); faces=((0,1,2));\n", x1, y1, z1, x2, y2, z2, x3, y3, z3 );
        _dump_current_material();
        fprintf(mds->m_rayFile, "})\n" );
    }
    else
    {

        if (smooth) {
            glBegin( GL_TRIANGLES );
            //glNormal3d( b*f - c*e, c*d - a*f, a*e - b*d );
            
            float t = 0.5;

            glm::vec3 v1_1 = { x2-x1, y2-y1, z2-z1 };
            glm::vec3 v1_2 = { x3-x1, y3-y1, z3-z1 };
            glm::vec3 n1 = cross(v1_1, v1_2);

            glm::vec3 v2_1 = { x1-x2, y1-y2, z1-z2 };
            glm::vec3 v2_2 = { x3-x2, y3-y2, z3-z2 };
            glm::vec3 n2 = cross(v2_1, v2_2);

            glm::vec3 v3_1 = { x1-x3, y1-y3, z1-z3 };
            glm::vec3 v3_2 = { x2-x3, y2-y3, z2-z3 };
            glm::vec3 n3 = cross(v3_1, v3_2);

            //glm::normalize(n);

            //printf("Draw triangle at x: %f, y: %f, z: %f \n", x1, y1, z1);

            glBegin(GL_TRIANGLES);
            glNormal3f(n1.x, n1.y, n1.z);
            glVertex3f( x1, y1, z1 );
            glNormal3f(n2.x, n2.y, n2.z);
            glVertex3f( x2, y2, z2 );
            glNormal3f(n3.x, n3.y, n3.z);
            glVertex3f( x3, y3, z3 );
            glEnd();
                
        }else {
            float a, b, c, d, e, f;
        
            /* the normal to the triangle is the cross product of two of its edges. */
            a = x2-x1;
            b = y2-y1;
            c = z2-z1;
        
            d = x3-x1;
            e = y3-y1;
            f = z3-z1;
            glBegin(GL_TRIANGLES);
            glNormal3f(b * f - c * e, c * d - a * f, a * e - b * d);
            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
            glVertex3f(x3, y3, z3);
            glEnd();
        }
    }
}

void drawPyramid()
{
    _setupOpenGl();
    
    int savemode;
    glGetIntegerv(GL_MATRIX_MODE, &savemode);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glBegin(GL_TRIANGLES);
        setDiffuseColor(1.0, 0.0, 0.0);
        glVertex3d(0.0, 1.0, 0.0); // { Front }
        glVertex3d(-1.0, -1.0, 1.0); // { Front }
        glVertex3d(1.0, -1.0, 1.0); // { Front }

        setDiffuseColor(0.0, 1.0, 0.0);
        glVertex3d(0.0, 1.0, 0.0); // { Right }
        glVertex3d(1.0, -1.0, 1.0); // { Right }
        glVertex3d(1.0, -1.0, -1.0); // { Right }

        setDiffuseColor(0.0, 0.0, 1.0);
        glVertex3d(0.0, 1.0, 0.0); // { Back }
        glVertex3d(1.0, -1.0, -1.0); // { Back }
        glVertex3d(-1.0, -1.0, -1.0); // { Back }

        setDiffuseColor(1.0, 0.0, 1.0);
        glVertex3d(0.0, 1.0, 0.0); // { Left }
        glVertex3d(-1.0, -1.0, -1.0); // { Left }
        glVertex3d(-1.0, -1.0, 1.0); // { Left }
    glEnd();
    glPopMatrix();
    glMatrixMode(savemode);
}

void drawHalfTorus(double r_main, double r_tube)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();

    _setupOpenGl();

    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, "scale(%f,%f,%f,Half sphere {\n", r_main, r_main, r_main);
        _dump_current_material();
        fprintf(mds->m_rayFile, "}))\n");
    }
    else
    {
        int divisions = 0;
        int num_tube = 10;
        GLUquadricObj* gluq;

        switch (mds->m_quality)
        {
        case HIGH:
            divisions = 32; break;
        case MEDIUM:
            divisions = 20; break;
        case LOW:
            divisions = 12; break;
        case POOR:
            divisions = 8; break;
        }

        //double angle_main = (M_PI / 2) / ((double)divisions - 1); // 0 to 90
        double angle_tube = (2 * M_PI) / ((double)divisions - 1); // 0 to 360
    
        double angle_each = 20;
        std::vector<std::vector<double>> stack_vertices;

        for (int i = 0; i < num_tube; i++) {
            int a = 10;
            double angle1 = (angle_each*M_PI/180) * i;
            double x_main = r_main * cos(angle1);
            double y_main = 0;
            double z_main = r_main * sin(angle1);

            for (int j = 0; j < divisions; j++) {
                double angle2 = angle_tube * j;

                std::vector<double> points;

                double x_tube = x_main + r_tube * cos(angle2) * cos(angle1); // x
                double y_tube = y_main + r_tube * sin(angle2);
                double z_tube = z_main + r_tube * cos(angle2) * sin(angle1);

                points = { x_tube, y_tube, z_tube };
                double debug = points[0];
                stack_vertices.push_back(points);
            }
        }

        int count = 0;
        int sector_count = 0;
        for (int i = 0; i < num_tube; i++) { // Sector Line
            sector_count++;
            for (int j = 0; j < divisions; j++) { // Stack Line
                std::vector<double> vertex1;
                std::vector<double> vertex2;
                std::vector<double >vertex3;
                std::vector<double >vertex4;
                int next_sector = count + divisions;

                if (i == num_tube-1) {
                    continue;
                }else if (j == divisions - 1) {
                    vertex1 = stack_vertices[count];
                    vertex2 = stack_vertices[count - j];
                    vertex3 = stack_vertices[next_sector];
                    vertex4 = stack_vertices[next_sector - j];
                }else {
                    // Get 4 coordinate to draw a square
                    vertex1 = stack_vertices[count];
                    vertex2 = stack_vertices[count + 1];
                    vertex3 = stack_vertices[next_sector];
                    vertex4 = stack_vertices[next_sector + 1];
                }


                // Draw a square

                // 1. Draw Upper Triangle
                drawTriangle(vertex1[0], vertex1[1], vertex1[2],
                    vertex2[0], vertex2[1], vertex2[2],
                    vertex3[0], vertex3[1], vertex3[2], true);

                // 2. Draw Lower Triangle
                drawTriangle(vertex4[0], vertex4[1], vertex4[2],
                    vertex2[0], vertex2[1], vertex2[2],
                    vertex3[0], vertex3[1], vertex3[2], true);

                count++;
            }
            int a = 0;
        }
    }
}

void drawUmbrella(double r, double angle)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();

    _setupOpenGl();

    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, "scale(%f,%f,%f, Umbrella {\n", r, r, r);
        _dump_current_material();
        fprintf(mds->m_rayFile, "}))\n");
    }
    else
    {
        int divisions = 0;
        GLUquadricObj* gluq;

        switch (mds->m_quality)
        {
        case HIGH:
            divisions = 32; break;
        case MEDIUM:
            divisions = 20; break;
        case LOW:
            divisions = 12; break;
        case POOR:
            divisions = 8; break;
        }
        divisions = 10;
        double angle_sector = (2 * M_PI) / ((double)divisions); // 0 to 360
        double angle_y = (M_PI*angle) / 180; // 90 to 0
        double adjust_r = r + (angle / 100);
        std::vector<std::vector<double>> vertices;

        // Draw a half sphere 
        for (int i = 0; i < divisions+1; i++) {
            double angle_s = angle_sector * i;
            double x = adjust_r *sin(angle_s);
            double y = adjust_r *sin(angle_y);
            double z = adjust_r *cos(angle_s);
            std::vector<double> points;

            points = { x, y, z };

            vertices.push_back(points);
        }


        for (int i = 0; i < divisions; i++) { // Sector Line
            double height[3] = { 0, 0.5, 0 };
            std::vector<double> vertex1 = vertices[i];
            std::vector<double> vertex2 = vertices[i + 1];

            drawTriangle(height[0], height[1], height[2],
                         vertex1[0], vertex1[1], vertex1[2],
                         vertex2[0], vertex2[1], vertex2[2], false);
        }
    }
}

void drawBackground(char* fileLocation)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();
    
    unsigned char* background1 = NULL;
    int background1_height;
    int background1_width;

    _setupOpenGl();

    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        //fprintf(mds->m_rayFile,
            //"scale(%f,%f,%f,translate(0.5,0.5,0.5,box {\n", x, y, z);
        _dump_current_material();
        fprintf(mds->m_rayFile, "})))\n");
    }
    else
    {
        background1 = readBMP(fileLocation, background1_width, background1_height);
        
        if (background1 == NULL)
            std::cout << "not working  " << fileLocation << "\n";
                
        GLuint textureID = 0;
        glGenTextures(1, &textureID);
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, background1_width, background1_height, 0, GL_RGB, GL_UNSIGNED_BYTE, background1);             
         
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);     
        
        glBindTexture(GL_TEXTURE_2D, textureID);

        glPushMatrix();
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3d(0.0, 0.0, 0.0);
            glTexCoord2f(0.0f, 1.0f); glVertex3d(0.0, 10.0, 0.0);
            glTexCoord2f(1.0f, 1.0f); glVertex3d(10.0, 10.0, 0.0);
            glTexCoord2f(1.0f, 0.0f); glVertex3d(10.0, 0.0, 0.0);
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
 
    }
    
    delete background1;
    background1 = NULL;
    //delete background2;
    //background2 = NULL;
}

void drawMoon() {
    unsigned char* moon = NULL;
    int moon_height;
    int moon_width;
    moon = readBMP("textures/img3.bmp", moon_width, moon_height);
    GLuint textureID = 0;
    glGenTextures(1, &textureID);


    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, moon_width, moon_height, 0, GL_RGB, GL_UNSIGNED_BYTE, moon);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPushMatrix();
    drawSphere(0.5);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    delete moon;
    moon = NULL;
}
