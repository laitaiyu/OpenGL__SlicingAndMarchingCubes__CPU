
#pragma once

#include <stdio.h>
#include <signal.h>

#include "inputModule.h"
#include "PLY.h"

extern PLYObject *ply;

static int motionMode;
static int startX;
static int startY;
static GLfloat angle = 20;    /* in degrees */
static GLfloat angle2 = 30;   /* in degrees */

GLfloat current_pos[] = {0.0, 0.0, 5.0};
GLfloat current_pos_bunny[] = {0.0, 0.0, 0.0};
GLfloat current_rot_bunny[] = {0.0, 0.0, 0.0};
GLfloat current_sca_bunny[] = {1.0, 1.0, 1.0};

int record = 0;
int box = 0;
int wire = 1;
int light = 0;
int flat = 1;
int texture = 0;
int ambient=1, diffuse=1, specular=1;


int path[256];
int iKeyT;
int iKeyX;
int iKeyY;
int iKeyZ;
float fKeyA;
float fKeyS;
float fKeyD;
int iKeyF1;
int iKeyF2;
int iKeyF3;


int iPreviousX = 0;
int iPreviousY = 0;
extern bool bAnyMoveZ;
int iAnyMoveZ = 0;
extern bool bAnyMoveX;
int iAnyMoveX = 0;
extern bool bAnyMoveY;
int iAnyMoveY = 0;
extern bool bAnyRotateZ;
int iAnyRotateZ = 0;
extern bool bAnyRotateX;
int iAnyRotateX = 0;
extern bool bAnyRotateY;
int iAnyRotateY = 0;

extern int iAddTriangles;
extern int iDeleteTriangles;
extern int iMoveTriangles;
extern int iChangeColor;

extern int iHistogram[256];
extern GLfloat fAlpha[256];


extern float rgba[256][4];
extern std::list<LittleTriangle> mylist;
std::list<LittleTriangle>::iterator itMove;
bool bMove = false;



bool compare_list(LittleTriangle first, LittleTriangle second)
{
    if(first.x < second.x)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void fillColor ( LittleTriangle t1, LittleTriangle t2 )
{
    RGBType rgb1 = HSV_to_RGB(t1.hsv);
    RGBType rgb2 = HSV_to_RGB(t2.hsv);

    float frac;

    for ( int i = t1.x; i<= t2.x; i++ )
    {
        frac = ((float)(i-t1.x)) / (t2.x - t1.x);
        rgba[i][0] = (1.0-frac) * rgb1.R + (frac) * rgb2.R;
        rgba[i][1] = (1.0-frac) * rgb1.G + (frac) * rgb2.G;
        rgba[i][2] = (1.0-frac) * rgb1.B + (frac) * rgb2.B;
        rgba[i][3] = (1.0);
    }
}


void F123Keyboard(int key, int x, int y)
{
    //iKeyF1 = 0;
    //iKeyF2 = 0;
    //iKeyF3 = 0;
    //switch(key)
    //{
    //case 1:
    //    iKeyF1 = !iKeyF1;
    //    break;
    //case 2:
    //    iKeyF2 = !iKeyF2;
    //    break;
    //case 3:
    //    iKeyF3 = !iKeyF3;
    //    break;
    //}
}

void readKeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
    case  0x1B:
    case  'q':
    case  'Q':
        delete(ply);
        exit(0);
        break;
    case 'i':
    case 'I':
        if (ply)
            ply->invertNormals();
        break;
    case 'v':
    case 'V':
        record = (record + 1) % 2; //---
        break;
    case 'b':
    case 'B':
        box = (box + 1) % 2;
        break;
    case 'w':
    case 'W':
        wire = (wire + 1) % 2;
        break;
    case 'h':
    case 'H':
        printf("\tpress q/Q for quit\n");
        printf("\tpress i/I to invert normals\n");
        printf("\tpress b/B to display bounding box\n");
        printf("\tpress w/W to toggle wire frame rendering\n");
        printf("\tpress e/E to revert viewpoint to initial positions\n");
        
        printf("\tpress x/X to rotate PLY object\n"); // rotate
        printf("\tpress Y/Y to rotate PLY object\n");
        printf("\tpress z/Z to rotate PLY object\n");
        printf("\tpress a/A to scale PLY object\n"); // scale
        printf("\tpress s/S to scale PLY object\n");
        printf("\tpress d/D to scale PLY object\n");
        
        printf("\tpress t/T to enable/disable texture mapping on the floor\n");
        break;
        
        // rotate ------------------------------------
    case 'x':
        
        // fill_me...
        iKeyX = ( iKeyX + 10 ) % 360;
        break;
    case 'X':
        
        // fill_me...
        iKeyX = ( iKeyX + 10 ) % 360;
       break;
    case 'y':
        
        // fill_me...
        iKeyY = ( iKeyY + 10 ) % 360;
        break;
    case 'Y':
        
        // fill_me...
        iKeyY = ( iKeyY + 10 ) % 360;
        break;
    case 'z':
        
        // fill_me...
        iKeyZ = ( iKeyZ + 10 ) % 360;
        break;
    case 'Z':
        
        // fill_me...
        iKeyZ = ( iKeyZ + 10 ) % 360;
        break;
        
        // scale ------------------------------------
    case 'a':
        
        // fill_me...
        fKeyA = fKeyA + 0.1f ;
        if ( fKeyA >= 1.1f ) { fKeyA = 0.0f; }
        break;
    case 'A':
        
        // fill_me...
        fKeyA = fKeyA + 0.1f ;
        if ( fKeyA >= 2.1f ) { fKeyA = 0.0f; }
        break;
    case 'd':
        
        // fill_me...
        fKeyD = fKeyD + 0.1f ;
        if ( fKeyD >= 2.1f ) { fKeyD = 0.0f; }
        break;
    case 'D':
        
        // fill_me...
        fKeyD = fKeyD + 0.1f ;
        if ( fKeyD >= 2.1f ) { fKeyD = 0.0f; }
        break;
    case 's':
        
        // fill_me...
        fKeyS = fKeyS + 0.1f ;
        if ( fKeyS >= 2.1f ) { fKeyS = 0.0f; }
        break;
    case 'S':
        
        // fill_me...
        fKeyS = fKeyS + 0.1f ;
        if ( fKeyS >= 2.1f ) { fKeyS = 0.0f; }
        break;
        // -------------------------------------------
    case 'e':
    case 'E':
        // reset initial view parameters
        angle = 20;
        angle2 = 30;
        current_pos[0] = 0.0;
        current_pos[1] = 0.0;
        current_pos[2] = 5.0;
        // reset rendering parameters
        flat = wire = 1;
        record = box = light = texture = 0;
        ambient = diffuse = specular = 1;
        break;
    case 't':
    case 'T':
        iKeyT = !iKeyT;
        break;
    case 'l':
    case 'L':
        light = (light + 1) % 2;
        break;
    case '4':
        bAnyMoveZ = false;
        bAnyMoveX = false;
        bAnyMoveY = false;
        bAnyRotateZ = false;
        bAnyRotateX = true;
        bAnyRotateY = false;
        break;
    case '5':
        bAnyMoveZ = false;
        bAnyMoveX = false;
        bAnyMoveY = false;
        bAnyRotateZ = false;
        bAnyRotateX = false;
        bAnyRotateY = true;
        break;
    case '6':
        bAnyMoveZ = false;
        bAnyMoveX = false;
        bAnyMoveY = false;
        bAnyRotateZ = true;
        bAnyRotateX = false;
        bAnyRotateY = false;
        break;
    case '7':
        bAnyMoveZ = false;
        bAnyMoveX = true;
        bAnyMoveY = false;
        bAnyRotateZ = false;
        bAnyRotateX = false;
        bAnyRotateY = false;
        break;
    case '8':
        bAnyMoveZ = false;
        bAnyMoveX = false;
        bAnyMoveY = true;
        bAnyRotateZ = false;
        bAnyRotateX = false;
        bAnyRotateY = false;
        break;
    case '9':
        bAnyMoveZ = true;
        bAnyMoveX = false;
        bAnyMoveY = false;
        bAnyRotateZ = false;
        bAnyRotateX = false;
        bAnyRotateY = false;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void mouseButtHandler(int button, int state, int x, int y)
{
  motionMode = 0;
  
  switch(button){
  case GLUT_LEFT_BUTTON:  
    if(state == GLUT_DOWN) {
      motionMode = 1;		// Rotate object
      startX = x;
      startY = y;
    }
    break;
  case GLUT_MIDDLE_BUTTON:  
    if(state == GLUT_DOWN) {
      motionMode = 2;		// Translate object
      startX = x;
      startY = y;
    }
    break;
  case GLUT_RIGHT_BUTTON: 
    if(state == GLUT_DOWN) { 
      motionMode = 3;		// Zoom
      startX = x;
      startY = y;
    }
    break;
  }
  glutPostRedisplay();
}

void special_callback(int key, int x, int y)
{
    switch (key) 
    {
    case GLUT_KEY_DOWN:
        
        // fill_me...
        current_pos_bunny[2]+=0.1f;
        printf("%f\n", current_pos_bunny[2]);
        break;
    case GLUT_KEY_UP:
        
        // fill_me...
        current_pos_bunny[2]-=0.1f;
        printf("%f\n", current_pos_bunny[2]);
        break;
    case GLUT_KEY_LEFT:
        
        // fill_me...
        current_pos_bunny[0]-=0.1f;
        printf("%f\n", current_pos_bunny[0]);
        break;
    case GLUT_KEY_RIGHT:
        
        // fill_me...
        current_pos_bunny[0]+=0.1f;
        printf("%f\n", current_pos_bunny[0]);
        break;
    case GLUT_KEY_F1:
        ambient = !ambient;
        break;
    case GLUT_KEY_F2:
        diffuse = !diffuse;
        break;
    case GLUT_KEY_F3:
        specular = !specular;
        break;
    }
}

void mouseMoveHandler(int x, int y)
{
    if ( bAnyMoveZ == false && bAnyMoveX == false && bAnyMoveY == false && bAnyRotateZ == false && bAnyRotateX == false && bAnyRotateY == false )
    {
        // No mouse button is pressed... return 
        switch(motionMode)
        {
        case 0:
            return;
            break;

        case 1: // Calculate the rotations
            angle = angle + (x - startX);
            angle2 = angle2 + (y - startY);
            startX = x;
            startY = y;
            break;

        case 2:
            current_pos[0] = current_pos[0] - (x - startX)/100.0;
            current_pos[1] = current_pos[1] - (y - startY)/100.0;
            startX = x;
            startY = y;
            break;

        case 3:
            current_pos[2] = current_pos[2] - (y - startY)/10.0;
            startX = x;
            startY = y;
            break;
        }

        glutPostRedisplay();
    }
    else if ( bAnyMoveZ == true )
    {
        switch(motionMode)
        {
        case 0:
            return;
            break;

        case 1: 
            if ( iPreviousX > x || iPreviousY < y )
            {
                iAnyMoveZ+=2;
            }
            else if ( iPreviousX < x || iPreviousY > y )
            {
                iAnyMoveZ-=2;
            }
            break;
        }
    }
    else if ( bAnyMoveX == true )
    {
        switch(motionMode)
        {
        case 0:
            return;
            break;

        case 1: 
            if ( iPreviousX > x || iPreviousY < y )
            {
                iAnyMoveX-=2;
            }
            else if ( iPreviousX < x || iPreviousY > y )
            {
                iAnyMoveX+=2;
            }
            break;
        }
    }
    else if ( bAnyMoveY == true )
    {
        switch(motionMode)
        {
        case 0:
            return;
            break;

        case 1: 
            if ( iPreviousX > x || iPreviousY < y )
            {
                iAnyMoveY-=2;
            }
            else if ( iPreviousX < x || iPreviousY > y )
            {
                iAnyMoveY+=2;
            }
            break;
        }
    }
    else if ( bAnyRotateZ == true )
    {
        switch(motionMode)
        {
        case 0:
            return;
            break;

        case 1: 
            if ( iPreviousX > x || iPreviousY < y )
            {
                iAnyRotateZ-=2;
            }
            else if ( iPreviousX < x || iPreviousY > y )
            {
                iAnyRotateZ+=2;
            }
            break;
        }
    }
    else if ( bAnyRotateX == true )
    {
        switch(motionMode)
        {
        case 0:
            return;
            break;

        case 1: 
            if ( iPreviousX > x || iPreviousY < y )
            {
                iAnyRotateX-=2;
            }
            else if ( iPreviousX < x || iPreviousY > y )
            {
                iAnyRotateX+=2;
            }
            break;
        }
    }
    else if ( bAnyRotateY == true )
    {
        switch(motionMode)
        {
        case 0:
            return;
            break;

        case 1: 
            if ( iPreviousX > x || iPreviousY < y )
            {
                iAnyRotateY-=2;
            }
            else if ( iPreviousX < x || iPreviousY > y )
            {
                iAnyRotateY+=2;
            }
            break;
        }
    }

    iPreviousX = x;
    iPreviousY = y;
}

void setUserView()
{
    glLoadIdentity();

    glTranslatef(-current_pos[0], current_pos[1], -current_pos[2]);
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
}



void mouseButtHandler2(int button, int state, int x, int y)
{
    motionMode = 0;
    printf("mouse click, (%d,%d)\n",x, y );
    int yy = 600 - y;
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN)
        {
            if ( iAddTriangles )
            {
                if ( x > 10 && x < (10+256) && yy > 380 && yy < 400 )
                {
                    HSVType hsv;
                    hsv.H=(float)clickH/60;
                    hsv.S=clickS;
                    hsv.V=clickV;
                    printf("(%f,%f,%f)\n",hsv.H,hsv.S,hsv.V);
                    // new triangle
                    LittleTriangle t;
                    t.x = x - 10;
                    t.hsv = hsv;
                    // add triangle to list
                    mylist.push_back(t);
                    // sort
                    mylist.sort(compare_list);
                    std::list<LittleTriangle>::iterator it;
                    std::list<LittleTriangle>::iterator it2;
                    // update color
                    it = mylist.begin();
                    it2 = mylist.begin();
                    it2++;

                    for ( int i=0; i<(mylist.size()-1); i++ )
                    {
                        fillColor(*it, *it2);
                        it++;
                        it2++;
                    }
                }
            }
            if ( iDeleteTriangles )
            {
                if ( x > 10 && x < (10+256) && yy > 380 && yy < 400 )
                {
                    std::list<LittleTriangle>::iterator it;
                    std::list<LittleTriangle>::iterator it2;
                    if ( mylist.size() > 0 ) 
                    {
                        for ( it = mylist.begin(); 
                            it != mylist.end();
                            ++it )
                        {
                            if ( ( x >= ( it->x ) ) && ( x < ( it->x + 20 ) ) )
                            {
                                rgba[it->x][0] = float(it->x)/256.0f;
                                rgba[it->x][1] = float(it->x)/256.0f;
                                rgba[it->x][2] = float(it->x)/256.0f;
                                rgba[it->x][3] = 0;
                                mylist.erase(it);
                                break;
                            }
                        }
                        // update color
                        if ( mylist.size() >= 2 ) 
                        {
                            mylist.sort(compare_list);
                            it = mylist.begin();
                            it2 = mylist.begin();
                            it2++;
                            for ( int i=0; i<(mylist.size()-1); i++ )
                            {
                                fillColor(*it, *it2);
                                it++;
                                it2++;
                            }
                        }
                        else
                        {
                            for ( int i = 0; i < 256; i++ )
                            {
                                rgba[i][0] = float(i)/256.0f;
                                rgba[i][1] = float(i)/256.0f;
                                rgba[i][2] = float(i)/256.0f;
                                rgba[i][3] = 1.0f;
                            }
                        }
                    }
                }
            }
            if ( iMoveTriangles )
            {
                bMove = false;
                if ( x > 10 && x < (10+256) && yy > 380 && yy < 400 )
                {
                    std::list<LittleTriangle>::iterator it;
                    if ( mylist.size() > 0 ) 
                    {
                        for ( it = mylist.begin(); 
                            it != mylist.end();
                            ++it )
                        {
                            if ( ( x >= ( it->x ) ) && ( x < ( it->x + 20 ) ) )
                            {
                                itMove = it;
                                bMove = true;
                                break;
                            }
                        }
                    }
                }
            }
            if ( iChangeColor )
            {
                if ( x > 10 && x < (10+256) && yy > 380 && yy < 400 )
                {
                    std::list<LittleTriangle>::iterator it;
                    std::list<LittleTriangle>::iterator it2;
                    if ( mylist.size() > 0 ) 
                    {
                        for ( it = mylist.begin(); 
                            it != mylist.end();
                            ++it )
                        {
                            if ( ( x >= ( it->x ) ) && ( x < ( it->x + 20 ) ) )
                            {
                                break;
                            }
                        }
                        HSVType hsv;
                        hsv.H=(float)clickH/60;
                        hsv.S=clickS;
                        hsv.V=clickV;
                        it->hsv.H = hsv.H;
                        it->hsv.S = hsv.S;
                        it->hsv.V = hsv.V;
                    }
                    // update color
                    if ( mylist.size() >= 2 ) 
                    {
                        mylist.sort(compare_list);
                        it = mylist.begin();
                        it2 = mylist.begin();
                        it2++;
                        for ( int i=0; i<(mylist.size()-1); i++ )
                        {
                            fillColor(*it, *it2);
                            it++;
                            it2++;
                        }
                    }
                    else
                    {
                        for ( int i = 0; i < 256; i++ )
                        {
                            rgba[i][0] = float(i)/256.0f;
                            rgba[i][1] = float(i)/256.0f;
                            rgba[i][2] = float(i)/256.0f;
                            rgba[i][3] = 1.0f;
                        }
                    }
                }
            }
   //         if ( x > 75 && x < 100 && yy > 600 && yy < 625 )
   //         {
   //             current_pos_bunny[0]+=0.1f;
   //             printf("right\n");
   //         }
   //         else if ( x > 50 && x < 75 && yy > 575 && yy < 600 )
   //         {
   //             current_pos_bunny[2]+=0.1f;
   //             printf("down\n");
   //         }
   //         else if ( x > 25 && x < 50 && yy > 600 && yy < 625 )
   //         {
   //             current_pos_bunny[0]-=0.1f;
   //             printf("left\n");
   //         }
   //         else if ( x > 50 && x < 75 && yy > 625 && yy < 650)
   //         {
   //             current_pos_bunny[2]-=0.1f;
   //             printf("up\n");
   //         }
   //         else if ( x > 200 && x < 224 && yy > 600 && yy < 624 )
   //         {
   //             light = !light;
   //         }
   //         else if ( x > 250 && x < 274 && yy > 600 && yy < 624 )
   //         {
   //             ambient = !ambient;
   //         }
   //         else if ( x > 300 && x < 324 && yy > 600 && yy < 624 )
   //         {
   //             diffuse = !diffuse;
   //         }
   //         else if ( x > 350 && x < 374 && yy > 600 && yy < 624 )
   //         {
   //             specular = !specular;
   //         }
			else if ( x > 200 && x < 400 && yy > 0 && yy < 200 )
			{
				wheelDetection ( x, yy, CENTRAL_X, CENTRAL_Y, WHEEL_RADIUS, WHEEL_RADIUS-20 );
				// wheel thick is 20
				triangleDetection ( x, yy, CENTRAL_X, CENTRAL_Y, WHEEL_RADIUS-20);
			}
        }
        break;
    case GLUT_MIDDLE_BUTTON:
        break;
    case GLUT_RIGHT_BUTTON:
        break;
    }
    glutPostRedisplay();
}


void interpolation(int x_old, int y_old, int x, int y)
{
    printf("old (%d,%d), new(%d,%d)\n", x_old, y_old, x, y );
    if(x_old == x)
    {
        path[x_old]=y_old;
    }
    else
    {
        float x_temp =0.0f, y_temp =0.0f;
        float x1, y1, x2, y2;
        x1 = x_old;
        y1 = y_old;
        x2 = x;
        y2 = y;
        if(x1>x2)
        {
            x_temp = x1;
            y_temp = y1;
            x1 = x2;
            y1 = y2;
            x2 = x_temp;
            y2 = y_temp;
        }
        int i = 0;
        for ( i = x1; i<=x2 && i<256; ++i )
        {
            if ( i >= 0 )
            {
                path[i] = y1 + (i-x1)/(x2-x1)*(y2-y1);
            }
            if(path[i]<0)
            {
                path[i]=0;
            }
            if (path[i]>255)
            {
                path[i]=255;
            }
        }
    }
}


void mouseMoveHandler2(int x, int y)
{
    int yy = 600-y;
    static int x_old = 0, y_old = 0;

    if ( x>=10 && x <(10+256) && yy >=410 && yy <460 )
    {
        path[x-10] = (yy-410)*50;
        interpolation(x_old-10, y_old-410, x-10, yy-410);
        printf("(%d,%d)\n",x,yy);
    }
    x_old = x;
    y_old = yy;

    if ( x_old < 10)
    {
        x_old=10;
    }

    if ( iMoveTriangles )
    {
        if ( x > 10 && x < (10+256) && yy > 380 && yy < 400 )
        {
            if ( bMove == true )
            {
                itMove->x = x - 10;
            }
        }
    }
    for ( int i = 0; i < 256; i++ )
    {
        rgba[i][0] = float(i)/256.0f;
        rgba[i][1] = float(i)/256.0f;
        rgba[i][2] = float(i)/256.0f;
        rgba[i][3] = 1.0f;
    }
    std::list<LittleTriangle>::iterator it;
    std::list<LittleTriangle>::iterator it2;
    if ( mylist.size() >= 2 ) 
    {
        mylist.sort(compare_list);
        it = mylist.begin();
        it2 = mylist.begin();
        it2++;
        for ( int i=0; i<(mylist.size()-1); i++ )
        {
            fillColor(*it, *it2);
            it++;
            it2++;
        }
    }
    for ( int i = 0; i < 256; ++i )
    {
        fAlpha[i] = float(path[i])/50.0f;
    }
    glutPostRedisplay();
}




