///////////////////////////////////////////////////////////////////
// NON RECURSIVE FILLING OF RANDOMLY GENERATED CONVEX POLYGONS  //
//                                                             //
// Current Bugs:                                              //
//                                                           //
//////////////////////////////////////////////////////////////

#include <GL/glut.h>
#include <GL/glui.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
using namespace std;

const int windowSize[2] = {1080, 720}, initialPos[2] = {150, 30},
          guiPOS[2] = {initialPos[0] + windowSize[0], initialPos[1]};
int vertices = 6, thickness = 2, colorChoiceF = 100, colorChoiceL = 111, polThick = thickness;
float lowestX, highestX, lowestY, highestY, lowestYx, highestYx, centerX, centerY;
int mousex, mousey, graphicsWindow;
bool refresh = TRUE, cursor = FALSE;
vector<int> polygonPoints = {};

void myGlutIdle(void)
{
    glutSetWindow(graphicsWindow);
    glutPostRedisplay();
}

// GENERATE POLYGON VERTICES ================================================================================
vector<int> generatePolyPoints(int vert)
{
    const float PI = 3.14159265;
    lowestX = windowSize[0], highestX = 0, lowestY = windowSize[1], highestY = 0;
    vector<int> points(vert * 2);
    int R = rand() % min(windowSize[1] / 2 - 61, 500) + 50;
    int dxy = R + 10;
    int x0 = dxy + rand() % (windowSize[0] - 2 * dxy);
    int y0 = dxy + rand() % (windowSize[1] - 2 * dxy);
    float angles[vert];

    // Generate n random angles between 0 and 2 PI and then sort them
    for (int i = 0; i < vert; i++)
        angles[i] = ((rand() % 360 / vert) + i * 360 / vert) * (PI / 180);

    sort(angles, angles + vert);

    // Parametric Equations of Circle
    for (int i = 0; i < vert; i++)
    {
        // x = x0 + R*cos(th)
        points[i * 2] = x0 + R * cos(angles[i]);
        if (points[i * 2] < lowestX)
            lowestX = points[i * 2];
        if (points[i * 2] > highestX)
            highestX = points[i * 2];
        // y = y0 + R*sin(th)
        points[i * 2 + 1] = y0 + R * sin(angles[i]);
        if (points[i * 2 + 1] < lowestY)
        {
            lowestY = points[i * 2 + 1];
            lowestYx = points[i * 2];
        }
        if (points[i * 2 + 1] > highestY)
        {
            highestY = points[i * 2 + 1];
            highestYx = points[i * 2];
        }
    }
    centerX = x0;
    centerY = y0;
    return points;
}

// GET RGB COLOR ARRAY FROM SINGLE INTEGER ======================================================================
vector<int> getColor(int colorChoice)
{
    vector<int> colorArray = {};
    string colorS = to_string(colorChoice);
    string zeros(3 - colorS.length(), '0');
    colorS.insert(0, zeros);
    char r = colorS[0], g = colorS[1], b = colorS[2];
    colorArray = {(int)r % 2, (int)g % 2, (int)b % 2};
    return colorArray;
}

// DRAW POLYGON ===============================================================================================
void drawPolygon()
{
    vector<int> colorL = getColor(colorChoiceL);
    // GL_POINT = draw polygon points, GL_LINE = draw polygon edges, GL_FILL = draw filled polygon
    glPolygonMode(GL_FRONT, GL_LINE);
    glBegin(GL_POLYGON);
    glColor3f(colorL[0], colorL[1], colorL[2]);
    for (int i = 0; i < polygonPoints.size() / 2; i++)
        glVertex2i(polygonPoints[i * 2], polygonPoints[i * 2 + 1]);
    glEnd();
}

// CHECK IF POSITION IS INSIDE POLYGON ===================================================================
bool checkPosition()
{
    float pixelColor1[3], pixelColor2[3];
    int y = mousey;
    bool leftLine = FALSE, rightLine = FALSE, isInside = FALSE;

    // Check for difference in colour between 2 neighbor pixel to the right
    // of mouse position
    for (int x = mousex; x <= highestX + thickness; x++)
    {
        glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixelColor1);
        glReadPixels(x + 1, y, 1, 1, GL_RGB, GL_FLOAT, pixelColor2);
        if (pixelColor1[0] == pixelColor2[0] || pixelColor1[1] == pixelColor2[1] ||
            pixelColor1[2] == pixelColor2[2])
        {
            rightLine = TRUE;
            break;
        }
    }

    // Check for difference in colour between 2 neighbor pixel to the left
    // of mouse position
    if (rightLine)
    {
        for (int x = mousex; x >= lowestX - thickness; x--)
        {
            glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixelColor1);
            glReadPixels(x - 1, y, 1, 1, GL_RGB, GL_FLOAT, pixelColor2);
            if (pixelColor1[0] != pixelColor2[0] || pixelColor1[1] != pixelColor2[1] ||
                pixelColor1[2] != pixelColor2[2])
            {
                leftLine = TRUE;
                break;
            }
        }
    }

    if (leftLine && rightLine)
    {
        isInside = TRUE;
    }

    return isInside;
}

// GET POINTS of INTERSEPTION WITH POLYGON POINTS =============================================================
vector<int> getLinePoints(int y, bool isInside)
{
    vector<int> interseptionPoints;
    vector<int> colorL = getColor(colorChoiceL);
    float pixelColor1[3], pixelColor2[3];
    int xStartL, xStartR;
    double a, b;

    // If inside the starting position of x is following 2 lines between
    // from x cordinate of lowestY to the center of the polygone and
    // from the center of the polygon to the x cordinate of the highestY
    // If not inside x starts from lowestX (for moving right) and
    // highestX (for moving left)
    if (isInside)
    {
        if (y < centerY)
        {
            // Slope a = (y2 - y1) / (x2 - x1)
            a = ((centerY - lowestY) / (centerX - lowestYx));
            // y2 = a * x2 + b
            b = (centerY - a * centerX);
        }
        else
        {
            // Slope a = (y2 - y1) / (x2 - x1)
            a = ((highestY - centerY) / (highestYx - centerX));
            // y2 = a * x2 + b
            b = (highestY - a * highestYx);
        }
        // x = (y - b)/a
        xStartL = (y - b) / a;
        xStartR = xStartL;
    }
    else
    {
        xStartL = lowestX - thickness - 1;
        xStartR = highestX + thickness + 1;
    }

    // Find P.O.I with Polygon Line on Right Side
    for (int x = xStartL; x < windowSize[0]; x++)
    {
        glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixelColor1);
        glReadPixels(x + 1, y, 1, 1, GL_RGB, GL_FLOAT, pixelColor2);
        if (pixelColor1[0] != pixelColor2[0] || pixelColor1[1] != pixelColor2[1] ||
            pixelColor1[2] != pixelColor2[2])
        {
            if (isInside)
                interseptionPoints.push_back(x + 1);
            else
                interseptionPoints.push_back(x + 2);
            break;
        }
    }

    // Find P.O.I with Polygon Line on Left Side
    for (int x = xStartR; x > 0; x--)
    {
        glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixelColor1);
        glReadPixels(x - 1, y, 1, 1, GL_RGB, GL_FLOAT, pixelColor2);
        if (pixelColor1[0] != pixelColor2[0] || pixelColor1[1] != pixelColor2[1] ||
            pixelColor1[2] != pixelColor2[2])
        {
            if (isInside)
                interseptionPoints.push_back(x - 1);
            else
                interseptionPoints.push_back(x - 2);
            break;
        }
    }

    // If not inside add 0 at the beginning and the lenght of window
    // at the end of the list
    if (!isInside)
    {
        interseptionPoints.insert(interseptionPoints.begin(), 0);
        interseptionPoints.insert(interseptionPoints.end(), windowSize[0]);
    }

    return interseptionPoints;
}

// FILL =======================================================================================================
void fillFunction()
{
    vector<int> colorF = getColor(colorChoiceF);
    vector<int> colorL = getColor(colorChoiceL);
    vector<int> interseptionPoints = {};
    float polyCPixel[3], cornerPixel[3];
    bool isInside = checkPosition(), changeLineColor = FALSE;

    glReadPixels(centerX, centerY, 1, 1, GL_RGB, GL_FLOAT, polyCPixel);
    glReadPixels(0, 0, 1, 1, GL_RGB, GL_FLOAT, cornerPixel);

    // If the colour of the line and the colour of the corner Pixel are the same
    if (colorL[0] == cornerPixel[0] && colorL[1] == cornerPixel[1] && colorL[2] == cornerPixel[2])
    {
        // If the colour of the line and the colour of the polygon center pixel are the same
        if (polyCPixel[0] == colorL[0] && polyCPixel[1] == colorL[1] && polyCPixel[2] == colorL[2])
            refresh = TRUE;
        if (!isInside)
            changeLineColor = TRUE;
    }
    // If the colour of the line and the colour of the polygon center pixel are the same
    // and we are inside
    if (colorL[0] == polyCPixel[0] && colorL[1] == polyCPixel[1] &&
        colorL[2] == polyCPixel[2] && isInside)
        changeLineColor = TRUE;

    if (refresh)
        changeLineColor = TRUE;
    else
    {
        cout << "Begin Filling\n";
        glColor3f(colorF[0], colorF[1], colorF[2]);
        glLineWidth(1);
        for (int y = 0; y < windowSize[1]; y++)
        {
            if (y > lowestY && y < highestY)
            {
                interseptionPoints = getLinePoints(y, isInside);
            }
            else
            {
                interseptionPoints = {};
                if (!isInside)
                    interseptionPoints = {0, windowSize[0]};
            }

            for (int i = 0; i < interseptionPoints.size(); i++)
            {
                if (!(i % 2))
                {
                    glBegin(GL_LINES);
                    glVertex2i(interseptionPoints[i], y);
                    glVertex2i(interseptionPoints[i + 1], y);
                    glEnd();
                }
            }
            float pct = (y + 1) * 100 / windowSize[1];
            cout << "Progress " << pct << "%\n";
            glFlush();
        }
        cout << "Filling Ended\n";
    }
    if (changeLineColor)
        colorChoiceL = colorChoiceF;
    glLineWidth(polThick);
    drawPolygon();
    glFlush();
}

// HANDLE THE PRESS OF "FILL" BUTTON ==========================================================================
void fillButton(int control)
{
    cursor = TRUE;
}

// HANDLE THE PRESS OF A MOUSE BUTTON ==========================================================================
void myGlutMouse(int button, int button_state, int x, int y)
{
    // If LMB is pressed check if mouse inside polygon
    // If RMB is pressed refresh polygon
    if (button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN)
    {
        mousex = x;
        mousey = windowSize[1] - y;
        if (cursor)
            fillFunction();
        cursor = FALSE;
    }
}

// HANDLE THE PRESS OF "REFRESH" BUTTON ========================================================================
void refreshButton(int control)
{
    refresh = TRUE;
}

// RENDER GRAPHICS ============================================================================================
void renderGraphics()
{
    glutMouseFunc(myGlutMouse);

    if (cursor)
        glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    else
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

    if (refresh)
    {
        polThick = thickness;
        vector<int> colorF = getColor(colorChoiceF);
        glClearColor(colorF[0], colorF[1], colorF[2], 0);
        glClear(GL_COLOR_BUFFER_BIT);
        polygonPoints = generatePolyPoints(vertices);
        glLineWidth(polThick);
        refresh = FALSE;
    }
    // Draw Polygon
    drawPolygon();
    glFlush();
}

// MAIN =======================================================================================================
int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitWindowPosition(initialPos[0], initialPos[1]);
    glutInitWindowSize(windowSize[0], windowSize[1]);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    graphicsWindow = glutCreateWindow("GraphicsWindow");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowSize[0], 0, windowSize[1]);
    glMatrixMode(GL_MODELVIEW);
    glutDisplayFunc(renderGraphics);

    GLUI_Master.set_glutIdleFunc(myGlutIdle);
    GLUI *glui = GLUI_Master.create_glui("GLUI", 0, guiPOS[0], guiPOS[1]);
    glui->add_spinner("Vertices:", GLUI_SPINNER_INT, &vertices)->set_int_limits(3, 100);
    glui->add_spinner("Thickness:", GLUI_SPINNER_INT, &thickness)->set_int_limits(1, 7);
    GLUI_Listbox *colorListBoxB = new GLUI_Listbox(glui, "Fill Color", &colorChoiceF);
    colorListBoxB->add_item(100, "RED");
    colorListBoxB->add_item(10, "GREEN");
    colorListBoxB->add_item(1, "BLUE");
    colorListBoxB->add_item(111, "WHITE");
    colorListBoxB->add_item(0, "BLACK");
    GLUI_Listbox *colorListBoxL = new GLUI_Listbox(glui, "Line Color", &colorChoiceL);
    colorListBoxL->add_item(111, "WHITE");
    colorListBoxL->add_item(100, "RED");
    colorListBoxL->add_item(10, "GREEN");
    colorListBoxL->add_item(1, "BLUE");
    colorListBoxL->add_item(0, "BLACK");
    glui->add_button("Fill", 1, fillButton);
    glui->set_main_gfx_window(graphicsWindow);
    glui->add_button("REFRESH", 0, refreshButton);

    glutMainLoop();

    return 0;
}