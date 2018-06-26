
#pragma once
#pragma warning( disable : 4996 )


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <io.h>
#include <process.h>
#include <list>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "./GL/glew.h"
#include "./GL/glut.h"
#include "./GL/glui.h"

#include "inputModule.h"
#include "FreeImage.h"
#include "bitmap.h"
#include "ply.h"
#include "hsv.h"
#include "marchingcubes.h"

// Global variables
GLfloat angle = 0.0;
vector<vertex> vertices;


extern "C" 
{
#define IMAGE_WIDTH  800
#define IMAGE_HEIGHT 600
#define GL_PI 3.1415962
    typedef struct
    {
        float fieldOfView;
        float aspect;
        float nearPlane;
        float farPlane;
    } perspectiveData;
}

extern GLfloat current_pos_bunny[];
extern GLfloat current_rot_bunny[];
extern GLfloat current_sca_bunny[];
extern int record;

unsigned char image[256][256][3];
unsigned char Object3DData[240][256][256];

int window;
int updateFlag;

PLYObject *ply = NULL;
GLuint texture_index;
perspectiveData pD;


float rgba[256][4];
std::list<LittleTriangle> mylist;



int window2;
extern int path[256];
extern int iKeyT;
extern int iKeyX;
extern int iKeyY;
extern int iKeyZ;
extern float fKeyA;
extern float fKeyS;
extern float fKeyD;
extern int iKeyF1;
extern int iKeyF2;
extern int iKeyF3;
static GLuint texName;

// Shading
float light_pos[4] = {0.0f, 4.0f, 0.0f, 1.0f};
float ambient_light[4] = {0.3f, 0.3f, 0.3f, 1.0f};
float light_color[4] = {0.8f, 0.8f, 0.8f, 1.0f};
float black_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
extern int light;
extern int ambient;
extern int diffuse;
extern int specular;
//
static GLuint texName2;

int iHistogram[256];
int iHistogramMax = 0;
GLfloat fAlpha[256];

bool bCross = false;

int iToolCross1;
int iToolCross2;
int iToolCross3;
int iToolCross4;
int iToolCross5;
int iToolCross6;
bool bAnyRotateZ = false;
extern int iAnyRotateZ;
int iToolAnyRotateZ;
bool bAnyRotateX = false; 
extern int iAnyRotateX;
int iToolAnyRotateX;
bool bAnyRotateY = true;
extern int iAnyRotateY;
int iToolAnyRotateY;
bool bAnyMoveZ = false;
extern int iAnyMoveZ;
int iToolAnyMoveZ;
bool bAnyMoveX = false;
extern int iAnyMoveX;
int iToolAnyMoveX;
bool bAnyMoveY = false;
extern int iAnyMoveY;
int iToolAnyMoveY;

int iAddTriangles;
int iDeleteTriangles;
int iMoveTriangles;
int iChangeColor;

int iToolMarchingCubesCPU;


unsigned char* data2;
GLuint texName3D;
int window3;


void renderScene(void) {


    if ( iToolMarchingCubesCPU != 0 )
    {
        glutSetWindow(window3);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        // Create light components
        GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        GLfloat position[] = { -1.5f, 1.0f, -4.0f, 1.0f };

        // Assign created components to GL_LIGHT0
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
        glLightfv(GL_LIGHT0, GL_POSITION, position);

        glEnable(GL_NORMALIZE);
        glEnable(GL_COLOR_MATERIAL);

        // Set material properties which will be assigned by glColor
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        float specReflection[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glRotatef(45.0f+angle, 0.0f, 1.0f, 0.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();
        glScalef(0.005f, 0.005f, 0.005f);
        glColor3f(0.0f, 0.6f, 0.6f);
        glTranslatef(0.0f, -128.0f, 32.0f);
        glRotatef(80.0f, 0.0f, 0.0f, 1.0f);

        // Draw the triangles
        vector<vertex>::iterator it;
        glBegin(GL_TRIANGLES);
        for(it = vertices.begin(); it < vertices.end(); it++) {
            glNormal3d(it->normal_x, it->normal_y, it->normal_z);
            glVertex3d(it->x, it->y, it->z);
        }
        glEnd();
        glPopMatrix();

        glutSwapBuffers();
        angle += 1.5f;

    }
}


void initData2() // texture3
{
    int width, height, depth;
    width = 256;
    height = 256;
    depth = 240;

    data2 = (unsigned char*)malloc(sizeof(unsigned char*) * depth * width * height );

    //read data from disk
    FILE *fp;
    fp = fopen("CT_head.dat","rb");
    fread(data2, sizeof(unsigned char), depth * width * height, fp);
    fclose(fp);

    //generate a texture id
    glGenTextures(1, &texName3D);
    printf("texName3D = %u\n", texName3D);
    glBindTexture(GL_TEXTURE_3D, texName3D);

    //Set Texture mapping parameters
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // generate a texture
    glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, width, height, depth, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data2);

    free(data2);
}



void initData()
{
    FILE *fp;
    fp = fopen("CT_head.dat", "rb");
    fread(Object3DData, sizeof(unsigned char), 256*256*240, fp);
    if ( fp == NULL)
    {
        printf("error opening file..\n");
        exit(1);
    }
    fclose(fp);
    memset ( &iHistogram, 0x00, 256 * sizeof(unsigned char));
    for ( int iY = 0; iY < 240; iY++ )
    {
        for ( int iX = 0; iX < 256; iX++ )
        {
            for ( int iZ = 0; iZ < 256; iZ++ )
            {
                if ( Object3DData[iY][iX][iZ] != 0 )
                {
                    iHistogram[Object3DData[iY][iX][iZ]]++;
                }
            }
        }
    }
    for ( int i = 0; i < 256; i++ )
    {
        if ( iHistogramMax < iHistogram[i] )
        {
            iHistogramMax = iHistogram[i];
        }
        rgba[i][0] = float(i)/256.0f;;
        rgba[i][1] = float(i)/256.0f;
        rgba[i][2] = float(i)/256.0f;
        rgba[i][3] = 1.0f;
    }
    printf("initData\n");
}


void drawCube ()
{
    // x axis
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    glVertex3f(-1,-1, 1);
    glVertex3f( 1,-1, 1);
    glVertex3f(-1, 1, 1);
    glVertex3f( 1, 1, 1);

    glVertex3f(-1,-1,-1);
    glVertex3f( 1,-1,-1);
    glVertex3f(-1, 1,-1);
    glVertex3f( 1, 1,-1);
    glEnd();

    // y axis
    glColor3f(0,1,0);
    glBegin(GL_LINES);
    glVertex3f(-1,-1, 1);
    glVertex3f(-1, 1, 1);
    glVertex3f( 1,-1, 1);
    glVertex3f( 1, 1, 1);

    glVertex3f(-1,-1,-1);
    glVertex3f(-1, 1,-1);
    glVertex3f( 1,-1,-1);
    glVertex3f( 1, 1,-1);
    glEnd();

    // z axis
    glColor3f(0,0,1);
    glBegin(GL_LINES);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1, 1);
    glVertex3f(-1, 1,-1);
    glVertex3f(-1, 1, 1);

    glVertex3f( 1,-1,-1);
    glVertex3f( 1,-1, 1);
    glVertex3f( 1, 1,-1);
    glVertex3f( 1, 1, 1);
    glEnd();
}


int loadtexture ( const char* fileName )
{
    BITMAPFILEHEADER1 texInfo;
    GLubyte* bits;
    GLuint id;

    bits = LoadBitmapFile(fileName,&texInfo);
    glGenTextures(1,&id);
    printf("id=%d......\n",id);

    glBindTexture(GL_TEXTURE_2D,id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_NEAREST

    glTexImage2D(GL_TEXTURE_2D, 0, 4, texInfo.biWidth, texInfo.biHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, bits );

    return id;
}


void initTexture2()
{
    texName2 = loadtexture(".\\Erath-Map.bmp");
    glBindTexture(GL_TEXTURE_2D, texName2);
}


void drawGlobe()
{
    glDisable(GL_LIGHTING);
    //glColor4f(1,1,1,1);
    glColor4f(0,0,0,1);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texName2);

    float d = 128.0f; //32
    float radius = 1.0f;
    float DTOR = 3.141592653587f / 180.0f;

    int x, y;
    float theta, phi;
    
    //GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
    //GL_POINT, GL_LINE, GL_FILL
    // void glPolygonMode (GLenum face, GLenum mode );

    // glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    for ( y = -2048.0f; y <= 2048.0f; y+=d )
    {
        glBegin(GL_TRIANGLE_STRIP);
        //glBegin(GL_LINE_STRIP);
        for ( x = - 2048.0f; x <= 2048.0f; x+=d )
        {
            theta = (x+2048.0f) / (2*2048.0f) * 360.0f - 90.0f;
            phi = (y - 2048.0f) / ( 2* 2048.0f) * 180.0f;
            glTexCoord2f(1.0-(x+2048.0f)/4096.0f, (y+2048.0f)/4096.0f);
            
            glVertex3f(
                radius * sin(phi*DTOR) * cos(DTOR*theta),
                radius * cos(phi*DTOR),
                radius * sin(phi*DTOR) * sin(DTOR*theta)
                );
            //
            phi = (y+d-2048.0f)/(2*2048.0f) * 180.0f;
            glTexCoord2f(1.0-(x+2048.0f)/4096.0f,(y+d+2048.0f)/4096.0f);
            glVertex3f(
                radius * sin(phi*DTOR) * cos(DTOR*theta),
                radius * cos(phi*DTOR),
                radius * sin(phi*DTOR) * sin(DTOR*theta)
                );

        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
}


unsigned char* readPPM(char* filename, int* width, int* height)
{
    FILE* fp;
    int i, w, h, d;
    unsigned char* image;
    char head[70];
    fp = fopen(filename, "rb");
    if(!fp)
    {
        perror(filename);
        return NULL;
    }
    fgets(head,70,fp);
    if(strncmp(head,"P6",2))
    {
        fprintf(stderr,"%s: Not a raw PPM file\n", filename);
        return NULL;
    }
    i = 0;
    while(i<3)
    {
        fgets(head,70,fp);
        if(head[0]=='#')
        {
            continue;
        }
        if(i==0)
        {
            i+=sscanf(head,"%d,%d,%d", &w, &h, &d);
        }
        else if (i==1)
        {
            i+=sscanf(head, "%d,%d", &h, &d);
        }
        else if ( i == 2 )
        {
            i+=sscanf(head, "%d", &d );
        }
    }
    image = (unsigned char*)malloc(sizeof(unsigned char)*w*h*3);
    fread(image,sizeof(unsigned char), w*h*3, fp);
    fclose(fp);
    *width = w;
    *height = h;
    return image;
}

/*
**
*/
void initLighting()
{
	// setup lights
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black_color);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}


/*
**
*/
void gl_ortho_begin(unsigned size_x, unsigned size_y)
{
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.375f, 0.375f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, size_x, 0, size_y);
    glMatrixMode(GL_MODELVIEW);
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}


/*
**
*/
void gl_ortho_end(void)
{
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


/*
**  cleanup
*/
void cleanup(int sig)
{
    delete(ply);
    exit(0);
}


/*
**  drawPlane
*/
void drawPlane()
{
    // floor
    //glColor3f(0.5f, 0.5f, 0.9f);
    //glBegin(GL_POLYGON);
    //glVertex3f(-5,-1, 5);
    //glVertex3f( 5,-1, 5);
    //glVertex3f( 5,-1,-5);
    //glVertex3f(-5,-1,-5);
    //glEnd();

    // wire
    float i;
    //glColor3f(0.5,0,0);
    //glBegin(GL_LINES);
    //for (i=-5.0f; i<=5.0f; i+=1.0f)
    //{
    //    glVertex3f( i,-0.98f,  5);
    //    glVertex3f( i,-0.98f, -5);
    //    glVertex3f( 5,-0.98f,  i);
    //    glVertex3f(-5,-0.98f,  i);
    //}
    //glEnd();

    // axis Red=x, Green=y, Blue=z
    glBegin(GL_LINES);
    for (i=-5.0f; i<=5.0f; i+=1.0f)
    {
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(5,0,0);
        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,5,0);
        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,5);
    }
    glEnd();


    //
    //glDisable(GL_CULL_FACE);
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D,texName2);
    //static float s = 0.0f;
    //s+=0.0001f;
    //glBegin(GL_QUADS);
    //glTexCoord2f(0.0f+s, 0.0f+s); glVertex3f(-5.0f,-0.99f, 5.0f);
    //glTexCoord2f(0.0f+s, 1.0f+s); glVertex3f( 5.0f,-0.99f, 5.0f);
    //glTexCoord2f(1.0f+s, 1.0f+s); glVertex3f( 5.0f,-0.99f,-5.0f);
    //glTexCoord2f(1.0f+s, 0.0f+s); glVertex3f(-5.0f,-0.99f,-5.0f);
    //glEnd();
    //glDisable(GL_TEXTURE_2D);
    //glEnable(GL_CULL_FACE);

}


/*
**  string render function
*/
void string_render(const char *string, GLint x, GLint y)
{
    unsigned i;
    glRasterPos2i(x + 1, y - 1);
    for (i = 0; string[i]; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
    }
}


/*
**  screenshot
*/
void screenshot(int x, int y, int w, int h)
{
    FIBITMAP *image = FreeImage_Allocate(w, h, 24);

    unsigned char *data = new unsigned char[w*h*3];
    unsigned char *temp = new unsigned char[w*3];

    if(!data || !temp)
    {
        fprintf(stderr, "no more memory!\n");
    }

    glReadPixels(x,y,w,h, GL_RGB, GL_UNSIGNED_BYTE, data);

    int i;
    unsigned char tempData;
    for(i=0; i<w*h*3; i+=3){
        tempData = data[i+2];
        data[i+2] = data[i];
        data[i]   = tempData;
    }

    // Save image
    image=FreeImage_ConvertFromRawBits(data, w,h,w*3, 24, 0,0,0);
    if(!image)
    {
        fprintf(stderr, "Failed to convert bits!\n");
    }


    static int count = 0;
    char filename[50];

    sprintf(filename,"%05d.png", count);

    if(FreeImage_Save(FIF_PNG, image, filename))
        fprintf(stderr, "Wrote %s!\n", filename);
    else
        fprintf(stderr, "Failed!\n");

    count++;

    FreeImage_Unload(image);

    delete [] temp;
    delete [] data;
}


void drawPlaneTexture()
{
    //glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texName);
    static float s = 0.0f;
    s+=0.005f;
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f+s, 0.0f+s); glVertex3f(-5.0f, 10.0f, -5.0f);
    glTexCoord2f(0.0f+s, 1.0f+s); glVertex3f( 5.0f, 10.0f, -5.0f);
    glTexCoord2f(1.0f+s, 1.0f+s); glVertex3f( 5.0f,  0.0f, -5.0f);
    glTexCoord2f(1.0f+s, 0.0f+s); glVertex3f(-5.0f,  0.0f, -5.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //glEnable(GL_CULL_FACE);
}

/*
**
*/
void display(void)
{  
    if ( iToolMarchingCubesCPU == 0 )
    {
        
        glutSetWindow(window);
        glutPostRedisplay();
        glutSetWindow(window2);
        glutPostRedisplay();

        //
        glutSetWindow(window);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glEnable( GL_LIGHTING );

        setUserView();



        if ( iToolCross1 )
        {
            // 平面
            glPushMatrix();
            glTranslatef(-1,-1,-1);
            for(int i = 0; i<256-1;i++)
            {
                glBegin(GL_QUAD_STRIP);
                for(int j = 0;j<256;j++)
                {
                    glColor4f(rgba[Object3DData[120][i][j]][0], rgba[Object3DData[120][i][j]][1], rgba[Object3DData[120][i][j]][2], fAlpha[Object3DData[120][i][j]]);
                    glVertex3f(i/128.0, 120/128.0, j/128.0);

                    glColor4f(rgba[Object3DData[120][i+1][j]][0], rgba[Object3DData[120][i+1][j]][1], rgba[Object3DData[120][i+1][j]][2], fAlpha[Object3DData[120][i+1][j]]);
                    glVertex3f((i+1)/128.0, 120/128.0, j/128.0);
                }
                glEnd();
            }
            glPopMatrix();
        }
        if ( iToolCross2 )
        {
            // 垂直
            glPushMatrix();
            glTranslatef(-1,-1,-1);
            for(int i = 0; i<240-1;i++)
            {
                glBegin(GL_QUAD_STRIP);
                for(int j = 0;j<256;j++)
                {
                    glColor4f(rgba[Object3DData[i][128][j]][0], rgba[Object3DData[i][128][j]][1], rgba[Object3DData[i][128][j]][2], fAlpha[Object3DData[i][128][j]]);
                    glVertex3f(128/128.0, i/128.0, j/128.0);

                    glColor4f(rgba[Object3DData[i+1][128][j]][0], rgba[Object3DData[i+1][128][j]][1], rgba[Object3DData[i+1][128][j]][2], fAlpha[Object3DData[i+1][128][j]]);
                    glVertex3f(128/128.0, (i+1)/128.0, j/128.0);
                }
                glEnd();
            }
            glPopMatrix();
        }
        if ( iToolCross3 )
        {
            // 垂直
            glPushMatrix();
            glTranslatef(-1,-1,-1);
            for(int i = 0; i<240-1;i++)
            {
                glBegin(GL_QUAD_STRIP);
                for(int j = 0;j<256;j++)
                {
                    glColor4f(rgba[Object3DData[i][j][128]][0], rgba[Object3DData[i][j][128]][1], rgba[Object3DData[i][j][128]][2], fAlpha[Object3DData[i][j][128]]);
                    glVertex3f(j/128.0, i/128.0, 128/128.0);

                    glColor4f(rgba[Object3DData[i+1][j][128]][0], rgba[Object3DData[i+1][j][128]][1], rgba[Object3DData[i+1][j][128]][2], fAlpha[Object3DData[i+1][j][128]]);
                    glVertex3f(j/128.0, (i+1)/128.0, 128/128.0);
                }
                glEnd();
            }
            glPopMatrix();
        }
        if ( iToolCross4 )
        {
            // 斜角
            glPushMatrix();
            glTranslatef(-1,-1,-1);
            for(int i = 0; i<240-1;i++)
            {
                glBegin(GL_QUAD_STRIP);
                for(int j = 0;j<256;j++)
                {
                    glColor4f(rgba[Object3DData[i][j][j]][0], rgba[Object3DData[i][j][j]][1], rgba[Object3DData[i][j][j]][2],fAlpha[Object3DData[i][j][j]]);
                    glVertex3f(j/128.0, i/128.0, j/128.0);

                    glColor4f(rgba[Object3DData[i+1][j][j]][0], rgba[Object3DData[i+1][j][j]][1], rgba[Object3DData[i+1][j][j]][2], fAlpha[Object3DData[i+1][j][j]]);
                    glVertex3f(j/128.0, (i+1)/128.0, j/128.0);
                }
                glEnd();
            }
            glPopMatrix();
        }
        if ( iToolCross5 )
        {
            // 動畫
            glPushMatrix();
            glTranslatef(-1,-1,-1);
            static int index2= 0;
            index2++;
            if(index2>=240)
            {
                index2=0;
            }
            for(int i = 0; i<256-1;i++)
            {
                glBegin(GL_QUAD_STRIP);
                for(int j = 0;j<256;j++)
                {
                    glColor4f(rgba[Object3DData[index2][i][j]][0], rgba[Object3DData[index2][i][j]][1], rgba[Object3DData[index2][i][j]][2], fAlpha[Object3DData[index2][i][j]]);
                    glVertex3f(i/128.0, index2/128.0, j/128.0);

                    glColor4f(rgba[Object3DData[index2][i+1][j]][0], rgba[Object3DData[index2][i+1][j]][1], rgba[Object3DData[index2][i+1][j]][2], fAlpha[Object3DData[index2][i+1][j]]);
                    glVertex3f((i+1)/128.0, index2/128.0, j/128.0);
                }
                glEnd();
            }
            glPopMatrix();
        }
        if ( iToolCross6 )
        {
            // 動畫 texture3
            glDisable(GL_LIGHTING);
            glEnable(GL_TEXTURE_3D);
            glBindTexture(GL_TEXTURE_3D, texName3D);
            glPushMatrix();
            glTranslatef(2,-1,-1);
            static int index3 = 0;
            index3++;
            if ( index3 >= 240 )
            {
                index3 = 0;
            }
            glBegin(GL_QUADS);
            {
                glTexCoord3f(0,0, index3/240.0);
                glVertex3f(-1.0, index3/120.0, -1.0);
                glTexCoord3f(1,0, index3/240.0);
                glVertex3f(1.0, index3/120.0, -1.0);
                glTexCoord3f(1,1,index3/240.0);
                glVertex3f(1.0, index3/120.0, 1.0);
                glTexCoord3f(0, 1, index3/240.0);
                glVertex3f(-1.0, index3/120.0, 1.0);
            }
            glEnd();
            glPopMatrix();
            glDisable(GL_TEXTURE_3D);
        }

        // 任意角
        if (1)
        {
            bCross = false;
            bAnyMoveZ = false;
            bAnyMoveX = false;
            bAnyMoveY = false;
            bAnyRotateZ = false;
            bAnyRotateX = false;
            bAnyRotateY = false;
            if ( iToolAnyMoveX )
            {
                bCross = false;
                bAnyMoveZ = false;
                bAnyMoveX = true;
                bAnyMoveY = false;
                bAnyRotateZ = false;
                bAnyRotateX = false;
                bAnyRotateY = false;
            }
            if ( iToolAnyMoveY )
            {
                bCross = false;
                bAnyMoveZ = false;
                bAnyMoveX = false;
                bAnyMoveY = true;
                bAnyRotateZ = false;
                bAnyRotateX = false;
                bAnyRotateY = false;
            }
            if ( iToolAnyMoveZ )
            {
                bCross = false;
                bAnyMoveZ = true;
                bAnyMoveX = false;
                bAnyMoveY = false;
                bAnyRotateZ = false;
                bAnyRotateX = false;
                bAnyRotateY = false;
            }
            if ( iToolAnyRotateX )
            {
                bCross = false;
                bAnyMoveZ = false;
                bAnyMoveX = false;
                bAnyMoveY = false;
                bAnyRotateZ = false;
                bAnyRotateX = true;
                bAnyRotateY = false;
            }
            if ( iToolAnyRotateY )
            {
                bCross = false;
                bAnyMoveZ = false;
                bAnyMoveX = false;
                bAnyMoveY = false;
                bAnyRotateZ = false;
                bAnyRotateX = false;
                bAnyRotateY = true;
            }
            if ( iToolAnyRotateZ )
            {
                bCross = false;
                bAnyMoveZ = false;
                bAnyMoveX = false;
                bAnyMoveY = false;
                bAnyRotateZ = true;
                bAnyRotateX = false;
                bAnyRotateY = false;
            }

            if ( 1 )
            {
                int iX = 0;
                int iY = 0;
                int iZ = 0;
                int iC = 0;
                iZ=iAnyMoveZ;

                glPushMatrix();
                glTranslatef(-1,-1,-1);
                for ( iX = 0 + iAnyMoveX; iX < ( 240 - 1 ) + iAnyMoveX; iX++ )
                {
                    glBegin(GL_QUAD_STRIP);
                    for ( iY = 0  + iAnyMoveY; iY < ( 256 ) + iAnyMoveY; iY++ )
                    {
                        if ( bAnyRotateX == true )
                        {
                            iZ = ( ( iX ) * ( float( iAnyRotateX ) / 360.0f ) * 3.1415962535f ) + iAnyMoveX;
                            if ( ( iX >= 0 && iX < 240 - 1 ) &&
                                ( iY >= 0 && iY < 256 ) &&
                                ( iZ >= 0 && iZ < 256 ) )
                            {
                                glColor4f(rgba[Object3DData[iX][iY][iZ]][0], rgba[Object3DData[iX][iY][iZ]][1], rgba[Object3DData[iX][iY][iZ]][2], fAlpha[Object3DData[iX][iY][iZ]]);
                                glVertex3f(iY/128.0, iX/128.0, iZ/128.0);

                                glColor4f(rgba[Object3DData[iX+1][iY+0][iZ+0]][0], rgba[Object3DData[iX+1][iY+0][iZ+0]][1], rgba[Object3DData[iX+1][iY+0][iZ+0]][2], fAlpha[Object3DData[iX][iY][iZ]]);
                                glVertex3f(((iY+0)/128.0), (iX+1)/128.0, (iZ+0)/128.0);
                            }
                            else
                            {
                                glColor3ub(0, 0, 0);
                                glVertex3f(iY/128.0, iX/128.0, iZ/128.0);

                                glColor3ub(0, 0, 0);
                                glVertex3f((iY+0)/128.0, (iX+1)/128.0, (iZ+0)/128.0);
                            }
                        }
                        else if ( bAnyRotateY == true )
                        {
                            iZ = ( ( iY ) * ( float( iAnyRotateY ) / 360.0f ) * 3.1415962535f ) + iAnyMoveY;
                            if ( ( iX >= 0 && iX < 240 - 1 ) &&
                                ( iY >= 0 && iY < 256 ) &&
                                ( iZ >= 0 && iZ < 256 ) )
                            {
                                glColor4f ( rgba[Object3DData[iX][iZ][iY]][0], rgba[Object3DData[iX][iZ][iY]][1], rgba[Object3DData[iX][iZ][iY]][2], fAlpha[Object3DData[iX][iZ][iY]] );
                                glVertex3f ( iZ/128.0, iX/128.0, iY/128.0 );

                                glColor4f ( rgba[Object3DData[iX+1][iZ+0][iY+0]][0], rgba[Object3DData[iX+1][iZ+0][iY+0]][1], rgba[Object3DData[iX+1][iZ+0][iY+0]][2], fAlpha[Object3DData[iX+1][iZ+0][iY+0]] );
                                glVertex3f ( (iZ+0)/128.0, (iX+1)/128.0, (iY+0)/128.0 );
                            }
                            else
                            {
                                glColor3ub(0, 0, 0);
                                glVertex3f(iZ/128.0, iX/128.0, iY/128.0);

                                glColor3ub(0, 0, 0);
                                glVertex3f((iZ+0)/128.0, (iX+1)/128.0, (iY+0)/128.0);
                            }
                        }
                        else if ( bAnyRotateZ == true )
                        {
                            iZ = ( ( iY ) * ( float( iAnyRotateZ ) / 360.0f ) * 3.1415962535f ) + iAnyMoveZ;
                            if ( ( iX >= 0 && iX < 240 - 1 ) &&
                                ( iY >= 0 && iY < 256 ) &&
                                ( iZ >= 0 && iZ < 256 ) )
                            {
                                glColor4f(rgba[Object3DData[iX][iY][iZ]][0], rgba[Object3DData[iX][iY][iZ]][1], rgba[Object3DData[iX][iY][iZ]][2], fAlpha[Object3DData[iX][iY][iZ]] );
                                glVertex3f(iY/128.0, iX/128.0, iZ/128.0);

                                glColor4f(rgba[Object3DData[iX+1][iY+0][iZ+0]][0], rgba[Object3DData[iX+1][iY+0][iZ+0]][1], rgba[Object3DData[iX+1][iY+0][iZ+0]][2], fAlpha[Object3DData[iX+1][iY+0][iZ+0]]);
                                glVertex3f(((iY+0)/128.0), (iX+1)/128.0, (iZ+0)/128.0);
                            }
                            else
                            {
                                glColor3ub(0, 0, 0);
                                glVertex3f(iY/128.0, iX/128.0, iZ/128.0);

                                glColor3ub(0, 0, 0);
                                glVertex3f((iY+0)/128.0, (iX+1)/128.0, (iZ+0)/128.0);
                            }
                        }
                        else 
                        {
                            if ( ( iX >= 0 && iX < 240 - 1 ) &&
                                ( iY >= 0 && iY < 256 ) &&
                                ( iZ >= 0 && iZ < 256 ) )
                            {
                                glColor4f(rgba[Object3DData[iX][iY][iZ]][0], rgba[Object3DData[iX][iY][iZ]][1], rgba[Object3DData[iX][iY][iZ]][2], fAlpha[Object3DData[iX][iY][iZ]]);
                                glVertex3f(iY/128.0, iX/128.0, iZ/128.0);

                                glColor4f(rgba[Object3DData[iX+1][iY+0][iZ+0]][0], rgba[Object3DData[iX+1][iY+0][iZ+0]][1], rgba[Object3DData[iX+1][iY+0][iZ+0]][2], fAlpha[Object3DData[iX][iY][iZ]]);
                                glVertex3f(((iY+0)/128.0), (iX+1)/128.0, (iZ+0)/128.0);
                            }
                            else
                            {
                                glColor3ub(0, 0, 0);
                                glVertex3f(iY/128.0, iX/128.0, iZ/128.0);

                                glColor3ub(0, 0, 0);
                                glVertex3f((iY+0)/128.0, (iX+1)/128.0, (iZ+0)/128.0);
                            }
                        }
                    }
                    glEnd();
                }
                glPopMatrix();
            } // 轉動 of End
        }


        
        if ( light )
        {
            glEnable(GL_LIGHTING);
        }
        else
        {
            glDisable(GL_LIGHTING);
        }
        if (ambient)
        {
            glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
        }
        else
        {
            glLightfv(GL_LIGHT0, GL_AMBIENT, black_color);
        }
        if (diffuse)
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
        }
        else
        {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, black_color);
        }
        if (specular)
        {
            glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
        }
        else
        {
            glLightfv(GL_LIGHT0, GL_SPECULAR, black_color);
        }

        
        GLfloat mat_specular[] = {1.0f,1.0f,1.0f,1.0f};
        GLfloat low_shininess[] = {5.0f};
        GLfloat mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f };
        GLfloat no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
        GLfloat mat_ambient_color[] = { 0.8f, 0.8f, 0.2f, 1.0f };
        GLfloat mat_diffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
        //GLfloat mat_specular[] = { 1.0f,1.0f,1.0f,1.0f };
        GLfloat no_shininess[] = { 0.0f };
        //GLfloat low_shininess[] = { 5.0f };
        GLfloat high_shininess[] = { 100.0f };
        //GLfloat mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };

        glMaterialfv ( GL_FRONT, GL_AMBIENT, no_mat );
        glMaterialfv ( GL_FRONT, GL_DIFFUSE, mat_diffuse );
        glMaterialfv ( GL_FRONT, GL_SPECULAR, no_mat );
        glMaterialfv ( GL_FRONT, GL_SHININESS, no_shininess );
        glMaterialfv ( GL_FRONT, GL_EMISSION, no_mat );


        
        if ( iKeyT )
        {
            //drawPlaneTexture();
        }

        
        // 畫軌跡球
        int iX = 0;
        //for ( i = 0; i < 256; ++i )
        //{
        //    glPushMatrix();
        //    glTranslatef(-5.0f+(i/256.0f)*10, -0.95f, 5.0f - (path[i]/256.0) * 10 );
        //    glutSolidSphere(0.1f,5, 5);
        //    glPopMatrix();
        //}



        drawPlane ();

        
        // draw Cube
        drawCube ();
        // draw Erath
        //drawGlobe();

        //// primitives
        //glColor3f(1,0,0);
        //glBegin(GL_TRIANGLES);
        //// fill_me...
        //glEnd();

        //// primitives
        //glColor3f(1,1,0);
        //glBegin(GL_POLYGON);
        //// fill_me...
        //glEnd();

        //glColor3f(0,1,0);
        //glBegin(GL_LINES);
        //// fill_me...
        //glEnd();


        
        static int index = 0;
        if ( index >= 256 )
        {
            index = 0;
        }
        //glPushMatrix();
        //// fill_me...
        //glTranslatef (
        //    current_pos_bunny[0],
        //    current_pos_bunny[1],
        //    current_pos_bunny[2] );
        //glTranslatef
        //    (
        //    -5.0f+(index/256.0f)*10,
        //    0, 
        //    5.0f - (path[index]/256.0)*10
        //    );
        //
        //glRotatef((GLfloat)iKeyX,1.0f,0.0f,0.0f);
        //glRotatef((GLfloat)iKeyY,0.0f,1.0f,0.0f);
        //glRotatef((GLfloat)iKeyZ,0.0f,0.0f,1.0f);
        //glScalef(fKeyA+1.0f,1.0f,1.0f);
        //glScalef(1.0f,fKeyS+1.0f,1.0f);
        //glScalef(1.0f,1.0f,fKeyD+1.0f);
        //ply->draw();  
        //glPopMatrix();
        //++index;

        // string render
        gl_ortho_begin(IMAGE_WIDTH, IMAGE_HEIGHT);
        glColor3f(0.0f, 0.0f, 0.0f);
        string_render("How To Make Slicing And Marching Cubes CPU Version Using OpenGL?", 20, 580); 
        string_render("v: start/end screenshot", 20, 550);
        gl_ortho_end();


        // QR-Code kamon
        
        glDisable(GL_LIGHTING);
        glBegin(GL_POINTS);
        for (int iX=0; iX<256; iX++)
        {
            for (int j=0; j<256; j++)
            {
                glColor3ubv(image[iX][j]);
                glVertex3f(2.5f + j/200.0f, -0.99, 2.5f + iX/200.0f);
            }
        }   
        glEnd();
        glEnable(GL_LIGHTING);


        // screenshot
        if (record)
        {
            screenshot(0,0,IMAGE_WIDTH,IMAGE_HEIGHT);
        }

        glutSwapBuffers();

        glDisable(GL_LIGHTING);

    }

}


/*
**
*/
void reshape(int width, int height)
{
    int screenWidth  = width;
    int screenHeight = height;

    if (screenWidth <= 0 || screenHeight <=0)
        return;

    glViewport(0, 0, screenWidth, screenHeight);
    glutReshapeWindow(screenWidth, screenHeight);

    pD.aspect = (float)screenWidth / screenHeight;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pD.fieldOfView, pD.aspect, pD.nearPlane, pD.farPlane);

    // set basic matrix mode
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


/*
**
*/
void initDisplay()
{
    // Perspective projection parameters
    pD.fieldOfView = 45.0;
    pD.aspect      = (float)IMAGE_WIDTH/IMAGE_HEIGHT;
    pD.nearPlane   = 0.1;
    pD.farPlane    = 50.0;

    // setup context
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pD.fieldOfView, pD.aspect, pD.nearPlane, pD.farPlane);

    // set basic matrix mode
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClearIndex(0);
    glClearDepth(1);

    //-------------------------------------------------------
    // Kamon (image.raw) must be 256x256 (color image) pixels
    FILE *p;
    p = fopen("qr.raw", "rb");  
    fread(image, sizeof(unsigned char), 256*256*3, p);
    fclose(p);
    //-------------------------------------------------------
}


/*
**
*/
void idle_callback(void){
    
    glutSetWindow(window);
    glutPostRedisplay();
    glutSetWindow(window2);
    glutPostRedisplay();
}

void init_ply(void)
{
    FILE *in;
    char *filename = "bunny.ply"; // "dragon.ply", "happy.ply"

    if (!(in = fopen(filename, "r"))) {
        fprintf(stderr, "Cannot open input file %s.\n", filename);
        exit(1);
    }
    ply = new PLYObject(in);
    ply->resize();
}


void initTexture()
{
    //read a ppm image
    unsigned char* image;
    int width, height;
    image = readPPM("face.ppm", &width, &height );
    //generate a texture id
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D,texName);
    //set texture mapping parameters
    //float env_color[]={1.0,1.0,1.0,1.0};
    //glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, env_color);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE); //GL_DECAL, GL_MODULATE, GL_BLEND
    //generate a texture
    glTexImage2D(GL_TEXTURE_2D,0,3,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image);
}



void display2(void)
{
    glutSetWindow(window2);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gl_ortho_begin(400,600);

	//色輪
	drawColorWheel();

    //lines of box
    glColor3f(1,1,0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(10,490);
    glVertex2i(10+256,490);
    glVertex2i(10+256,490+100);
    glVertex2i(10,490+100);
    glEnd();
    //line color of Histogram
    for ( int i = 0; i < 256; ++i )   
    {
        glBegin(GL_LINE_STRIP);
        {
            glColor3f ( rgba[i][0], rgba[i][1], rgba[i][2] );
            glVertex2i ( 10 + i, 491 );
            glVertex2i ( 10 + i, 491 + (((float(iHistogram[i])/float(iHistogramMax))*100.0f)*1) );
        }
        glEnd();
    }

    //line color of Alpha
    for ( int i = 0; i < 256; ++i )   
    {
        glBegin(GL_LINE_STRIP);
        {
            glColor4f ( 1.0, 0.0, 0.0, fAlpha[i] );
            glVertex2i ( 10 + i, 480 );
            glVertex2i ( 10 + i, 480 - 10 );
        }
        glEnd();
    }

    //line color of Box
    for ( int i = 0; i < 256; ++i )   
    {
        glBegin(GL_LINE_STRIP);
        {
            glColor3f ( rgba[i][0], rgba[i][1], rgba[i][2] );
            glVertex2i ( 10 + i, 460 );
            glVertex2i ( 10 + i, 460 - 50 );
        }
        glEnd();
    }
    //lines of box
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_QUADS);
    glVertex2i(10,400);
    glVertex2i(10+256,400);
    glVertex2i(10+256,380);
    glVertex2i(10,380);
    glEnd();

    drawLittleTriangle();

    //line
    glColor3f(1,1,0);
    glBegin(GL_LINE_STRIP);
    int i = 0;
    for( i=0; i<256; ++i )
    {
        glVertex2i(10+i,410+float(path[i]));
    }
    glEnd();

    gl_ortho_end();
    glutSwapBuffers();

    //arrow
    //glColor3f(1,1,1);
    //glBegin(GL_TRIANGLES);
    //up
    //glVertex2i(62,650);
    //glVertex2i(75,625);
    //glVertex2i(50,625);
    //right
    //glVertex2i(75,625);
    //glVertex2i(100,612);
    //glVertex2i(75,600);
    //down
    //glVertex2i(50,600);
    //glVertex2i(75,600);
    //glVertex2i(62,575);
    //left
    //glVertex2i(25,612);
    //glVertex2i(50,625);
    //glVertex2i(50,600);
    //glEnd();
    //words
    //glColor3f(1.0f, 0.0f, 0.0f);
    //string_render("UP",55,630); // fill_me...
    //string_render("RIGHT",75,610);
    //string_render("DOWN",40,585);
    //string_render("LEFT",20,610);



    //button
    //GLfloat x,y,z,angle;
    //z = 0;
    //button 1
    //glPushMatrix();
    //glTranslatef(212,612,0);
    //glColor3f(0.0f,0.5f,0.5f);
    //glBegin(GL_POINTS);
    //for(angle=0.0f;angle<2*GL_PI;angle+=0.01f)
    //{
    //    x=12.0f*sin(angle);
    //    y=12.0f*cos(angle);
    //    glVertex2f(x,y);
    //}
    //glEnd();
    //glPopMatrix();
    //button 2
    //glPushMatrix();
    //glTranslatef(262,612,0);
    //glColor3f(0.0f,0.5f,0.5f);
    //glBegin(GL_POINTS);
    //for(angle=0.0f;angle<2*GL_PI;angle+=0.01f)
    //{
    //    x=12.0f*sin(angle);
    //    y=12.0f*cos(angle);
    //    glVertex2f(x,y);
    //}
    //glEnd();
    //glPopMatrix();
    //button 3
    //glPushMatrix();
    //glTranslatef(312,612,0);
    //glColor3f(0.0f,0.5f,0.5f);
    //glBegin(GL_POINTS);
    //for(angle=0.0f;angle<2*GL_PI;angle+=0.01f)
    //{
    //    x=12.0f*sin(angle);
    //    y=12.0f*cos(angle);
    //    glVertex2f(x,y);
    //}
    //glEnd();
    //glPopMatrix();
    //button 4
    //glPushMatrix();
    //glTranslatef(362,612,0);
    //glColor3f(0.0f,0.5f,0.5f);
    //glBegin(GL_POINTS);
    //for(angle=0.0f;angle<2*GL_PI;angle+=0.01f)
    //{
    //    x=12.0f*sin(angle);
    //    y=12.0f*cos(angle);
    //    glVertex2f(x,y);
    //}
    //glEnd();
    //glPopMatrix();
    //arrow
    //glColor3f(0,1,1);
    //glBegin(GL_TRIANGLES);
    //glVertex2i(125,625);
    //glVertex2i(150,612);
    //glVertex2i(125,600);
    //glEnd();

}


void myGlutIdle( void )
{
  /* According to the GLUT specification, the current window is 
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != window ) 
    glutSetWindow(window);  

  glutPostRedisplay();
}

void DefaultTFE(int control)
{
    if ( control == 11 )
    {
        int iCount = mylist.size();
        FILE *fp = fopen(".\\default.tfe","wb");
        fwrite ( &iHistogram[0], 256*sizeof(int),sizeof(int), fp );
        fwrite ( &fAlpha[0], 256*sizeof(GLfloat),sizeof(GLfloat), fp );
        fwrite ( &path[0], 256*sizeof(int),sizeof(int), fp );
        fwrite ( &iCount, 1*sizeof(int),sizeof(int), fp );
        if ( mylist.empty() == false )
        {
            std::list<LittleTriangle>::iterator it;
            LittleTriangle lt;
            for ( it = mylist.begin();
                it != mylist.end();
                ++it )
            {
                lt.hsv.H = it->hsv.H;
                lt.hsv.S = it->hsv.S;
                lt.hsv.V = it->hsv.V;
                lt.x = it->x;
                fwrite ( &lt, sizeof(LittleTriangle),sizeof(LittleTriangle), fp );
            }
        }
        fclose(fp);
    }
    else if ( control == 12 )
    {
        int iCount = 0;
        FILE *fp = fopen(".\\default.tfe","rb");
        fread ( &iHistogram[0], 256*sizeof(int),sizeof(int), fp );
        fread ( &fAlpha[0], 256*sizeof(GLfloat),sizeof(GLfloat), fp );
        fread ( &path[0], 256*sizeof(int),sizeof(int), fp );
        fread ( &iCount, 1*sizeof(int),sizeof(int), fp );
        mylist.clear();
        LittleTriangle it;
        for ( int iI = 0;
            iI < iCount;
            ++iI )
        {
            fread ( &it, sizeof(LittleTriangle),sizeof(LittleTriangle), fp );
            mylist.push_back(it);
        }
        fclose(fp);
        glutMainLoop();
    }
}

/*
**
*/
int main(int argc, char **argv)
{
    
    iKeyT = 1;
    iKeyX = 0;
    iKeyY = 0;
    iKeyZ = 0;
    fKeyA = 0.0f;
    fKeyS = 0.0f;
    fKeyD = 0.0f;
    iKeyF1 = 0;
    iKeyF2 = 0;
    iKeyF3 = 0;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(IMAGE_WIDTH,IMAGE_HEIGHT);

    
    window = glutCreateWindow("How To Make Slicing And Marching Cubes CPU Version Using OpenGL?");  

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(readKeyboard);
    glutMouseFunc(mouseButtHandler);
    glutMotionFunc(mouseMoveHandler);
    glutPassiveMotionFunc(mouseMoveHandler);
    glutSpecialFunc(special_callback);
    //glutIdleFunc(idle_callback);
    
    glutSpecialUpFunc(F123Keyboard);
    glutIdleFunc(display);


    
    glewInit();
    initData2();
    initData();
    //init_ply();
    initDisplay();
    //initTexture();
	initLighting();
    initTexture2();
    for ( int i = 0; i< 256; ++i )
    {
        fAlpha[i] = 1.0f;
        path[i]=50;
    }

    
    // window 2
    glutInitWindowPosition(830,100);
    glutInitWindowSize(400,600);
    window2 = glutCreateWindow ("Transfer Function Editor");
    printf("window2=%d\n",window2);

    glutDisplayFunc(display2);
    glutMouseFunc(mouseButtHandler2);
    glutPassiveMotionFunc(mouseMoveHandler2);



    
    glutInitWindowPosition(830,0);
    GLUI *glui = GLUI_Master.create_glui( "On/Off Control" );
    new GLUI_StaticText( glui, "How To How To Make Slicing And Marching Cubes CPU Version Using OpenGL?" );
    new GLUI_StaticText( glui, "Part 1: Cross sections along x, y, and z-axis." );
    new GLUI_StaticText( glui, "Part 2. Multiple cutting planes could be active or inactive." );
    new GLUI_Checkbox( glui, "Part 1 & 2: Cross Sections 1 (Y Plane, Red = X)", &iToolCross1 );
    new GLUI_Checkbox( glui, "Part 1 & 2: Cross Sections 2 (X Plane, Green = Y)", &iToolCross2 );
    new GLUI_Checkbox( glui, "Part 1 & 2: Cross Sections 3 (Z Plane, Blue = Z)", &iToolCross3 );
    new GLUI_Checkbox( glui, "Part 1 & 2: Cross Sections 4", &iToolCross4 );
    new GLUI_Checkbox( glui, "Part 1 & 2: Cross Sections 5 using Index method", &iToolCross5 );
    new GLUI_Checkbox( glui, "Part 1 & 2: Cross Sections 6 using Texture method", &iToolCross6 );
    new GLUI_StaticText( glui, "Part 3: Interactive slicing from an arbitrary angle." );
    new GLUI_Checkbox( glui, "Part 3: Move X", &iToolAnyMoveX );
    new GLUI_Checkbox( glui, "Part 3: Move Y", &iToolAnyMoveY );
    new GLUI_Checkbox( glui, "Part 3: Move Z", &iToolAnyMoveZ );
    new GLUI_Checkbox( glui, "Part 3: Rotate X", &iToolAnyRotateX );
    new GLUI_Checkbox( glui, "Part 3: Rotate Y", &iToolAnyRotateY );
    new GLUI_Checkbox( glui, "Part 3: Rotate Z", &iToolAnyRotateZ );
    new GLUI_StaticText( glui, "Part 4. Histogram and HSV Color Palette." );
    new GLUI_StaticText( glui, "        The Part 4 in the \"Transfer Function Editor\"." );
    new GLUI_StaticText( glui, "Part 5. Add, delete, or move little color triangles." );
    new GLUI_StaticText( glui, "        The Part 5 in the \"Transfer Function Editor\"." );
    new GLUI_Checkbox( glui,   "        Add triangles.", &iAddTriangles );
    new GLUI_Checkbox( glui,   "        Delete triangles.", &iDeleteTriangles );
    new GLUI_Checkbox( glui,   "        Move triangles.", &iMoveTriangles );
    new GLUI_StaticText( glui, "Part 6. Change the color of the color triangle." );
    new GLUI_StaticText( glui, "        The Part 6 in the \"Transfer Function Editor\"." );
    new GLUI_Checkbox( glui,   "        Change Color.", &iChangeColor );
    new GLUI_StaticText( glui, "Part 7. Display opacity effects." );
    new GLUI_StaticText( glui, "        The Part 7 in the \"Transfer Function Editor\"." );
    new GLUI_StaticText( glui, "Part 8. Enable blending with opacity curve." );
    new GLUI_StaticText( glui, "        The Part 8 in the \"Transfer Function Editor\"." );
    new GLUI_StaticText( glui, "Part 9. Save and load default.tfe." );
    new GLUI_Button( glui, "Save to default.tfe.", 11, DefaultTFE );
    new GLUI_Button( glui, "Load to default.tfe.", 12, DefaultTFE );
    new GLUI_Checkbox( glui, "Part 10. Marching Cubes CPU version", &iToolMarchingCubesCPU );
    glui->set_main_gfx_window( window );
    GLUI_Master.set_glutIdleFunc( myGlutIdle ); 


    // marching Cubes using CPU version
    printf("Please wait...about three minutes. thanks.\n");
    vertices = runMarchingCubes(parseRawFile(".\\CT_head.dat", 240, 256, 256 ), 
        240, 256, 256, 1, 1, 1, 37.0);

    
    // window 3
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(300,300);
    window3 = glutCreateWindow("Marching Cubes CPU version");
    printf("window3=%d\n",window3);
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutMainLoop();
    

    return 0;             /* ANSI C requires main to return int. */
}
