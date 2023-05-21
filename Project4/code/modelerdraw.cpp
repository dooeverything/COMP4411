#include "modelerdraw.h"


//#include "marchingCube.h"

#include <FL/gl.h>
#include <GL/glu.h>
#include <cstdio>




// ********************************************************
// Support functions from previous version of modeler
// ********************************************************
void _dump_current_modelview(void)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();

    if (mds->m_rayFile == NULL)
    {
        fprintf(stderr, "No .ray file opened for writing, bailing out.\n");
        exit(-1);
    }

    GLdouble mv[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, mv);
    fprintf(mds->m_rayFile,
        "transform(\n    (%f,%f,%f,%f),\n    (%f,%f,%f,%f),\n     (%f,%f,%f,%f),\n    (%f,%f,%f,%f),\n",
        mv[0], mv[4], mv[8], mv[12],
        mv[1], mv[5], mv[9], mv[13],
        mv[2], mv[6], mv[10], mv[14],
        mv[3], mv[7], mv[11], mv[15]);
}

void _dump_current_material(void)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();

    if (mds->m_rayFile == NULL)
    {
        fprintf(stderr, "No .ray file opened for writing, bailing out.\n");
        exit(-1);
    }

    fprintf(mds->m_rayFile,
        "material={\n    diffuse=(%f,%f,%f);\n    ambient=(%f,%f,%f);\n}\n",
        mds->m_diffuseColor[0], mds->m_diffuseColor[1], mds->m_diffuseColor[2],
        mds->m_diffuseColor[0], mds->m_diffuseColor[1], mds->m_diffuseColor[2]);
}

// ****************************************************************************

// Initially assign singleton instance to NULL
ModelerDrawState* ModelerDrawState::m_instance = NULL;

ModelerDrawState::ModelerDrawState() : m_drawMode(NORMAL), m_quality(MEDIUM)
{
    float grey[] = { .5f, .5f, .5f, 1 };
    float white[] = { 1,1,1,1 };
    float black[] = { 0,0,0,1 };

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
    ModelerDrawState* mds = ModelerDrawState::Instance();

    mds->m_ambientColor[0] = (GLfloat)r;
    mds->m_ambientColor[1] = (GLfloat)g;
    mds->m_ambientColor[2] = (GLfloat)b;
    mds->m_ambientColor[3] = (GLfloat)1.0;

    if (mds->m_drawMode == NORMAL)
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mds->m_ambientColor);
}
void setAmbientAlphaColor(float r, float g, float b, float a)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();

    mds->m_ambientColor[0] = (GLfloat)r;
    mds->m_ambientColor[1] = (GLfloat)g;
    mds->m_ambientColor[2] = (GLfloat)b;
    mds->m_ambientColor[3] = (GLfloat)a;

    if (mds->m_drawMode == NORMAL)
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mds->m_ambientColor);

}

void setDiffuseColor(float r, float g, float b)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();

    mds->m_diffuseColor[0] = (GLfloat)r;
    mds->m_diffuseColor[1] = (GLfloat)g;
    mds->m_diffuseColor[2] = (GLfloat)b;
    mds->m_diffuseColor[3] = (GLfloat)1.0;

    if (mds->m_drawMode == NORMAL)
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mds->m_diffuseColor);
    else
        glColor3f(r, g, b);
}

void setSpecularColor(float r, float g, float b)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();

    mds->m_specularColor[0] = (GLfloat)r;
    mds->m_specularColor[1] = (GLfloat)g;
    mds->m_specularColor[2] = (GLfloat)b;
    mds->m_specularColor[3] = (GLfloat)1.0;

    if (mds->m_drawMode == NORMAL)
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mds->m_specularColor);
}

void setShininess(float s)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();

    mds->m_shininess = (GLfloat)s;

    if (mds->m_drawMode == NORMAL)
        glMaterialf(GL_FRONT, GL_SHININESS, mds->m_shininess);
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
    ModelerDrawState* mds = ModelerDrawState::Instance();

    fprintf(stderr, "Ray file format output is buggy (ehsu)\n");

    if (!rayFileName)
        return false;

    if (mds->m_rayFile)
        closeRayFile();

    mds->m_rayFile = fopen(rayFileName, "w");

    if (mds->m_rayFile != NULL)
    {
        fprintf(mds->m_rayFile, "SBT-raytracer 1.0\n\n");
        fprintf(mds->m_rayFile, "camera { fov=30; position=(0,0.8,5); direction=(0,-0.8,-5); }\n\n");
        fprintf(mds->m_rayFile,
            "directional_light { direction=(-1,-2,-1); color=(0.7,0.7,0.7); }\n\n");
        return true;
    }
    else
        return false;
}

void _setupOpenGl()
{
    ModelerDrawState* mds = ModelerDrawState::Instance();
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
    ModelerDrawState* mds = ModelerDrawState::Instance();

    if (mds->m_rayFile)
        fclose(mds->m_rayFile);

    mds->m_rayFile = NULL;
}

void drawSphere(double r)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();

    _setupOpenGl();

    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, "scale(%f,%f,%f,sphere {\n", r, r, r);
        _dump_current_material();
        fprintf(mds->m_rayFile, "}))\n");
    }
    else
    {
        int divisions = 12;
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

        gluq = gluNewQuadric();
        gluQuadricDrawStyle(gluq, GLU_FILL);
        gluQuadricTexture(gluq, GL_TRUE);
        gluSphere(gluq, r, divisions, divisions);
        gluDeleteQuadric(gluq);
    }
}

void drawHalfSphere(double r)
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

        double angle_sector = (2 * M_PI) / ((double)divisions - 1); // 0 to 360
        double angle_stack = (M_PI / 2) / ((double)divisions - 1); // 90 to 0

        std::vector<std::vector<double>> stack_vertices;

        // Draw a half sphere 
        for (int i = 0; i < divisions; i++) {
            double angle1 = angle_sector * i;
            for (int j = 0; j < divisions; j++) {
                double angle2 = (M_PI / 2) - angle_stack * j;

                std::vector<double> points;

                double x = r * cos(angle2) * sin(angle1); // x
                double y = r * sin(angle2);
                double z = r * cos(angle2) * cos(angle1);

                points = { x, y, z };
                double debug = points[0];
                stack_vertices.push_back(points);
            }
        }
        int count = 0;
        int sector_count = 0;
        for (int i = 0; i < divisions; i++) { // Sector Line
            sector_count++;
            for (int j = 0; j < divisions; j++) { // Stack Line
                std::vector<double> vertex1;
                std::vector<double> vertex2;
                std::vector<double >vertex3;
                std::vector<double >vertex4;
                int next_sector = count + divisions;;

                if (sector_count == divisions) {
                    next_sector = j + 1;
                }

                if (j == 0) {
                    vertex1 = stack_vertices[count];
                    vertex2 = stack_vertices[count + 1];
                    vertex3 = stack_vertices[next_sector];

                    drawTriangle(vertex1[0], vertex1[1], vertex1[2],
                        vertex2[0], vertex2[1], vertex2[2],
                        vertex3[0], vertex3[1], vertex3[2], true);

                    continue;
                }


                // Get 4 coordinate to draw a square
                vertex1 = stack_vertices[count];
                vertex2 = stack_vertices[count + 1];
                vertex3 = stack_vertices[next_sector];
                vertex4 = stack_vertices[next_sector + 1];

                // Draw a square

                // 1. Draw Upper Triangle
                drawTriangle(vertex1[0], vertex1[1], vertex1[2],
                    vertex2[0], vertex2[1], vertex2[2],
                    vertex3[0], vertex3[1], vertex3[2], false);

                // 2. Draw Lower Triangle
                drawTriangle(vertex4[0], vertex4[1], vertex4[2],
                    vertex2[0], vertex2[1], vertex2[2],
                    vertex3[0], vertex3[1], vertex3[2], true);


                count++;
            }
        }
    }
}

void drawBox(double x, double y, double z)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();

    _setupOpenGl();

    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile,
            "scale(%f,%f,%f,translate(0.5,0.5,0.5,box {\n", x, y, z);
        _dump_current_material();
        fprintf(mds->m_rayFile, "})))\n");
    }
    else
    {
        /* remember which matrix mode OpenGL was in. */
        int savemode;
        glGetIntegerv(GL_MATRIX_MODE, &savemode);

        /* switch to the model matrix and scale by x,y,z. */
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glScaled(x, y, z);

        glBegin(GL_QUADS);

        glNormal3d(0.0, 0.0, -1.0);
        glVertex3d(0.0, 0.0, 0.0); glVertex3d(0.0, 1.0, 0.0);
        glVertex3d(1.0, 1.0, 0.0); glVertex3d(1.0, 0.0, 0.0);

        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0); glVertex3d(1.0, 0.0, 0.0);
        glVertex3d(1.0, 0.0, 1.0); glVertex3d(0.0, 0.0, 1.0);

        glNormal3d(-1.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0); glVertex3d(0.0, 0.0, 1.0);
        glVertex3d(0.0, 1.0, 1.0); glVertex3d(0.0, 1.0, 0.0);

        glNormal3d(0.0, 0.0, 1.0);
        glVertex3d(0.0, 0.0, 1.0); glVertex3d(1.0, 0.0, 1.0);
        glVertex3d(1.0, 1.0, 1.0); glVertex3d(0.0, 1.0, 1.0);

        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(0.0, 1.0, 0.0); glVertex3d(0.0, 1.0, 1.0);
        glVertex3d(1.0, 1.0, 1.0); glVertex3d(1.0, 1.0, 0.0);

        glNormal3d(1.0, 0.0, 0.0);
        glVertex3d(1.0, 0.0, 0.0); glVertex3d(1.0, 1.0, 0.0);
        glVertex3d(1.0, 1.0, 1.0); glVertex3d(1.0, 0.0, 1.0);

        glEnd();

        /* restore the model matrix stack, and switch back to the matrix
        mode we were in. */
        glPopMatrix();
        glMatrixMode(savemode);
    }
}

void drawTextureBox(double x, double y, double z)
{
    // NOT IMPLEMENTED, SORRY (ehsu)
}

void drawCylinder(double h, double r1, double r2)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();
    int divisions;

    _setupOpenGl();

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

    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile,
            "cone { height=%f; bottom_radius=%f; top_radius=%f;\n", h, r1, r2);
        _dump_current_material();
        fprintf(mds->m_rayFile, "})\n");
    }
    else
    {
        GLUquadricObj* gluq;

        /* GLU will again do the work.  draw the sides of the cylinder. */
        gluq = gluNewQuadric();
        gluQuadricDrawStyle(gluq, GLU_FILL);
        gluQuadricTexture(gluq, GL_TRUE);
        gluCylinder(gluq, r1, r2, h, divisions, divisions);
        gluDeleteQuadric(gluq);

        if (r1 > 0.0)
        {
            /* if the r1 end does not come to a point, draw a flat disk to
                cover it up. */

            gluq = gluNewQuadric();
            gluQuadricDrawStyle(gluq, GLU_FILL);
            gluQuadricTexture(gluq, GL_TRUE);
            gluQuadricOrientation(gluq, GLU_INSIDE);
            gluDisk(gluq, 0.0, r1, divisions, divisions);
            gluDeleteQuadric(gluq);
        }

        if (r2 > 0.0)
        {
            /* if the r2 end does not come to a point, draw a flat disk to
                cover it up. */

                /* save the current matrix mode. */
            int savemode;
            glGetIntegerv(GL_MATRIX_MODE, &savemode);

            /* translate the origin to the other end of the cylinder. */
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslated(0.0, 0.0, h);

            /* draw a disk centered at the new origin. */
            gluq = gluNewQuadric();
            gluQuadricDrawStyle(gluq, GLU_FILL);
            gluQuadricTexture(gluq, GL_TRUE);
            gluQuadricOrientation(gluq, GLU_OUTSIDE);
            gluDisk(gluq, 0.0, r2, divisions, divisions);
            gluDeleteQuadric(gluq);

            /* restore the matrix stack and mode. */
            glPopMatrix();
            glMatrixMode(savemode);
        }
    }

}

void drawTriangle(double x1, double y1, double z1,
    double x2, double y2, double z2,
    double x3, double y3, double z3, bool smooth)
{
    ModelerDrawState* mds = ModelerDrawState::Instance();

    _setupOpenGl();

    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile,
            "polymesh { points=((%f,%f,%f),(%f,%f,%f),(%f,%f,%f)); faces=((0,1,2));\n", x1, y1, z1, x2, y2, z2, x3, y3, z3);
        _dump_current_material();
        fprintf(mds->m_rayFile, "})\n");
    }
    else
    {
        double a, b, c, d, e, f;

        /* the normal to the triangle is the cross product of two of its edges. */
        a = x2 - x1;
        b = y2 - y1;
        c = z2 - z1;

        d = x3 - x1;
        e = y3 - y1;
        f = z3 - z1;

        if (smooth) {
            glBegin(GL_TRIANGLES);
            //glNormal3d( b*f - c*e, c*d - a*f, a*e - b*d );
            glVertex3d(x1, y1, z1);
            glVertex3d(x2, y2, z2);
            glVertex3d(x3, y3, z3);
            glEnd();
        }
        else {
            glBegin(GL_TRIANGLES);
            glNormal3d(b * f - c * e, c * d - a * f, a * e - b * d);
            glVertex3d(x1, y1, z1);
            glVertex3d(x2, y2, z2);
            glVertex3d(x3, y3, z3);
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

void drawTotoroHead(double r)
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

        double angle_sector = (2 * M_PI) / ((double)divisions - 1); // 0 to 360
        double angle_stack = (M_PI / 2) / ((double)divisions - 1); // 90 to 0

        std::vector<std::vector<double>> stack_vertices;
        double half_division = divisions / 2;
        // Draw a half sphere 
        for (int i = 0; i < divisions; i++) {
            double angle1 = angle_sector * i;
            for (int j = 0; j < divisions; j++) {
                double angle2 = (M_PI / 2) - angle_stack * j;


                std::vector<double> points;

                double adjustment = ((half_division - j) / half_division) * 0.3;

                if (j == 0) {
                    adjustment = adjustment - 0.06;
                }
                else if (j == 1) {
                    adjustment = adjustment - 0.03;
                }
                else if (j == 2) {
                    adjustment = adjustment - 0.02;
                }

                double l = r - adjustment;

                if (angle2 >= 45 && angle2 < 80) {
                    l += 0.3 * j;
                }

                double x = l * cos(angle2) * sin(angle1);
                double y = l * sin(angle2);
                double z = l * cos(angle2) * cos(angle1);

                points = { x, y, z };
                double debug = points[0];
                stack_vertices.push_back(points);
            }
        }
        int count = 0;
        int sector_count = 0;
        for (int i = 0; i < divisions; i++) { // Sector Line
            sector_count++;
            for (int j = 0; j < divisions; j++) { // Stack Line
                std::vector<double> vertex1;
                std::vector<double> vertex2;
                std::vector<double >vertex3;
                std::vector<double >vertex4;
                int next_sector = count + divisions;;

                if (sector_count == divisions) {
                    next_sector = j + 1;
                }

                if (j == 0) {
                    vertex1 = stack_vertices[count];
                    vertex2 = stack_vertices[count + 1];
                    vertex3 = stack_vertices[next_sector];

                    drawTriangle(vertex1[0], vertex1[1], vertex1[2],
                        vertex2[0], vertex2[1], vertex2[2],
                        vertex3[0], vertex3[1], vertex3[2], true);
                    continue;
                }


                // Get 4 coordinate to draw a square
                vertex1 = stack_vertices[count];
                vertex2 = stack_vertices[count + 1];
                vertex3 = stack_vertices[next_sector];
                vertex4 = stack_vertices[next_sector + 1];

                /*if (j == divisions-1) {
                    continue;
                }*/

                // Draw a square

                // 1. Draw Upper Triangle
                drawTriangle(vertex1[0], vertex1[1], vertex1[2],
                    vertex2[0], vertex2[1], vertex2[2],
                    vertex3[0], vertex3[1], vertex3[2], false);

                // 2. Draw Lower Triangle
                drawTriangle(vertex4[0], vertex4[1], vertex4[2],
                    vertex2[0], vertex2[1], vertex2[2],
                    vertex3[0], vertex3[1], vertex3[2], true);


                count++;
            }
        }
    }
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
            double angle1 = (angle_each * M_PI / 180) * i;
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

                if (i == num_tube - 1) {
                    continue;
                }
                else if (j == divisions - 1) {
                    vertex1 = stack_vertices[count];
                    vertex2 = stack_vertices[count - j];
                    vertex3 = stack_vertices[next_sector];
                    vertex4 = stack_vertices[next_sector - j];
                }
                else {
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
        double angle_y = (M_PI * angle) / 180; // 90 to 0
        double adjust_r = r + (angle / 100);
        std::vector<std::vector<double>> vertices;

        // Draw a half sphere 
        for (int i = 0; i < divisions + 1; i++) {
            double angle_s = angle_sector * i;
            double x = adjust_r * sin(angle_s);
            double y = adjust_r * sin(angle_y);
            double z = adjust_r * cos(angle_s);
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
        glTexCoord2f(0.0f, 1.0f); glVertex3d(0.0, 8.0, 0.0);
        glTexCoord2f(1.0f, 1.0f); glVertex3d(15.0, 8.0, 0.0);
        glTexCoord2f(1.0f, 0.0f); glVertex3d(15.0, 0.0, 0.0);
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
    setAmbientColor(0.996, 0.988, 0.843);
    setDiffuseColor(0.996, 0.988, 0.843);
    drawSphere(0.5);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    delete moon;
    moon = NULL;
}

void drawRain(double x, double y, double z)
{
    glPushMatrix();
    //setAmbientColor(0, 0, 0);
    setAmbientColor(0.768, 0.827, 0.874);
    setDiffuseColor(0.768, 0.827, 0.874);
    float ran = (float)(rand() % 10)/100.0;
    //std::cout << "random is " << ran<< std::endl;
    glBegin(GL_LINES);
    glVertex3d(x, y, z);
    glVertex3d(x + ran, y - 0.2, z);
    glEnd();
    glPopMatrix();
}

void drawBreathParticle(double x, double y, double z) {

    unsigned char* tgaImage = NULL;
    int width;
    int height;
    int dataType;

    tgaImage = loadTGA("textures/tga/cloud.tga", width, height, dataType);

    if (tgaImage == NULL)
        std::cout << "not working  " << std::endl;
    
    //std::cout << "Width: " << width << " and Height: " << height << " and Data: " << dataType << std::endl;
   
    GLuint textureID = 0;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    
    if (dataType == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tgaImage);
    }
    else if (dataType == 4) {
        //std::cout << "Alpha" << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tgaImage);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_LINEAR

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPushMatrix();

        //glTranslated(x, y, z);
       
        setAmbientColor(1, 1, 1);
        setDiffuseColor(1, 1, 1);
    
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3d(0.0, 0.0, 0.0);
        glTexCoord2f(0.0f, 1.0f); glVertex3d(0.0, 1.0, 0.0);
        glTexCoord2f(1.0f, 1.0f); glVertex3d(1.0, 1.0, 0.0);
        glTexCoord2f(1.0f, 0.0f); glVertex3d(1.0, 0.0, 0.0);
        glEnd();
 
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawBezierSurface()
{
    ModelerDrawState* mds = ModelerDrawState::Instance();
    if (mds->ctrlPoints.size() <= 16 && mds->ctrlPoints.size() > 0) {
        /*glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glScalef(mds->scale, mds->scale, mds->scale);
        glRotatef(mds->yAngle, 0.0, 1.0, 0.0);
        glRotatef(mds->zAngle, 0.0, 0.0, 1.0);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();*/
   
        for (int i = 0; i < mds->ctrlPoints.size(); ++i) {

            float x = mds->ctrlPoints[i][0];
            float y = mds->ctrlPoints[i][1];
            float z = mds->ctrlPoints[i][2];

            setAmbientColor(1.0, 0.0, 0.0);
            setDiffuseColor(1.0, 0.0, 0.0);

            cout << "Draw a Point at " << x << " and " << y << " and " << z << endl;
            glPushMatrix();
                glTranslatef(x, y, z);
                drawBox(0.1, 0.1, 0.1);
            glPopMatrix();
        }

        //glEnd();

        return;
    } 


    CurveEvaluator* bezierCurve = new BezierCurveEvaluator();
    //bezierCurve->evaluateCurve(ctrlPoints, evaluatedPoints)
}


