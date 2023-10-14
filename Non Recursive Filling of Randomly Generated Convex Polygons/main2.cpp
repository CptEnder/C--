// ///////////////////////////////////////////////////////////////////
// // NON RECURSIVE FILLING OF RANDOMLY GENERATED CONVEX POLYGONS  //
// //                                                             //
// // Current Bugs:                                              //
// //                                                           //
// //////////////////////////////////////////////////////////////

// #include <GL/glut.h>
// #include <GL/glui.h>
// #include <math.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <iostream>
// #include <string.h>
// #include <bits/stdc++.h>
// using namespace std;

// const int windowSize[2] = {300, 300}, initialPos[2] = {150, 30},
//           guiPOS[2] = {initialPos[0] + windowSize[0], initialPos[1]};
// int vertices = 4, thickness = 2, colorChoiceF = 100, colorChoiceL = 111, polThick = thickness;
// int lowestX, highestX, lowestY, highestY;
// int mousex, mousey, graphicsWindow;
// bool refresh = TRUE, cursor = FALSE;
// vector<int> polygonPoints = {};

// void myGlutIdle(void)
// {
//     glutSetWindow(graphicsWindow);
//     glutPostRedisplay();
// }

// // GENERATE POLYGON VERTICES ================================================================================
// vector<int> generatePolyPoints(int vert)
// {
//     const float PI = 3.14159265;
//     lowestX = windowSize[0], highestX = 0, lowestY = windowSize[1], highestY = 0;
//     vector<int> points(vert * 2);
//     int R = rand() % min(windowSize[1] / 2 - 61, 500) + 50;
//     int dxy = R + 10;
//     int x0 = dxy + rand() % (windowSize[0] - 2 * dxy);
//     int y0 = dxy + rand() % (windowSize[1] - 2 * dxy);
//     float angles[vert];

//     // Generate n random angles between 0 and 2 PI and then sort them
//     for (int i = 0; i < vert; i++)
//         angles[i] = ((rand() % 360 / vert) + i * 360 / vert) * (PI / 180);

//     sort(angles, angles + vert);

//     // Parametric Equations of Circle
//     for (int i = 0; i < vert; i++)
//     {
//         // x = x0 + R*cos(th)
//         points[i * 2] = x0 + R * cos(angles[i]);
//         if (points[i * 2] < lowestX)
//             lowestX = points[i * 2];
//         if (points[i * 2] > highestX)
//             highestX = points[i * 2];
//         // y = y0 + R*sin(th)
//         points[i * 2 + 1] = y0 + R * sin(angles[i]);
//         if (points[i * 2 + 1] < lowestY)
//             lowestY = points[i * 2 + 1];
//         if (points[i * 2 + 1] > highestY)
//             highestY = points[i * 2 + 1];
//     }
//     return points;
// }

// // GET RGB COLOR ARRAY FROM SINGLE INTEGER ======================================================================
// vector<int> getColor(int colorChoice)
// {
//     vector<int> colorArray = {};
//     string colorS = to_string(colorChoice);
//     string zeros(3 - colorS.length(), '0');
//     colorS.insert(0, zeros);
//     char r = colorS[0], g = colorS[1], b = colorS[2];
//     colorArray = {(int)r % 2, (int)g % 2, (int)b % 2};
//     return colorArray;
// }

// // DRAW POLYGON ===============================================================================================
// void drawPolygon()
// {
//     vector<int> colorL = getColor(colorChoiceL);
//     // GL_POINT = draw polygon points, GL_LINE = draw polygon edges, GL_FILL = draw filled polygon
//     glPolygonMode(GL_FRONT, GL_LINE);
//     glBegin(GL_POLYGON);
//     glColor3f(colorL[0], colorL[1], colorL[2]);
//     for (int i = 0; i < polygonPoints.size() / 2; i++)
//         glVertex2i(polygonPoints[i * 2], polygonPoints[i * 2 + 1]);
//     glEnd();
// }

// // CHECK IF POSITION IS INSIDE POLYGON ===================================================================
// bool checkPosition()
// {
//     vector<int> colorL = getColor(colorChoiceL);
//     float colorPixel[3], polyCPixel[3], cornerPixel[3];
//     int y = mousey;
//     int centerX = lowestX + (highestX - lowestX) / 2, centerY = lowestY + (highestY - lowestY) / 2;
//     bool leftLine = FALSE, rightLine = FALSE, isInside = FALSE;
//     for (int i = mousex; i < windowSize[0]; i++)
//     {
//         glReadPixels(i + 1, y, 1, 1, GL_RGB, GL_FLOAT, colorPixel);
//         if (colorPixel[0] == colorL[0] && colorPixel[1] == colorL[1] && colorPixel[2] == colorL[2])
//         {
//             leftLine = TRUE;
//             break;
//         }
//     }

//     if (leftLine)
//     {
//         for (int i = mousex; i > 0; i--)
//         {
//             glReadPixels(i + 1, y, 1, 1, GL_RGB, GL_FLOAT, colorPixel);
//             if (colorPixel[0] == colorL[0] && colorPixel[1] == colorL[1] && colorPixel[2] == colorL[2])
//             {
//                 rightLine = TRUE;
//                 break;
//             }
//         }
//     }

//     if (leftLine && rightLine)
//     {

//         isInside = TRUE;
//     }
//     glReadPixels(centerX, centerY, 1, 1, GL_RGB, GL_FLOAT, polyCPixel);
//     glReadPixels(0, 0, 1, 1, GL_RGB, GL_FLOAT, cornerPixel);
//     if (polyCPixel[0] == cornerPixel[0] && polyCPixel[1] == cornerPixel[1] && polyCPixel[2] == cornerPixel[2])
//         if (polyCPixel[0] == colorL[0] && polyCPixel[1] == colorL[1] && polyCPixel[2] == colorL[2])
//             refresh = TRUE;

//     return isInside;
// }

// // GET POINTS of INTERSEPTION WITH POLYGON POINTS =============================================================
// vector<int> getLinePoints(int y, bool isInside)
// {
//     vector<int> interseptionPoints;
//     vector<int> colorL = getColor(colorChoiceL);
//     float colorPixel[3];

//     // Find P.O.I with Polygon Line from Left Side
//     for (int x = lowestX - thickness; x < windowSize[0]; x++)
//     {
//         glReadPixels(x + 1, y, 1, 1, GL_RGB, GL_FLOAT, colorPixel);
//         if (colorPixel[0] == colorL[0] && colorPixel[1] == colorL[1] && colorPixel[2] == colorL[2])
//         {
//             interseptionPoints.push_back(x + polThick / 2);
//             break;
//         }
//     }

//     // Find P.O.I with Polygon Line from Right Side
//     for (int x = highestX + thickness; x > 0; x--)
//     {
//         glReadPixels(x - 1, y, 1, 1, GL_RGB, GL_FLOAT, colorPixel);
//         if (colorPixel[0] == colorL[0] && colorPixel[1] == colorL[1] && colorPixel[2] == colorL[2])
//         {
//             interseptionPoints.push_back(x - polThick / 2);
//             break;
//         }
//     }

//     if (!isInside)
//     {
//         interseptionPoints.insert(interseptionPoints.begin(), 0);
//         interseptionPoints.insert(interseptionPoints.end(), windowSize[0]);
//     }

//     return interseptionPoints;
// }

// // FILL =======================================================================================================
// void fillFunction()
// {
//     vector<int> colorF = getColor(colorChoiceF);
//     vector<int> interseptionPoints = {};
//     bool isInside = checkPosition();
//     cout << "Begin Filling\n";

//     glColor3f(colorF[0], colorF[1], colorF[2]);
//     glLineWidth(1);
//     for (int y = 0; y < windowSize[1]; y++)
//     {
//         if (y >= lowestY && y <= highestY)
//         {
//             interseptionPoints = getLinePoints(y, isInside);
//         }
//         else
//         {
//             interseptionPoints = {};
//             if (!isInside)
//                 interseptionPoints = {0, windowSize[0]};
//         }

//         for (int i = 0; i < interseptionPoints.size(); i++)
//         {
//             if (!(i % 2))
//             {
//                 glBegin(GL_LINES);
//                 glVertex2i(interseptionPoints[i], y);
//                 glVertex2i(interseptionPoints[i + 1], y);
//                 glEnd();
//             }
//         }
//         float pct = (y + 1) * 100 / windowSize[1];
//         cout << "Progress " << pct << "%\n";
//         glFlush();
//     }
//     cout << "Filling Ended\n";
//     if (refresh)
//         colorChoiceL = colorChoiceF;
//     glLineWidth(polThick);
//     drawPolygon();
//     glFlush();
// }

// // HANDLE THE PRESS OF "FILL" BUTTON ==========================================================================
// void fillButton(int control)
// {
//     cursor = TRUE;
// }

// // HANDLE THE PRESS OF A MOUSE BUTTON ==========================================================================
// void myGlutMouse(int button, int button_state, int x, int y)
// {
//     // If LMB is pressed check if mouse inside polygon
//     // If RMB is pressed refresh polygon
//     if (button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN)
//     {
//         mousex = x;
//         mousey = windowSize[1] - y;
//         if (cursor)
//             fillFunction();
//         cursor = FALSE;
//     }
// }

// // HANDLE THE PRESS OF "REFRESH" BUTTON ========================================================================
// void refreshButton(int control)
// {
//     refresh = TRUE;
// }

// // RENDER GRAPHICS ============================================================================================
// void renderGraphics()
// {
//     glutMouseFunc(myGlutMouse);

//     if (cursor)
//         glutSetCursor(GLUT_CURSOR_CROSSHAIR);
//     else
//         glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

//     if (refresh)
//     {
//         polThick = thickness;
//         vector<int> colorF = getColor(colorChoiceF);
//         glClearColor(colorF[0], colorF[1], colorF[2], 0);
//         glClear(GL_COLOR_BUFFER_BIT);
//         polygonPoints = generatePolyPoints(vertices);
//         glLineWidth(polThick);
//         refresh = FALSE;
//     }
//     // Draw Polygon
//     drawPolygon();
//     glFlush();
// }

// // MAIN =======================================================================================================
// int main(int argc, char **argv)
// {

//     glutInit(&argc, argv);
//     glutInitWindowPosition(initialPos[0], initialPos[1]);
//     glutInitWindowSize(windowSize[0], windowSize[1]);
//     glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//     graphicsWindow = glutCreateWindow("GraphicsWindow");
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     gluOrtho2D(0, windowSize[0], 0, windowSize[1]);
//     glMatrixMode(GL_MODELVIEW);
//     glutDisplayFunc(renderGraphics);

//     GLUI_Master.set_glutIdleFunc(myGlutIdle);
//     GLUI *glui = GLUI_Master.create_glui("GLUI", 0, guiPOS[0], guiPOS[1]);
//     glui->add_spinner("Vertices:", GLUI_SPINNER_INT, &vertices)->set_int_limits(3, 11);
//     glui->add_spinner("Thickness:", GLUI_SPINNER_INT, &thickness)->set_int_limits(1, 7);
//     GLUI_Listbox *colorListBoxB = new GLUI_Listbox(glui, "Fill Color", &colorChoiceF);
//     colorListBoxB->add_item(100, "RED");
//     colorListBoxB->add_item(10, "GREEN");
//     colorListBoxB->add_item(1, "BLUE");
//     colorListBoxB->add_item(111, "WHITE");
//     colorListBoxB->add_item(0, "BLACK");
//     GLUI_Listbox *colorListBoxL = new GLUI_Listbox(glui, "Line Color", &colorChoiceL);
//     colorListBoxL->add_item(111, "WHITE");
//     colorListBoxL->add_item(100, "RED");
//     colorListBoxL->add_item(10, "GREEN");
//     colorListBoxL->add_item(1, "BLUE");
//     colorListBoxL->add_item(0, "BLACK");
//     glui->add_button("Fill", 1, fillButton);
//     glui->set_main_gfx_window(graphicsWindow);
//     glui->add_button("REFRESH", 0, refreshButton);

//     glutMainLoop();

//     return 0;
// }