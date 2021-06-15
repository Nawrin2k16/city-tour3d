#include<stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#ifdef __WIN64
#include <windows.h>  //for Sleep() only
#include<GL/glut.h>
#elif __WIN32
#include <windows.h>  //for Sleep() only
#include<GL/glut.h>
#elif __APPLE_CC__
#include <GLUT/glut.h>
#elif __linux
#include<GL/glut.h>
#elif __unix
#include<GL/glut.h>
#endif
#include<limits.h>
#include<unistd.h>
#include<time.h>
#include<math.h>

#include<cstring>
#include<sstream>
#include<iostream>
#include<string>
#include<vector>
#include<windows.h>
#include<mmsystem.h>

using namespace std;

#include "BmpLoader.h"


#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define M_PI 3.1416
#define DEF_D 5

#define MAX_CARS 3

//static DWORD last_idle_time;
//int game_state = 1;

#include <string>

std::string timerValue;

int Ran= 1;
int turn =0;

enum state
{
    start,EXIT,menu
}; // to define state of our game

state game_state;
//int MenuBox = -1;

int setDis = 0;
int cone;
bool g = false, Stop = false ;
bool over = false;
float halfWidth = (float)(WINDOW_WIDTH/2.0);
float halfHeight = (float)(WINDOW_HEIGHT/2.0);

int win_Race = 0, Lose_Race = 0;
int Collition = 0;
//Car variables
int Ins = 0;

bool buton_press = true;
bool steerSpeed = false;
int seconds = 0;
int minutes =0, hours =0;
int temp = 10000;
int dist = 0;
int fuel = 200;

//for curve
int anglex= 0, angley = 0, anglez = 0;          //rotation angles
int window;
int wired=0;
int shcpt= 0;
int animat = 0;
const int L=20;
const int dgre=3;
int ncpt=L+1;
int clikd=0;
const int nt = 40;				//number of slices along x-direction
const int ntheta = 20;

GLfloat ctrlpoints[L+1][3];


void showControlPoints()
{
    glPointSize(5.0);
    //glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_POINTS);
    for (int i = 0; i <=L; i++)
        glVertex3fv(&ctrlpoints[i][0]);
    glEnd();
}



unsigned int ID[100];

float rotationAngle = 30.0f, ang = 0;
float SunAngle = 0, dx = .5 ;
float  sunR = 0;
bool  lol = false;
// angle of rotation for the camera dirdirection
float angle = 0.0, yAngle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = -5.0f, z = 30.0f, y = 4.5f;
double xAngle = 0.0f;
float roll = 0.0f, x_col = 1, y_col = 1, z_col = 1 ;

float mouseX = 0.0f, mouseY = 0.0f;
//Spotlight


GLfloat spot_direction[] = { x+lx,  y, -(z+lz) };
bool MouseC = false;

//int OpenDoor = 0;
bool amb = true, dif = true, spec = true;
//, amb2=1, dif2 = 1, spec2 = 1, amb3=1, dif3 = 1, spec3 = 1;
bool Sl = false, silent = false;

class point1
{
public:
    point1()
    {
        x1=0;
        y1=0;
    }
    int x1;
    int y1;
} clkpt[2];

int fl = 0;
GLint viewport[4]; //var to hold the viewport info
GLdouble modelview[16]; //var to hold the modelview info
GLdouble projection[16]; //var to hold the projection matrix info
float wcsClkDn[3],wcsClkUp[3];

void scsToWcs(float sx,float sy, float wcsv[3] );

//control points
long long nCr(int n, int r)
{
    if(r > n / 2) r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for(i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

void BezierCurve ( double t,  float xy[2])
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;
    for(int i=0; i<=L; i++)
    {
        int ncr=nCr(L,i);
        double oneMinusTpow=pow(1-t,double(L-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*ctrlpoints[i][0];
        y+=coef*ctrlpoints[i][1];

    }
    xy[0] = float(x);
    xy[1] = float(y);

    //return y;
}

void setNormal(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(-Nx,-Ny,-Nz);
}

void WaterPot()
{
    int i, j;
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;
    //number of angular slices
    const float dtheta = 2*M_PI / ntheta;		//angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    BezierCurve( t,  xy);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i = 0; i < nt; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        BezierCurve( t,  xy);
        x1 = xy[0];
        r1 = xy[1];

        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= ntheta; ++j )
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = r * cosa;
            y1 = r1 * cosa;	//current and next y
            z = r * sina;
            z1 = r1 * sina;	//current and next z

            //edge from point at x to point at next x
            glVertex3f (x, y, z);

            if(j>0)
            {
                setNormal(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);
            }
            else
            {
                p1x=x;
                p1y=y;
                p1z=z;
                p2x=x1;
                p2y=y1;
                p2z=z1;

            }
            glVertex3f (x1, y1, z1);

            //forms quad with next pair of points with incremented theta value
        }
        glEnd();
        x = x1;
        r = r1;
    } //for i
}

void spotlight1(float m, float n, bool ON = false)
{
    GLfloat nolight[] = { 0.0, 0.0, 0.0, 1.0 };

    GLfloat light1_ambient[] = { 1, 1, 1, 1.0 };
    GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_position[] = { m,  0.5, n, 1.0 };

    glEnable(GL_LIGHT1);

    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.005);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0005);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0002);

    if(ON == true)
    {
        glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
        glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
    }
    else
    {
        glLightfv(GL_LIGHT1, GL_AMBIENT, nolight);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, nolight);
        glLightfv(GL_LIGHT1, GL_SPECULAR, nolight);
    }
}


void spotlight2(float m, float n, bool ON = 0)
{
    GLfloat nolight[] = { 0.0, 0.0, 0.0, 1.0 };

    GLfloat light2_ambient[] = { 1, 1, 1, 1.0 };
    GLfloat light2_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light2_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light2_position[] = { m,  0.5, n, 1.0 };

    glEnable(GL_LIGHT2);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.05);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.005);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.002);

    if(ON == true)
    {
        glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
        glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);

        glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.0);
    }
    else
    {
        glLightfv(GL_LIGHT2, GL_AMBIENT, nolight);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, nolight);
        glLightfv(GL_LIGHT2, GL_SPECULAR, nolight);
    }

}


void Material(float x_col,float y_col,float z_col, int Sph = 1)
{

    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { x_col, y_col, z_col, 1.0 };
    GLfloat mat_shininess[] = {32};
    //GLfloat maFspot

    GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_diffuse[] = {1, 1, 1, 1.0 };

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    if(Sph == 3 && SunAngle <180)
    {
        GLfloat mat_emission[] = { 1, (sunR)/90,  sunR/90, 1.0 };


        glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
        glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, no_mat);
        glMaterialfv( GL_FRONT, GL_DIFFUSE, no_mat);
    }
    else
    {
        glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
        glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, no_mat);
    }
    //glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    if(Sph == 0)
    {

        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glEnable(GL_TEXTURE_2D);
    }
}

void water()
{
    GLfloat cp[L+1][3] =
    {
        {1, 8, 13},
        {1.45, 5.475, 13}, {1.46, 5.35, 13},
        {1.55, 5.8, 13}, {1, 5.9, 13},
        {1.7, 5.225, 13}, {1.975, 5.35, 13},
        {1.5, 0.25, 13}, {1.3, 1.2, 13},
        {1.175, 2, 13}, {1.75, 2.625, 13},
        {1.75, 3.1, 13}, {2.1, 3.9, 13},
        {1.95, 4.675, 13}, {2.925, 5.425, 13},
        {3.875, 5.975, 13}, {5.3, 6.475, 13},
        {6.85, 6.825, 13}, {5.05, 7.2, 13},
        {5.025, 4.925, 13}, {5.35, 5.175, 13}
    };

    for (int i=0; i<L+1; i++)
    {
        for(int j=0; j<3; j++)
        {
            ctrlpoints[i][j] = cp[i][j];
        }
    }
    glPushMatrix();
    if(shcpt)
    {
        Material(1, 0,0);
        showControlPoints();
    }
    glPopMatrix();
}


void displayT(char * str, float x, float y, float z, int width = 30)
{
    glDisable(GL_LIGHTING);
    glPushMatrix();
    //glColor3f(1, 1, 1); // Set color to red purely for testing.
    glLineWidth(width);

    glRasterPos3f(x-0.8, y-1.85, z-7.9);
    //glRasterPos3f(x, y, z-8);
    for (int c=0; str[c] != ' ' ; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[c]);
    }
    glPopMatrix();
    glEnable(GL_LIGHTING);
}


void box(float x_min, float x_max, float y_min, float y_max, float z_min, float z_max, int Col = 1, int Rep = 1)
{
    glBegin(GL_QUADS);
    Material(x_col,y_col, z_col);
    //back

    glNormal3f(0, 0, 1);
    glTexCoord2f(Rep, Rep);
    glVertex3f(x_min, y_min, z_max);

    glNormal3f(0, 0, 1);
    glTexCoord2f(0,Rep);
    glVertex3f(x_max, y_min, z_max);

    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);
    glVertex3f(x_max, y_max, z_max);

    glNormal3f(0, 0, 1);
    glTexCoord2f(Rep,0);
    glVertex3f(x_min, y_max, z_max);


    //front
    glNormal3f(0, 0, -1);
    glTexCoord2f(Rep, Rep);
    glVertex3f(x_min, y_min, z_min);

    glNormal3f(0, 0, -1);
    glTexCoord2f(0,Rep);
    glVertex3f(x_min, y_max, z_min);

    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0);
    glVertex3f(x_max, y_max, z_min);

    glNormal3f(0, 0, -1);
    glTexCoord2f(Rep,0);
    glVertex3f(x_max, y_min, z_min);

    //Right
    //Material(x_col-0.04,y_col-0.04, z_col-0.04);
    glNormal3f(1, 0, 0);
    glTexCoord2f(Rep, Rep);
    glVertex3f(x_max, y_min, z_min);

    glNormal3f(1, 0, 0);

    glTexCoord2f(Rep,0);
    glVertex3f(x_max, y_max, z_min);

    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0);
    glVertex3f(x_max, y_max, z_max);

    glNormal3f(1, 0, 0);
    glTexCoord2f(0,Rep);
    glVertex3f(x_max, y_min, z_max);

    //Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(Rep,Rep);
    glVertex3f(x_min, y_min, z_min);

    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,Rep);
    glVertex3f(x_min, y_min, z_max);

    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0);
    glVertex3f(x_min, y_max, z_max);

    glNormal3f(-1, 0, 0);
    glTexCoord2f(Rep,0);
    glVertex3f(x_min, y_max, z_min);

    //top
    //Material(x_col,y_col, z_col);

    glNormal3f(0, 1, 0);
    glTexCoord2f(Rep,Rep);
    glVertex3f(x_max, y_max, z_max);

    glNormal3f(0, 1, 0);
    glTexCoord2f(0,Rep);
    glVertex3f(x_min, y_max, z_max);

    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0);
    glVertex3f(x_min, y_max, z_min);

    glNormal3f(0, 1, 0);
    glTexCoord2f(Rep,0);
    glVertex3f(x_max, y_max, z_min);

    //bottom
    //Material(x_col-0.1,y_col-0.1, z_col-0.1);
    glNormal3f(0, -1, 0);
     glTexCoord2f(Rep, Rep);
    glVertex3f(x_max, y_min, z_max);

    glNormal3f(0, -1, 0);
    glTexCoord2f(0,Rep);
    glVertex3f(x_min, y_min, z_max);

    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0);
    glVertex3f(x_min, y_min, z_min);

    glNormal3f(0, -1, 0);
    glTexCoord2f(Rep,0);
    glVertex3f(x_max, y_min, z_min);


    glEnd();

    //x -1.5, x+1.5, 1.6, 1.7, z-10.5, z-7.3


    if(Col == 1)
    {
        if((x > -20 && x < 22) || (x<-1020 && x> -980) || (x>980 && x<1000) || (x>1980 && x<2020) || (x<-1980 && x>-2020))
        {
            if(x+1.4 >= x_min && x-1.4 <= x_max && z+2.2 >= z_min && z-5.5 <= z_max)
            {
                //printf("Road 1 CRUSH");
                ////printf("\nPosition : %f and %f, X from %f to %f, Z from %f to %f\n",x ,z ,x_min, x_max, z_min,  z_max );
                Collition = 1;
            }
        }
        else if((z >-40  && z < 25) || (z>-1025 && z< -980) || (z<980 && z>1025 ) ||  (z<1980 && z>2025 ))
        {
            if(x+2.2 >= x_min && x-5.7 <= x_max && z+1.5>= z_min && z-1.5 <= z_max)
            {
                //printf("Road 2 CRUSH");
                //printf("\nPosition : %f and %f, X from %f to %f, Z from %f to %f\n",x,z,x_min, x_max, z_min,  z_max );
                Collition = 1;
            }
        }

        else if(x+1.4 >= x_min && x-1.4 <= x_max && z+1.5>= z_min && z-1.5 <= z_max)
        {
            //printf("Road OTHER CRUSH");
            //printf("\nPosition : %f and %f, X from %f to %f, Z from %f to %f\n",x,z,x_min, x_max, z_min,  z_max );
            Collition = 1;
        }
    }
}

void sunlight()
{
    float Red = 1;

    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };


    if(SunAngle < 180)
    {
        if(SunAngle < 90)
        {
            sunR += dx;
        }
        else
            sunR -= dx;
        GLfloat light_ambient[] = { 1, 1,  1, 1.0 };

        if(sunR< 72)
        {
            GLfloat light_amb[] = { Red, double((sunR)/90+0.2),  double(sunR/90+0.2), 1.0 };
            for(int i=0; i<4; i++)
                light_ambient[i] = light_amb[i];
        }

        GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };


        GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light_position[] = { 0, 0, 0, 1.0 };

        glEnable( GL_LIGHT0);

        if(amb == true)
            glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        else
            glLightfv(GL_LIGHT0, GL_AMBIENT, no_light);
        if(dif == true)
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        else
            glLightfv(GL_LIGHT0, GL_DIFFUSE, no_light);

        if(spec == true)
            glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        else
            glLightfv(GL_LIGHT0, GL_SPECULAR, no_light);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    }
    else
    {
        sunR = 0;
        Red = 0;
        glDisable(GL_LIGHT0);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, no_light);
    }
}

void sun()
{
    SunAngle += dx;
    if(SunAngle > 360)
        SunAngle = 0;
    glPushMatrix();
    Material(1, 1, 1, 3);
    glRotatef(SunAngle, -x, 0, 0);
    glTranslatef(x-5, y, z+30-130);
    //glutSolidSphere(2.5, 25, 25);
    //gluDisk(quadratic, 0.001, 2, 15, 15);

    sunlight();
    glPopMatrix();
}

void menu_bars( float y1, float y2, float x1 = x+lx-5, float x2 = x+lx+ 5)
{
    if(over == false)
    {
        glPushMatrix();
        Material(1, 1, 1);
        glScalef(2, 2, 2);
        x_col = 0.4, y_col = 0.4, z_col = 0.4;
        box(x1, x2, y1, y2, z+lz-2, z+lz+0.01, 0);
        glPopMatrix();
    }
}

void pick(int xx, int yy)
{
    if(xx > 301 && xx < 725)
    {
        if(yy >295 && yy<410 )
        {
            game_state = start;

        }
        else if(yy >430 && yy<550 )
        {
            game_state = menu;
            //Ins = 1;
        }
        else if(yy >570 && yy<690)
            game_state = EXIT ;
    }
    glutPostRedisplay();
}

void star(float i, float j)
{

    glBindTexture(GL_TEXTURE_2D, ID[10]);
    glEnable(GL_TEXTURE_2D);
    Material( 1, 0, 0, 0);
    glPushMatrix();
    glTranslatef(i, 1, j);
    glRotatef(rotationAngle, 1, 0, 0);
    glTranslatef(-i, -1, -j);
    //1st Triangle
    glBegin(GL_TRIANGLES);
    glVertex3f(i-1, 1.0, j);
    glVertex3f(i+0.5, 1, j+1);
    glVertex3f(i+0.5, 1, j-1);
    glEnd();

    //12nd Triangle
    glBegin(GL_TRIANGLES);
    glVertex3f(i-0.5, 1.0, j+1);
    glVertex3f(i+1, 1, j);
    glVertex3f(i-0.5, 1.0, j-1);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    setDis = 0;

    Material(1, 1,1 );
    if(x >= i-2 && x <= i+2 && z>= j-2 && z <= j+2)
    {
        if(fuel<900)
        {
            setDis = 1;
            fuel+=100;
            if(silent == false) PlaySound("D:\\Sounds\\success-sound-effect.wav", NULL, SND_ASYNC|SND_FILENAME);
        }
        else
        {
            setDis = 2;
            temp -=100;
            if(silent == false) PlaySound("D:\\Sounds\\drop.wav", NULL, SND_ASYNC|SND_FILENAME);
        }
    }
    if(setDis == 1 )
    {
        glPushMatrix();
        glColor3f(0, 1, 0);
        displayT("+100 ", x+lx, y+ly, z+lz);
        glPopMatrix();
    }
    else if(setDis == 2)
    {
        glPushMatrix();
        glColor3f(1, 0, 0);
        displayT("FUEL_LIMIT_EXCEEDED ", x+lx, y+ly+1, z+lz);
        glTranslatef(0, -0.6, 0);
        displayT("PENALTY_ADDED ",  x+lx, y+ly+1, z+lz);
        glPopMatrix();
    }
}

void wheel(float x, float z, int weight)
{

    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    glPushMatrix();

    if(weight == 0)
    {
        glTranslatef(x, 0.45, z);
        glRotatef(-rotationAngle, x, 0, 0);
        glTranslatef(-x, -0.45, -z);
    }


    glPushMatrix();
    glTranslatef(x, 0.45, z);
    glRotatef(90, 0, 1, 0);

    glBindTexture(GL_TEXTURE_2D, ID[4]);
    glEnable(GL_TEXTURE_2D);

    Material(0.2,0.2,0.2, 0);

    glutSolidTorus(0.15, 0.45, 25, 25);

    //Material(1, 1.0, 1.0);

    //gluCylinder(quadratic, 0.4f, 0.4f, 0.15f, 32, 32);

    gluQuadricTexture(quadratic, 1);

    Material(0.7, 0.7, 0.7, 0);
    gluDisk(quadratic,0.001, 0.4, 25, 25);

    glDisable(GL_TEXTURE_2D);


    glPopMatrix();
    glPopMatrix();
}

void surface(float x, float z)
{
    glPushMatrix();
    glTranslatef(0, 1.1, (z-6.0));

    glRotatef(-20, 1, 0, 0);
    glTranslatef(0, -1.1, -(z-6.0));
    x_col = 0.4, y_col = 0.4, z_col = 0.4;
    box(x -1.5, x+1.5, 0.9, 1.5, z-7.1, z-5.25, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 1.1, (z-1.0));

    glRotatef(-50, 1, 0, 0);
    glTranslatef(0, -1.1, -(z-1.0));
    x_col = 0.2, y_col = 0.2, z_col = 0.2;
    box(x -1.5, x+1.5, 0.9, 2.5, z-1.5, z-1.4, 0);
    glPopMatrix();
}

void drawCar(float x, float z, int l= 8, int weight = 1)
{
    //drawOCar(x, z-9);

    glBindTexture(GL_TEXTURE_2D, ID[l]);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    //top
    if(weight == 0)
    {
        x_col = 0.4;
        y_col = 0.4;
        z_col =0.4;
    }
    else
    {
        x_col = (l+1) /12 +0.1;
        y_col = (l) /11 ;
        z_col = (l+1) /12;

    }
    box(x -1.5, x+1.5, 1.6, 1.7, z-5.5, z-2.3, weight );

    glPushMatrix();
    //glass wrapped
    x_col = 0.4;
    y_col = 0.4;
    z_col = 0.4;
    box(x -1.4, x+1.4, 0.9, 1.2, z-1.75, z-1.68,weight );
    box(x -1.5, x+1.5, 0.9, 1.2, z-7.1, z-6.9,weight );

    //upper part of the bottom

    glTranslatef(0, 1.1, (z-1.0));
    glRotatef(-50, 1, 0, 0);
    glTranslatef(0, -1.1, -(z-1.0));

    box(x -1.5, x-1.4, 0.9, 2.5, z-1.5, z-1.4,weight );
    box(x +1.4, x+1.5, 0.9, 2.5, z-1.5, z-1.4,weight );
    //box(x -1.5, x-1.4, 0.9, 2.5, z-11.5, z-10.25);
    //box(x +1.4, x+1.5, 0.9, 2.5, z-11.5, z-10.25);
    glPopMatrix();

    //botthom part

    box(x-1.5, x+1.5, 0.7, 0.8, z-0.61, z-0.58,weight );
    box(x-1.5, x+1.5, 0.5, 0.7, z-6.0, z-0.61,weight );

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //wheel

    wheel(x-1.5, z-2.0, weight);

    wheel(x+1.5, z-2.0, weight);

    wheel(x-1.5, z-6.0, weight);

    wheel(x+1.5, z-6.0, weight);

    //glasses
    x_col = 1;
    y_col = 1;
    z_col = 1;
    glBindTexture(GL_TEXTURE_2D, ID[3]);
    glEnable(GL_TEXTURE_2D);

    box(x -1.5, x+1.5, 0.9, 1.1, z-0.6, z-0.58,weight );
    box(x -1.5, x+1.5, 0.9, 1.1, z-6.0, z-6.01,weight );

    box(x -1.5, x+1.5, 1.0, 1.4, z-2.5, z-5.8,weight );

    //box(x +1.5, x+1.51, 0.9, 1.6, z-7.5, z-10.8);
    Material(0.3,0.3,0.3);
    glPushMatrix();

    surface(x, z);
    //back mirror
    box(x -1.7, x-1.4, 1.4, 1.41, z-6.5, z-6.2,weight );
    box(x +1.4, x+1.7, 1.4, 1.41, z-6.5, z-6.2,weight );



    box(x +1.5, x+1.9, 1.25, 1.55, z-6.5, z-6.4,weight );
    box(x -1.9, x-1.5, 1.25, 1.55, z-6.5, z-6.4,weight );
    if(weight == 0)
    {
        glPushMatrix();

        spotlight1(x+1.5, z-6.5, Sl);
        spotlight2(x-1.5, z-6.5,Sl);
        glPopMatrix();
    }


    glPushMatrix();
    glTranslatef(0,0,5);
    //back and front slope
    for (float i=0.8; i<1.2; i+=0.02)
    {
        box(x-1.5, x+1.5, i, i+0.2, z-i*8, z-i*7,weight );
    }

    float j= 0.8;
    for (float i=1.2; i>0.8; i -= 0.02)
    {
        box(x-1.5, x+1.5, i-0.2, i+0.2, z-j*10.8, z-j*9,weight );
        j += 0.02;
    }
    glPopMatrix();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

int k =0;

void coneDiv(int i, int j)
{
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, ID[21]);
    glEnable(GL_TEXTURE_2D);
    //gluQuadricTexture(quadratic, 1);

    x_col = 1, y_col = 1, z_col = 0.4;
    box(i-0.25, i+0.25, -0.5, 0.5, j-0.25, j + 0.25);
    glPushMatrix();

    glTranslatef(i, 0, j);
    glRotatef(-90, 1, 0, 0);
    Material(1, 0, 0);
    glutSolidCone(0.25, 2, 25, 25);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

void setCone()
{
    int len =1000;
    int Ran = 1;
    for(int i = -len+5 ; i<len-5; i+=75)
    {
        Ran ++;
        //straight road and paralle 4 lines
        coneDiv(0, i);
        coneDiv(2000, i);
        coneDiv(-2000, i);
        coneDiv(-1010, i+1000);
        coneDiv(990, i+1000);
        coneDiv(i, 2000);


        if(Ran == 3|| Ran ==1)
        {
            if(Ran == 3)
            {
                //main straight
                star(5, i+5);
                drawCar(-5, i-10, 8);
                //Rightmost straight
                star(2005, i+2);
                drawCar(2005, i+15, 9);
                //leftmost straight
                star(-2005, i+2);
                drawCar(-2005, i-15, 10);
                //middleRight line
                star(995, i+1000+2);
                drawCar(995, i+1000-15, 11);
                //middleLeft line
                star(-1015, i+1000+2);
                drawCar(-1015, i+1000-15, 8);
                Ran = 0;

            }
            else
            {
                //main straight
                drawCar(5, i+1, 11);
                star(-5, i-1);
                //Rightmost straight
                drawCar(1995, i-10, 9);
                star(1995, i-1);
                //leftmost straight
                drawCar(-1995, i-10, 10);
                star(-1995, i+16);
                //middleRight line
                drawCar(985, i+1000-10, 11);
                star(985, i+1000+16);
                //middleLeft line
                drawCar(-1005, i+1000-10, 8);
                star(-1005, i+1000+16);

                //top horizontal line right
                star(i+1000-10, -995);
                coneDiv(i+1000, -990);
                star(i+1000+11, -985);
                //middle horizontal line right
                if(i<0)
                {
                    star(i+1000-13, 5);
                    coneDiv(i+1000, 0);
                    star(i+1000+13, -5);
                }
                //bottom horizontal line right
                star(i+1000-14, 1005);
                coneDiv(i+1000, 1010);
                star(i+1000+16, 1015);

                //top horizontal line left
                star(i-1000, -1005);
                coneDiv(i-1000, -1010);
                star(i-1000, -1015);
                //middle horizontal line left
                if(i>0)
                {
                    star(i-1000, -5);
                    coneDiv(i-1000, 0);
                    star(i-1000-25, 5);
                }
                //bottom horizontal line left
                star(i-1000-20, 995);
                coneDiv(i-1000, 990);
                star(i-1000+20, 985);
                //bottommost horizontal line
                star(i+55, 2005);
                star(i-35, 1995);

            }
        }
    }
}

void divider()
{
    Material(1, 1, 1);

    box(-0.2, 0.25, -0.2, 0.0051, -3, 3, 0);

    glBindTexture(GL_TEXTURE_2D, ID[16]);
    glEnable(GL_TEXTURE_2D);

    x_col = 1, y_col = 1, z_col = 1;

    glPushMatrix();
    //glTranslatef(0, 0.1, 20);
    Material(1.0, 0.0, 0.0);

    box(-12, 12, -0.21, -0.2, -20, 20, 0, 2);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void floorT(float len)
{

    glBindTexture(GL_TEXTURE_2D, ID[1]);
    glEnable(GL_TEXTURE_2D);


    x_col = 0.4;
    y_col = 0.4;
    z_col = 0.4;

    box(-10, 10, -0.1, 0.005, (-1.0)*len, len, 0);

    glDisable(GL_TEXTURE_2D);
    Material(1, 1, 1);
    glPushMatrix();
    glTranslatef(0, 0, 5);
    //glRotatef(90, 0, 0, 1);
    //int j=0;
    //int j = 1;
    for (int i=-len; i< len; i+= 24)
    {

        glPushMatrix();
        glTranslatef(0, 0, i );
        divider();
        glPopMatrix();

    }
    glPopMatrix();

}

void road()
{

    int len =1000;
    glPushMatrix();
    //strict divider
    setCone();

    //Roads
    for(int i=0; i<4; i++)
    {

        glTranslatef(-(len-10),0, -(len+10));
        glRotatef(90, 0, 1, 0);

        floorT(len);

    }
    glPopMatrix();
    glPushMatrix();

    for(int i=0; i<4; i++)
    {

        glTranslatef((len-10),0, (len+10));
        glRotatef(90, 0, 1, 0);

        floorT(len);

    }
    glPopMatrix();

    glRotatef(-90, 0, 1, 0);
    glPushMatrix();

    for(int i=0; i<4; i++)
    {

        glTranslatef((len-10),0, (len+10));
        glRotatef(90, 0, 1, 0);

        floorT(len);

    }
    glPopMatrix();


}

void flag();

void pole()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    glBindTexture(GL_TEXTURE_2D, ID[15]);
    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(quadratic, 1);


    Material(1, 1, 1);

    x_col = 0.7;
    y_col = 0.7;
    z_col = 0.7;
    box(-12, -11.6, -0.5, 0.5, 11.6, 12);
    glPushMatrix();
    glTranslatef(-11.8, 5, 11.8);
    glRotatef(90, 1, 0, 0);
    gluCylinder(quadratic, 0.1f, 0.1f, 10, 32, 32 );
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //flag
    flag();
}

void twig(float x, float y, float z)
{

    glPushMatrix();
    glTranslatef(-16, 3.8, -20.25);
    glRotatef(50, 0, 0, -20);
    glTranslatef(16, -3.8, 20.25);

    x_col = 0.4;
    y_col = 0.4;
    z_col = 0.4;
    box(x-1,x+1,y-0.1, y+0.1, z-0.2,z+0.2);

    glPopMatrix();

}

void tunnel()
{
    glBindTexture(GL_TEXTURE_2D, ID[8]);
    glEnable(GL_TEXTURE_2D);
    Material(0.5, 0.5, 0.5);
    x_col = 1;
    y_col = 1;
    z_col = 1;
    box(2011.5, 2012, -0.25, 1.5, -978, 882, 1);
    box(1988, 1988.5, -0.25, 1.5, -978, 882, 1);


    box(15.0, 970, -0.5, 1.5, -12, -11, 1);
    box(15.0, 970, -0.5, 1.5, 11, 12, 1);

    glDisable(GL_TEXTURE_2D);


    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    glBindTexture(GL_TEXTURE_2D, ID[7]);
    glEnable(GL_TEXTURE_2D);
    //gluQuadricTexture(quadratic, 1);

    glPushMatrix();
    glTranslatef(2000, 0, -978);
    //gluQuadricTexture(quadratic, 1);
    Material(0.5, 0.5, 0.5, 0);

    gluCylinder(quadratic, 12, 12, 1860, 25, 25);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glBindTexture(GL_TEXTURE_2D, ID[10]);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    Material(0, 0, 0, 0);
    glRotatef(90, 0, 1, 0);
    glTranslatef(0, 0, 15);
    gluCylinder(quadratic, 12, 12, 960, 25, 25);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void tree()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    glBindTexture(GL_TEXTURE_2D, ID[15]);
    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(quadratic, 1);


    Material(0, 0, 0, 0);

    glPushMatrix();
    glPushMatrix();
    glTranslatef(-15, 5, -20);
    glRotatef(90, 1, 0, 0);
    gluCylinder(quadratic, 0.5f, 0.7f, 10, 5, 5 );

    //glTranslatef(0, 2.3, 0);
    glPopMatrix();
    Material(0, 0, 0);
    twig(-16, 3.9, -19.8);

    glPushMatrix();
    Material(0, 0, 0, 0);
    glTranslatef(-16, 3.8, -20.25);
    glRotatef(180, 0, 1, 0);
    glTranslatef(14, -3.8, 20.25);
    twig(-16, 3.9, -19.8);
    glPopMatrix();
    glPopMatrix();

    Material(0, 0, 0, 0);
    box(-16.0, -14.0, -0.5, 0.5, -21, -19);

    //-17.0, -15.0, 3.8, 4, -20, -19.6

    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, ID[5]);
    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(quadratic, 1);

    Material(0, 0, 0, 0);

    glPushMatrix();
    glTranslatef(-15, 5, -20);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.6f, 1.7f, 12, 12);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-15, 6, -20);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.6f, 1.5f, 5, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-15, 6.7, -20);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.6f, 1.0f, 15, 15);
    glutSolidSphere(1.2f, 5, 5);
    glPopMatrix();


    glDisable(GL_TEXTURE_2D);
}

void forest()
{

    for (int i =980  ; i>-980; i -= 50)
    {
        glPushMatrix();
        glTranslatef(-1960, 0, i);
        tree();
        glPopMatrix();
    }

    //trees infront of houses
    for (int i = -945; i<980; i += 35)
    {
        glPushMatrix();
        glTranslatef(0, 0, i);
        if (i <-100 || i>-35 )
            tree();
        glPopMatrix();
    }
    //trees wrapper
    glBindTexture(GL_TEXTURE_2D, ID[5]);
    glEnable(GL_TEXTURE_2D);
    Material(1, 1, 1);

    x_col = 0.24;
    y_col = 0.24;
    z_col = 0.24;
    box(-1982.5, -1982, -0.5, 2.5, -985, 975,1, 4);

    box(-1968.5, -1968, -0.5, 2.5, -985, 975, 1, 4);

    glDisable(GL_TEXTURE_2D);

}

GLfloat CntrlPt[4][4][3];


void init_surface(void)
{
    GLfloat cp[L+1][3] =
    {
        {-2.2, -0.0250002, 13},
        {-0.7, 6.825, 13},
        {-0.0749999, 0.45, 13},
        {0.1, 0.325, 13},
        {0.175, 0.225, 13},
        {0.375, 0.0750001, 13},
        {1.4, 5.27275, 13},
        {1.625, 5.275, 13},
        {1.975, 7.325, 13},
        {3.975, 6.325, 13},
        {2.025, 0.575, 13},
        {2.75, 4.675, 13},
        {3, 4.7, 13},
        {3.55, 4.7, 13},
        {3.925, 0.4, 13},
        {4.775, -0.05, 13},
        {1.975, 7.325, 13},
        {5.9, 3.925, 13},
        {6.975, 8.325, 13},

        {6.1, 6.75, 13},
//{6.1, 0.275, 13},
        {6.775, 0.05, 13}
    };


    for (int i=0; i<L+1; i++)
    {
        for(int j=0; j<3; j++)
        {
            ctrlpoints[i][j] = cp[i][j];
        }
    }
    glPushMatrix();
    if(shcpt)
    {
        Material(1, 0,0);
        showControlPoints();
    }
    glPopMatrix();
}

void cone_flag(void)
{
    int u, v;
    for (u = 0; u < 4; u++)
    {
        for (v = 0; v < 4; v++)
        {
            CntrlPt[u][v][0] =  0.5*((GLfloat)u - 1.5);
            CntrlPt[u][v][1] = 0.01*(GLfloat)v;

            if ( (u == 1 || u == 2 || u == 3) && (v == 1 || v == 2))
                CntrlPt[u][v][2] = 0.5;
            else
                CntrlPt[u][v][2] = -0.5 *(rand() % 2);
        }
    }
    glPushMatrix();

    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
            0, 1, 12, 4, &CntrlPt[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_AUTO_NORMAL);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    glPopMatrix();
}

void flag()
{
    cone_flag();
    glBindTexture(GL_TEXTURE_2D, ID[21]);
    glEnable(GL_TEXTURE_2D);
    Material(1, 1, 1);
    glPushMatrix();
    glRotatef(90, -1, 0, 0);
    glTranslatef(-12.5, -11.9, 4.8);
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);

    glPopMatrix();
    glDisable(GL_AUTO_NORMAL);

    glDisable(GL_TEXTURE_2D);
}

void waterfall()
{
    glPushMatrix();
    glScalef(2, 2, 2);
    //waterSurface();
    glPushMatrix();
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    glBindTexture(GL_TEXTURE_2D, ID[7]);
    glEnable(GL_TEXTURE_2D);
    Material(1, 1, 1, 0);
    glPushMatrix();
    glTranslatef(0, 3.7, 0);
    //
    for (int i =0 ; i<36; i++)
    {
        glRotatef(rand() % 360, 0, 1, 0);
        glRotatef(-10, 0, 1, 0);
        glPushMatrix();
        glScalef(0.4, 1.6, 0.4);
        glTranslatef(2, -0.5, 0);
        glutSolidTorus(0.03, 2, 3, 24);
        glPopMatrix();
        glPushMatrix();
        glScalef(1.2, 1.6, 1.2);
        glTranslatef(1.3, -0.7, 0);
        glutSolidTorus(0.03, 1.3, 3, 24);
        glPopMatrix();
    }
    glPopMatrix();

    glRotatef(-90, 1, 0, 0);
    //gluQuadricTexture(quadratic, 1);
    gluCylinder(quadratic, 0.1, 0.05, 7, 32, 32);
    glTranslatef(0, 0, 1.8);
    gluDisk(quadratic, 0, 3.5, 15, 15);

    water();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, ID[7]);
    glEnable(GL_TEXTURE_2D);
    Material(0, 0, 0, 0);
    glPushMatrix();
    glScalef(1.3, 1.2, 1.3);
    glTranslatef(0, -1, 0);
    glRotatef(90, 0, 0, 1);
    WaterPot();


    glPopMatrix();
    glDisable(GL_AUTO_NORMAL);
    glDisable(GL_TEXTURE_2D);

}


void riverShift()
{
    glBindTexture(GL_TEXTURE_2D, ID[7]);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    Material(0, 0, 1);
    box(115, 1985, -0.7, -0.45, -970, -930,1, 50);

    //leftside river
    box(-1960, -1040, -0.7, -0.45, -980, 970, 1, 50);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ID[5]);
    glEnable(GL_TEXTURE_2D);
    Material(1, 1, 1);
    box(110, 1985, -0.7, 0.45, -972, -970);
    box(108, 110, -0.7, 0.45, -972, -900);
    glDisable(GL_TEXTURE_2D);
}

void gate_top()
{
    GLfloat top[L+1][3] =
    {
        {-2.275, 0.09, 13},
        {-2.05, 1.45, 13},
        {-1.6, 2.375, 13},
        {-1.1, 3.55, 13},
        {-0.85, 4.225, 13},
        {-0.3, 4.825, 13},
        {0.275, 5.275, 13},
        {0.75, 5.75, 13},
        {1.2, 5.375, 13},
        {2.35, 4.6, 13},
        {3.025, 3.725, 13},
        {3.725, 2.675, 13},
        {4.525, 2.7, 13},
        {5, 3.225, 13},
        {5.675, 3.9, 13},
        {6.025, 4.875, 13},
        {6.95, 3.925, 13},
        {7.75, 3.175, 13},
        {8.5, 2.675, 13},
        {9.15, 1.45, 13},
        {9.15, 0.09, 13}
    };

    for (int i=0; i<L+1; i++)
    {
        for(int j=0; j<3; j++)
        {
            ctrlpoints[i][j] = top[i][j];
        }
    }
    glPushMatrix();
    if(shcpt)
    {
        Material(1, 0,0);
        showControlPoints();
    }
    glPopMatrix();
}

void Gates()
{
    glBindTexture(GL_TEXTURE_2D, ID[8]);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();

    Material(1, 1, 1);

    x_col = 0.1;
    y_col = 0.1;
    z_col = 0.1;

    for(float i = 12 ; i < 980; i += 5)
    {
        box(12, 12.05, -1.1, 3, i, i +1);
        box(-12.05, -12, -1.1, 2, i, i + 0.5);
    }
    for(float i = -968 ; i < -12; i += 5)
    {
        box(12, 12.5, -1.1, 3, i, i+1.5);
        box(-12.05, -12, -1.1, 2, i, i + 0.5);
        box( -1020.5, -1020, -1.1, 3, i-0.5+985, i+985);
        box( -985.5, -985, -1.1, 3, i-0.5+985, i+985);
    }


    for(float i = -1989 ; i < -15 ; i += 5)
    {

        box(i-0.5, i, -1.1, 3, -989, -988);
    }
    for(float i = -1989 ; i < 1989 ; i += 5)
    {
        if (i<2000 && i>53)
            box(i-1023-0.5, i-1023, -1.1, 3, 1980, 1980.5);
        box(i-0.5, i, -1.1, 3, -1020.5, -1020);
    }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    /*if(((x>11 && x<989) || (x>-989 && x<-11.5)) && ((z> -979 && z<-11) || (z>11 && z<989)))
        Collition = 1;*/

    /*if(( (x>-1989 && x<-1011)&& (z<1989 && z>1011))|| ((x>1011 && x<1989) && (z<1989 && z>1011)))
        Collition = 1;*/

}

void houses(int l = 15)
{
    glBindTexture(GL_TEXTURE_2D, ID[l]);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();

    Material(1, 1, 1);

    //house body
    x_col =0.1;
    y_col =0.01;
    z_col =0.1;
    box(-50, -15, -0.1, 70, -100, -35, 1, 10);


    Material(0, 0, 0);

    //floor cut
    for(float i = -0.1; i < 70; i += 7.1)
    {
        x_col +=i/900;
        y_col +=i/900;
        z_col +=i/1000;
        box(-51, -14, i-0.5, i, -101, -32.5, 1, 10);
        //box(-49, -4, i - )
    }
    for(float i = -50; i < -15; i += 3.5)
    {
        x_col +=0.01;
        y_col +=0.001;
        z_col +=0.001*i/70;
        box(i-0.5, i, -0.1, 70, -101, -32.5, 1, 10);

    }
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, ID[6]);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    Material(0,0,0);
    //glass window
    x_col =1;
    y_col =1;
    z_col =1;
    box(-49.8, -15.2, 2, 68, -100.1, -33.1, 1, 10);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void SkyGround()
{
    //sky box
    //gluQuadricTexture(quadratic, 1);
    if(g == false)
    {
        glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, ID[6]);
        glEnable(GL_TEXTURE_2D);
        Material(1, 1, 1, 0);
        glTranslatef(x, y, z);
        //glScalef(1, 5, 1);
        glutSolidSphere( 100.0, 20, 20 );
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();


        glPushMatrix();
        //glTranslatef(x, y, z);
        sun();
        glPopMatrix();
    }

    //ground box

    glBindTexture(GL_TEXTURE_2D, ID[22]);
    glEnable(GL_TEXTURE_2D);
    Material(0, 0, 0);
    x_col =0.7;
    y_col =0.7;
    z_col =0.7;
    box(-2020, 2020, -4, -0.5, -1050, 2020, 0, 500);


    box(-2020, 2020, -0.5, 3, -1050, -1049);
    box(-2020, 2020, -0.5, 3, 2019, 2020);
    box(-2020, -2019, -0.5, 3, -1050, 2020 );
    box(2019, 2020, -0.5, 3, -1050, 2020);

    glDisable(GL_TEXTURE_2D);

    //grass field

    glBindTexture(GL_TEXTURE_2D, ID[5]);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();

    Material(0, 0, 0);
    box(-985, -30, -0.5, 0.5, 25, 975, 1, 100);

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void shops(float x_g, float z_g, int l, int s)
{
    glBindTexture(GL_TEXTURE_2D, ID[l]);
    glEnable(GL_TEXTURE_2D);
    x_col =0.21;
    y_col =0.19;
    z_col =0.22;
    box(x_g - 10, x_g + 10, -0.5, 5, z_g - 10, z_g+10);
    glPushMatrix();
    for (int i=0; i<4; i++)
    {
        x_col += 0.27;
        y_col += 0.25;
        z_col += 0.2;
        glTranslatef(x_g, 2.5, z_g);
        glScalef(0.8, 1.3, 0.8);
        glTranslatef(-x_g, -2.5, -z_g);
        box(x_g - 10, x_g + 10, -0.5, 5, z_g - 10, z_g+10);
    }

    glPopMatrix();

    //shop windows
    glBindTexture(GL_TEXTURE_2D, ID[3]);
    glEnable(GL_TEXTURE_2D);
    x_col =1;
    y_col =1;
    z_col =1;

    box(x_g -10.15, x_g+10.15, 2.5, 4, z_g-7, z_g-3);
    box(x_g -10.15, x_g+10.15, 2.5, 4, z_g+3, z_g+7);

    //door divider
    x_col =1;
    y_col =1;
    z_col =1;
    box(x_g-3.5, x_g+3.5, -0.5, 4, z_g-10.2, z_g+10.2);
    glDisable(GL_TEXTURE_2D);

    //window frame
    glBindTexture(GL_TEXTURE_2D, ID[15]);
    glEnable(GL_TEXTURE_2D);
    box(x_g - 10.049, x_g+10.049, 2.3, 4.2, z_g-7.2, z_g-2.8);
    box(x_g - 10.049, x_g+10.049, 2.3, 4.2, z_g+2.8, z_g+7.2);
    //window divider
    box(x_g - 10.07, x_g+ 10.07, 2.3, 4.2, z_g - 5.1, z_g - 4.9);
    box(x_g - 10.07, x_g+ 10.07, 2.3, 4.2, z_g + 4.9, z_g + 5.1);

    glPushMatrix();
    Material(1, 1, 1);
    for (int i=0; i<4; i++)
    {
        x_col +=0.3;
        y_col +=0.2;
        z_col +=0.1;
        box(x_g - 11, x_g + 11, 5, 5.2, z_g-11, z_g+11);
        glTranslatef(x_g, 2.5, z_g);
        glScalef(0.8, 1.3, 0.8);
        glTranslatef(-x_g, -2.5, -z_g);
    }
    glPopMatrix();

    //door
    box(x_g-3, x_g +3, -0.5, 4, z_g-10.5, z_g-10.5);


    glDisable(GL_TEXTURE_2D);

    //shop Signboard
    glBindTexture(GL_TEXTURE_2D, ID[s]);
    glEnable(GL_TEXTURE_2D);
    Material(1, 1, 1);
    x_col =1;
    y_col =1;
    z_col =1;
    box(x_g+11, x_g+11.5, 3.5, 7, z_g-4, z_g+4);
    box(x_g-11.5, x_g-11, 3.5, 7, z_g-4, z_g+4);

    glDisable(GL_TEXTURE_2D);

}

void Market()
{
    shops(960, 35, 1, 16);
    int j=9;
    for (int i= 50; i< 1970; i += 40)
    {
        glPushMatrix();
        if(i<970 || i>1015)
            shops(960, 35+i, i%4+8,j%4+17);
        glPopMatrix();
        j++;


        glPushMatrix();
        //glTranslatef(50, 0, i);
        if(i<970 || i>1015)
            shops(1015, 35+i, (i+j)%4+8,j%4+17);
        glPopMatrix();
    }


    for (int i= -2940; i< 1000; i += 40)
    {
        glPushMatrix();

        if(i>45 || (i<0 && i> -945) )
        {
            glPushMatrix();
            //glTranslatef(i, 0, 950);
            shops(960+i, 35+950, j%4+8,j%4+17);
            glPopMatrix();
            j++;
        }
        if(i<-980 && i>-2950)
        {
            glPushMatrix();
            if(i >-1865 || i<-1993)
            {
                //glTranslatef(i, 0, 990);
                shops(960+i, 35+990, (2+j)%4+8,j%4+17);
            }
            glPopMatrix();
        }

        j++;
        glPopMatrix();
    }

}
void shelter();

void HouseSets()
{
    tree();
    houses();
    glPushMatrix();
    glTranslatef(30, -0.5, -40 );
    waterfall();

    glTranslatef(0, 0, 75 );
    waterfall();

    glPopMatrix();

    glPushMatrix();
    init_surface();
    glTranslatef(20, 0, 12 );
    for( int i=0 ; i<30; i++)
    {
        if(i%2 == 0)
        {
            glTranslatef(0, 0, 40);
        }
        else
        {
            glTranslatef(0, 0, 20);
            shelter();
        }
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-30, 0, 1040 );
    for( int i=0 ; i<14; i++)
    {
        glTranslatef(70, 0, 0);
        shelter();
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5,0, -50);
    for(int i = 0; i<12; i++)
    {
        glTranslatef(1.0,0, -80);
        glPushMatrix();
        glRotatef(90, 0, 1, 0);
        glPushMatrix();
        glTranslatef(-100,-0.5, 18);
        glPopMatrix();
        houses(i% 4 + 8);


        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    for(int i = 13; i>0; i--)
    {
        glTranslatef(0,0, -70);
        glPushMatrix();

        glRotatef(-90, 0, 1, 0);
        houses(i% 4 + 8);

        glPopMatrix();
    }
    glPopMatrix();
}

//m = -1000, float m , float n, float width
void umbrella()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    glPushMatrix();
    gluQuadricTexture(quadratic, 1);
    glRotatef(90, -1, 0, 0);
    gluCylinder(quadratic, 0.2, 0.2, 6, 4, 4);

    glTranslatef(0, 0, 5);
    Material(0.5, 0.5, 0.5, 0);
    glutSolidCone(5, 2, 4, 4);
    glPopMatrix();
    Material(0,0,0, 0);
}

void seats()
{
    //seat plate
    box(-5, 5, 1.5, 1.7, 2, 4, 0);
    box(-5, 5, 1.5, 3.5, 1.7, 2, 0);
    box(-5, 5, -1, 1.5, 2, 2.2, 0);
    box(-5, 5, -1, 1.5, 3.5, 3.7, 0);
    umbrella();
}


void shelter()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    glPushMatrix();
    glScalef(1.3, 1.3, 1.3);
    glBindTexture(GL_TEXTURE_2D, ID[10]);
    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(quadratic, 1);
    Material(1, 1, 1, 0);
    glRotatef(90, -1, 0, 0);
    glPushMatrix();
    glTranslatef(0.7, 0.7, 0);
    glRotatef(45, 0, 0, -1);
    for(int i = 0; i<4; i++)
    {
        glRotatef(90, 0, 0, -1);
        glTranslatef(-3.8, -3.8, 0);
        gluCylinder(quadratic, 0.2, 0.2, 4, 4, 4);
        glutSolidCone(0.7, 0.7, 6, 6);
    }
    glPopMatrix();
    Material(0.5, 0.5, 0.5, 0);
    glScalef(1, 1, 0.3);
    glPushMatrix();
    glScalef(1, 1, 0.7);
    glTranslatef(3.7, 2.5, -6);
    glRotatef(90, 0, -1, 0);
    WaterPot();


    glPopMatrix();
    glTranslatef(3.7, 2.5, 19);
    glRotatef(90, 0, 1, 0);
    WaterPot();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void lake()
{
    //lake box
    glBindTexture(GL_TEXTURE_2D, ID[7]);
    glEnable(GL_TEXTURE_2D);
    Material(1, 1, 1, 0);

    x_col = 1;
    y_col =1;
    z_col =1;
    box(-1000, -55, -0.5, -0.49, -70, -20);
    //in the bottommost field
    box(-980, 940, -0.5, -0.49, 1055, 1965, 1, 100);

    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, ID[9]);
    glEnable(GL_TEXTURE_2D);

    Material(1, 1, 1);
    box(-1002, -1000, -0.5, 0.5, -70, -20, 1);
    box(-57, -55, -0.5, 0.5, -50, -20, 1);

    box(-1000, -55, -0.5, 0.5, -72, -70, 1);
    box(-1000, -55, -0.5, 0.5, -20, -18, 1);
    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, ID[3]);
    glEnable(GL_TEXTURE_2D);
    //Material(0, 0, 0);
    //bottom leftmost vertical
    box(-1021, -1020, -0.5, 5, 1020, 2000);
    //bottom rightmost vertical
    box(-991, -989, -0.5, 1, 1020, 1950);
    box(50, 51, -0.5, 2, 20, 970);
    //base left horizontal
    box(-984, -15, -0.5, 1, 13, 15);
    box(-50, 970, -0.5, 1, 1025, 1027);


    Material(1, 0, 0, 0);
    for(int i = 1050; i <2000; i+=60)
    {
        glPushMatrix();

        glTranslatef(-990, 0, i-20);
        glRotatef(90, 0, 1, 0);
        seats();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(i-2030, -0.5, 16);
        seats();
        glPopMatrix();

        glPushMatrix();

        glTranslatef(15, -0.5, i-1010);
        glRotatef(90, 0, 1, 0);
        seats();

        glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);


}


void surrounding()
{
    //Material(1, 1, 1);
    SkyGround();
    HouseSets();
    Gates();
    Market();
    lake();
    forest();
    tunnel();
    riverShift();
    //river();

}

void crossMarks()
{
    pole();
    glBindTexture(GL_TEXTURE_2D, ID[16]);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    //glTranslatef(0, 0.1, 20);
    x_col =0.7;
    y_col =0.7;
    z_col =0.7;

    box(-10, 10, -0.2, 0.05, -10, 10, 0);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void start_game()
{
    Ran= 1;
    turn =0;
    x = -5;
    y = 4.5;
    z = 30;
    lx = 0.0;
    ly = 0.0;
    lz = -1.0;
    angle =0.0;
    mouseX = 0.0;
    mouseY =0.0;
    yAngle = 0;
    roll =0;
    dist = 0;
    fuel = 200;
    Collition = 0;
    temp = 10000;
    win_Race = 0;
    Lose_Race = 0;
    SunAngle = 0;
    g = false;
    Stop = false ;
    silent = false;
    sunR = 0;
    dx= 0.5;
    Sl = false;
    silent = false;

}


char* fuelMessage(int score)
{
    char h[6] = {'0', '0', '0', '0', '0', ' '};//r = new char[8];
    sprintf(h, "%d", score);

    char buffer[6];
    for (int i=0; i<6; i++)
        buffer[i] = h[i];

    if( score / 10 == 0)buffer[1] = ' ';
    else if(score / 100 == 0)buffer[2] = ' ';
    else if(score / 1000 == 0)buffer[3] = ' ';
    else if(score / 10000 == 0)buffer[4] = ' ';



    return buffer;
}

void drawMessage(int x, int z)
{
    glBindTexture(GL_TEXTURE_2D, ID[3]);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    Material(1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(1,1);
    glVertex3f(x-1.1, y-1.7, z-8);
    glTexCoord2f(0,1);
    glVertex3f(x-1.1, y-2.2, z-8);
    glTexCoord2f(0,0);
    glVertex3f(x+1.1, y-2.2, z-8);

    glTexCoord2f(1,0);
    glVertex3f(x+1.1, y-1.7, z-8);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Fuel Message

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);

    if(game_state == start)
    {
        if(g == true)
        {
            displayT( "ORTHOGRAPHIC-VIEW ", x + 0.4, y + 2, z-2 );
        }
        else
        {
            if(SunAngle<180)
            {
                displayT("DAY-MODE ", x+0.4, y+0.4, z);
            }
            else
            {
                displayT("NIGHT-MODE ", x+0.4, y+0.4, z);
                if(Sl == false)
                {
                    displayT("PRESS-KEY ", x+0.3, y+0.6, z);
                    displayT("'h' ", x+1.1, y+0.6, z);
                }
            }
            if (steerSpeed == true)
            {
                displayT( "MAX-SPEED ", x + 0.4, y + 1, z-2 );
            }
            else if (lol == false)
            {
                displayT( "PRESS-UP-ARROW-FOR-SPEED-UP ", x + 0.4, y + 1, z-2 );
            }

            if(Stop == true)
            {
                displayT( "AUTO-FORWARDING-DISABLED ", x + 0.4, y + 1.2, z-1.7 );
            }
            if(Sl == true)
            {
                displayT( "HEADLIGHT-ON ", x + 0.4, y + 2.4, z-1.5 );
            }
            if(silent == true)
            {
                displayT( "SILENT-MODE-ON ", x + 0.4, y + 1.6, z-1.2 );
            }
            if(MouseC == true)
            {
                displayT( "MOUSE-CONTROL-ENABLED ", x + 0.4, y + 1.7, z-0.8 );
            }
            displayT("Fuel: ",x, y, z);
            glTranslatef(0.3, 0, 0);
            displayT(fuelMessage(fuel), x, y, z);

            //Score Message
            glTranslatef(0.6, 0, 0);
            if(dist < 99999)
            {
                displayT("Score: ",x, y, z);
                glTranslatef(0.6, 0, 0);
                displayT(fuelMessage(dist), x, y, z);
            }
            else
                displayT("MAX_SCORE ", x, y, z);

            //Remaining moves
            glTranslatef(-1.3, -0.2, 0);
            displayT("Moves ",x, y, z);
            glTranslatef(0.5, 0, 0);
            displayT("Left: ",x, y, z);
            glTranslatef(0.6, 0, 0);
            displayT(fuelMessage(temp), x, y, z);
        }
    }
    glPopMatrix();


    glPopMatrix();

}
void changeSize(int w, int h);

void drawmap()
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    //glClear(GL_DEPTH_BUFFER_BIT);
    //start_game();
    glViewport(0,0, 1600, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-200.0, 200.0, -3.0, +90.0, -200.0, 200.0);
    gluLookAt(x, 100, z, x, 0.0, z, 0.0, 0.0, -2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glCallList(object);

    //glViewport(1500, WINDOW_HEIGHT-100, WINDOW_WIDTH, WINDOW_HEIGHT);
    glBindTexture(GL_TEXTURE_2D, ID[2]);
    glEnable(GL_TEXTURE_2D);
    Material(1, 1, 1);
    glLineWidth(5);
    glBegin(GL_QUADS);
    glTexCoord2f(1,1);
    glVertex2f(x-5, y-1.5);
    glTexCoord2f(0,1);
    glVertex2f(x-5, y+1.5);
    glTexCoord2f(0,0);
    glVertex2f(x+5, y+1.5);
    glTexCoord2f(1,0);
    glVertex2f(x+5, y-1.5);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
}
void WinScreen(int l)
{
    glDisable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(x, y, z,
              x, y, -z,
              roll + 0.0f, 10.0f, 0.0f);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    if(Lose_Race == 0 && win_Race == 0)
        start_game();
    else
    {
        x = -5;
        y = 4.5;
        z = 30;
        lx = 0.0;
        ly = 0.0;
        lz = -1.0;
        angle =0.0;
        mouseX = 0.0;
        mouseY =0.0;
        yAngle = 0;
    }

    //title trame
    if(l == 14 && over == false)
    {
        //glDisable(GL_LIGHTING);
        float j;

        if(game_state == start)
            j = y+3.5;
        else if(game_state == menu)
            j = y;
        else if(game_state == EXIT)
            j = y-3.5;

        glLineWidth(5);
        glBegin(GL_LINE_LOOP);
        glTexCoord2f(1,0);
        glVertex2f(x-5, j-1.5);
        glTexCoord2f(1,1);
        glVertex2f(x-5, j+1.5);
        glTexCoord2f(0,1);
        glVertex2f(x+5.7, j+1.5);
        glTexCoord2f(0,0);
        glVertex2f(x+5.7, j-1.5);
        glEnd();
        //glEnable(GL_LIGHTING);
    }
    else if(l == 12 || l == 13)
    {
        glBindTexture(GL_TEXTURE_2D, ID[24]);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glTexCoord2f(1,0);
        glVertex2f(x-4, y+8);
        glTexCoord2f(1,1);
        glVertex2f(x-4, y+10);
        glTexCoord2f(0,1);
        glVertex2f(x+4, y+10);
        glTexCoord2f(0,0);
        glVertex2f(x+4, y+8);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        displayT(fuelMessage(dist), x+0.5, y+4.05, z);
    }

    glBindTexture(GL_TEXTURE_2D, ID[l]);
    glEnable(GL_TEXTURE_2D);
    Material(1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(x-12, y-10);
    glTexCoord2f(0,1);
    glVertex2f(x-12, y+10);
    glTexCoord2f(1,1);
    glVertex2f(x+12, y+10);
    glTexCoord2f(1,0);
    glVertex2f(x+12, y-10);
    glEnd();


    glDisable(GL_TEXTURE_2D);

    glutPostRedisplay();
    glEnable(GL_LIGHTING);
}

void Game()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(x, y, z,
              x + lx, y + ly, z + lz,
              roll + 0.0f, 10.0f, 0.0f);

    crossMarks();
    glPushMatrix();
    floorT(1000);
    road();
    glPopMatrix();
    surrounding();

    if(dist > 1000 && x > -15 && x <15 && z>0 && z<30 && game_state == start)
    {
        if(silent == false) PlaySound("D:\\Sounds\\Cheers.wav", NULL, SND_ASYNC|SND_FILENAME);
        win_Race = 1;
    }

    else if(Collition == 1 || fuel <= 0 || temp <=0)
    {
        if(Collition == 1)
        {
            if(silent == false) PlaySound("D:\\Sounds\\Crash.wav", NULL, SND_ASYNC|SND_FILENAME);
            printf("\nYOU COLLIDED\n");
        }

        else if(fuel <= 0)
        {
            if(silent == false) PlaySound("D:\\Sounds\\lose.wav", NULL, SND_ASYNC|SND_FILENAME);
            printf("\nNO FUEL\n");
        }
        else if(temp <= 0)
        {
            if(silent == false) PlaySound("D:\\Sounds\\give_up.wav", NULL, SND_ASYNC|SND_FILENAME);
            printf("\nMOVEMENT LIMIT EXCEEDED\n");
        }
        Lose_Race = 1;
        //glutPostRedisplay();
    }

    else
    {
        if(fuel < 25 && fuel>2)
            if(silent == false) PlaySound("D:\\Sounds\\critical_charge.wav", NULL, SND_ASYNC|SND_FILENAME);
        glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(-angle*180/M_PI, 0, y, 0);
        glTranslatef(-x, -y, -z);
        if(roll == 0)
        {
            drawMessage(x, z);
        }
        drawCar(x, z, 3, 0);
        if(g == true)
        {
            drawmap();
        }
        else
            changeSize(1600, 1000);
        glLoadIdentity();

        glPopMatrix();

    }


    if(abs(mouseX) > 0.3)
    {
        angle -= (0.004f * mouseX);
        lx = sin(angle);
        lz = -cos(angle);
    }
    if(abs(mouseY) > 0.3)
    {
        if(abs(yAngle) < (M_PI/2))
        {
            yAngle += (0.002f * mouseY);
        }
        ly = sin(yAngle);
    }

    glutPostRedisplay();
}

void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(x, y, z,
              x + lx, y + ly, z + lz,
              roll + 0.0f, 10.0f, 0.0f);


    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    if(silent == true)
    {
        PlaySound("D:\\Sounds\\silence_64kb.wav", NULL, SND_ASYNC|SND_FILENAME);
    }


    if(over == false || Ins == 3 )
    {
        if(g == true)
            changeSize(1600, 1000);
        over = false;
        WinScreen(14);
    }


    if(over == true && game_state == start)
    {
        if(win_Race == 0 && Lose_Race == 0)
        {
            glLoadIdentity();
            glutFullScreen();
            glEnable(GL_LIGHTING);
            Game();

            if(wired)
            {
                glPolygonMode( GL_FRONT, GL_LINE ) ;
                glPolygonMode( GL_BACK, GL_LINE ) ;

            }
            else
            {
                glPolygonMode( GL_FRONT,GL_FILL ) ;
                glPolygonMode( GL_BACK, GL_FILL ) ;
            }
            glDisable(GL_LIGHTING);
        }
        else if(win_Race == 1)
        {
            WinScreen(12);
        }
        else if(Lose_Race == 1)
        {
            WinScreen(13);
        }
    }

    else if(over == true && game_state == menu)
    {
        if(Ins == 1)
            WinScreen(2);
        else if(Ins == 2)
            WinScreen(23);
    }

    if(over == true && game_state == EXIT)
    {
        exit(0);
    }
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    //glGetDoublev( GL_MOD, modelview );

    glutSwapBuffers();
    glFlush();

    //glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy)
{
    float fraction = 0.05f;
    if(over == false)
    {
        if(game_state == EXIT)
        {
            if(key == GLUT_KEY_UP)
                game_state = menu;
            else if(key == GLUT_KEY_DOWN)
                game_state = start;
        }
        else if(game_state == menu)
        {
            if(key == GLUT_KEY_DOWN)
                game_state = EXIT;
            else if(key == GLUT_KEY_UP)
                game_state = start;
        }

        else if(game_state == start)
        {
            if(key == GLUT_KEY_UP)
                game_state = EXIT;
            else if(key == GLUT_KEY_DOWN)
                game_state = menu;
        }
    }
    else if(game_state == start && over == true)
    {
        temp--;

        if(key ==  GLUT_KEY_LEFT)
        {
            angle -=  M_PI/2.0 * fraction;
        }
        else if(key == GLUT_KEY_RIGHT)
        {
            angle += M_PI/2.0 * fraction;
        }
        else if(key == GLUT_KEY_DOWN)
        {
            x -= lx * 4  ;
            z -= lz * 4 ;
            dist--;
            fuel--;
            ang -=15 ;
        }
        else if(key == GLUT_KEY_UP)
        {
            x += lx*6 ;
            z += lz*6 ;
            dist+=2;
            lol = true;
            fuel--;
            ang += 20;
        }
        else
            lol = false;

    }
    else if(over == true  && Ins == 1)
    {
        if(key ==  GLUT_KEY_RIGHT)
        {
            if(silent == false) PlaySound("D:\\Sounds\\gunhandgun.wav", NULL, SND_ASYNC|SND_FILENAME|SND_NOSTOP);
            Ins = 2;
        }
        else if(key ==  GLUT_KEY_LEFT)
        {
            if(silent == false) PlaySound("D:\\Sounds\\gunhandgun.wav", NULL, SND_ASYNC|SND_FILENAME|SND_NOSTOP);
            Ins = 0;
            over = false;
        }
        //printf("\nIns %d\n", Ins);

    }
    else if(over == true  && Ins == 2)
    {
        if(key ==  GLUT_KEY_LEFT)
        {
            if(silent == false) PlaySound("D:\\Sounds\\gunhandgun.wav", NULL, SND_ASYNC|SND_FILENAME|SND_NOSTOP);
            Ins = 1;
        }
    }

}
void keyboard_up(unsigned char key, int x, int y)
{
    if(key == ' ')
        steerSpeed = false;
    if(key == 'g')
    {
        g = false;
        silent = false;
    }
    else if(key == 'Q')
        wired = 0;

    buton_press = true;
}



void processNormalKeys(unsigned char key, int xx, int yy)
{
    float fraction = 0.8f;

    if(key == 13 && over == false)
    {
        if (game_state == start )
        {
            PlaySound("D:\\Sounds\\button-click-sound-effect.wav", NULL, SND_ASYNC|SND_FILENAME);
            over = true;
            Lose_Race = 0;
            win_Race = 0;
            printf("\nover %d ",over);
        }
        else if ( game_state == menu)
        {
            PlaySound("D:\\Sounds\\button-click-sound-effect.wav", NULL, SND_ASYNC|SND_FILENAME);
            over = true;
            Ins = 1;
            //printf(" Right BUTTON ");
        }
        else if (game_state == EXIT)
        {
            exit(0);
        }
        // what we are going to do when left button clicked over this button
    }

    if(Lose_Race == 1 || win_Race == 1 )
    {
        buton_press = false;
        over = false;
        Lose_Race = 0;
        win_Race = 0;
    }

    else if(game_state == start)
    {
        if (key == ' ')
        {
            buton_press = false;
            steerSpeed = true;
            x += lx*8 ;
            z += lz*8 ;
            dist+=3;
            temp--;
            fuel -= 2;
            ang += 35;
        }
        else if(key == 'j')
        {
            buton_press = false;
            if(ly > 0 )
                y += ly * fraction;
            else
                y -= ly * fraction;
        }
        else if(key == 'w')
        {
            buton_press = false;
            if(ly > 0 )
                y -= ly * fraction;
            else
                y += ly * fraction;
        }

        else if (key == 'x')
        {
            buton_press = false;
            roll += 0.5f;
        }
        else if(key == 'q')
            shcpt=!shcpt;
        else if(key == 'Q')
            wired = 1;

        else if (key == 'z')
        {
            buton_press = false;
            roll -= 0.5f;
        }
        else if (key == 'r')
        {
            Stop = !Stop ;
            if(Stop == true)
            {
                silent = true;
            }
            else
                silent = false;
        }
        if (key == '+')
        {
            dx += 0.1;
        }
        else if (key == '-')
        {
            if(dx>0.1)
                dx -= 0.1;
        }
        else if (key == 'A')
        {
            amb = !amb;
        }
        else if (key == 'D')
        {
            dif = !dif;
        }
        else if (key == 'S')
        {
            spec = !spec;
        }
        else if (key == 'h')
        {
            if(silent == false) PlaySound("D:\\Sounds\\headlight.wav", NULL, SND_ASYNC|SND_FILENAME);
            temp--;
            Sl = !Sl;
        }
        else if (key == 'g')
        {
            if(silent == false) PlaySound("D:\\Sounds\\headlight.wav", NULL, SND_ASYNC|SND_FILENAME);
            g = true;
            silent = true;
        }
        else if (key == 27)
        {
            if(silent == false) PlaySound("D:\\Sounds\\esc.wav", NULL, SND_ASYNC|SND_FILENAME);
            over = false;
            Ins = 0;
        }
    }

    if (key == 's')
    {
        silent =  !silent ;
    }

    if (key == 27 && game_state == menu)
    {
        exit(0);
    }
    if(silent == false && buton_press == true) PlaySound("D:\\Sounds\\gunhandgun.wav", NULL, SND_ASYNC|SND_FILENAME);
}

// Handles the events triggered when the mouse is moved in the window area.
// Handles yaw and pitch of the camera.
// @param xx : x coordinate of mouse position
// @param yy : y coordinate of mouse position
void processMouseMovement(int xx, int yy)
{
    if(over == false)
    {
        pick(xx, yy);
    }

    //printf("%d",over);
    mouseX = (float)(halfWidth - xx)/halfWidth;
    mouseY = (float)(halfHeight - yy)/halfHeight;

    if(MouseC == true)
    {
        angle -= (0.1f * mouseX);
        if(abs(yAngle) < (M_PI/2))
        {
            yAngle += (0.01f * mouseY);
        }
    }
    else
    {
        yAngle = 0;
        mouseX = 0;
        mouseY = 0;
    }
    //angle -= (0.01f * mouseX);
    lx = sin(angle);
    lz = -cos(angle);

    ly = sin(yAngle-0.1);
}

// Adjusts the viewport sie when the window size is changed and sets the projection.
// @param w : new width of window
// @param h : new height of window
void changeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;
    float ratio = w * 1.0 / h;
    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    // Reset Matrix
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    halfWidth = (float)(w/2.0);
    halfHeight = (float)(h/2.0);
    // Set the correct perspective.
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);

}

void animate ()
{
    if(game_state == start && over == true && g == false && Lose_Race == 0 && win_Race == 0)
    {
        if(Stop == true)
        {
            if(dist>0)
                dist--;
        }
        else
        {
            if(silent == false ) PlaySound("D:\\Sounds\\son_993.wav", NULL, SND_ASYNC|SND_FILENAME|SND_NOSTOP|SND_LOOP);
            rotationAngle = (int)(rotationAngle+ang) % 360;
            ang+=3;
            x += lx*2 ;
            z += lz*2 ;
            dist++;
            temp--;
            fuel--;
            ang += 20;
        }
        glutPostRedisplay();

        // next Timer call milliseconds later
    }
    else
    {
        PlaySound("D:\\Sounds\\background.wav", NULL, SND_ASYNC|SND_FILENAME|SND_NOSTOP|SND_LOOP);
    }

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON )
    {
        if (over == false && game_state == start )
        {
            if(silent == false) PlaySound("D:\\Sounds\\button-click-sound-effect.wav", NULL, SND_ASYNC|SND_FILENAME);
            over = true;
            Lose_Race = 0;
            win_Race = 0;
            printf("\nover %d ",over);
        }
        else if (over == false  && game_state == menu)
        {
            if(silent == false) PlaySound("D:\\Sounds\\button-click-sound-effect.wav", NULL, SND_ASYNC|SND_FILENAME);
            over = true;
            Ins = 1;
            //printf(" Right BUTTON ");
        }
        else if (over == false && game_state == EXIT)
        {
            exit(0);
        }
        // what we are going to do when left button clicked over this button
    }
    else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    {
        if(fl!=1)
        {
            fl=1;
            clkpt[0].x1=x;
            clkpt[0].y1=y;
        }

        MouseC = !MouseC;
        scsToWcs(clkpt[0].x1,clkpt[0].y1,wcsClkDn);
        cout<<"\nD: "<<x<<" "<<y<<" wcs: "<<wcsClkDn[0]<<" "<<wcsClkDn[1]<<" "<<wcsClkDn[2];
    }
    else if(button==GLUT_RIGHT_BUTTON && state==GLUT_UP)
    {
        if (fl==1)
        {
            clkpt[1].x1=x;
            clkpt[1].y1=y;
            fl=0;
        }
        scsToWcs(clkpt[1].x1,clkpt[1].y1,wcsClkUp);
        cout<<"\nU: "<<x<<" "<<y<<" wcs: "<<wcsClkUp[0]<<" "<<wcsClkUp[1];

        clikd=!clikd;
    }
}

void scsToWcs(float sx,float sy, float wcsv[3] )
{

    GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
    GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

    glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
    glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info

    winX = sx;
    winY = (float)viewport[3] - (float)sy;
    winZ = 0;

    //get the world coordinates from the screen coordinates
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
    wcsv[0]=worldX;
    wcsv[1]=worldY;
    wcsv[2]=worldZ;


}

void LoadTexture(const char*filename, int index)
{
    glGenTextures(1, &ID[index]);
    glBindTexture(GL_TEXTURE_2D, ID[index]);
    printf("%d\n",ID[index]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData);
}


int main(int argc, char **argv)
{
    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    glutCreateWindow("1607028_Project-3D RACE");


    glShadeModel(GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);


    LoadTexture("D:\\images\\road3.bmp",1); //floor
    LoadTexture("D:\\images\\key.bmp",2);
    LoadTexture("D:\\images\\glass.bmp",3);
    LoadTexture("D:\\images\\wheel2.bmp",4);
    LoadTexture("D:\\images\\grass2.bmp",5);
    LoadTexture("D:\\images\\sky.bmp",6);
    LoadTexture("D:\\images\\Water.bmp",7);

    LoadTexture("D:\\images\\road1.bmp",8);
    LoadTexture("D:\\images\\download.bmp",9);
    LoadTexture("D:\\images\\metal.bmp",10);
    LoadTexture("D:\\images\\car1.bmp",11);


    LoadTexture("D:\\images\\win.bmp",12);
    LoadTexture("D:\\images\\lose3.bmp",13);

    LoadTexture("D:\\images\\Title.bmp",14);

    LoadTexture("D:\\images\\bark2.bmp",15);
    LoadTexture("D:\\images\\checker2.bmp",16);

    LoadTexture("D:\\images\\shop.bmp",17);

    LoadTexture("D:\\images\\shop2.bmp",18);
    LoadTexture("D:\\images\\shop3.bmp",19);
    LoadTexture("D:\\images\\coffee.bmp",20);

    LoadTexture("D:\\images\\cone.bmp",21);

    LoadTexture("D:\\images\\plane.bmp",22);
    LoadTexture("D:\\images\\instruction.bmp",23);
    LoadTexture("D:\\images\\score2.bmp",24);
    //LoadTexture("D:\\images\\Nsky2.bmp",25);


    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutMouseFunc(mouse);
    glutIdleFunc(animate);
    glutKeyboardFunc(processNormalKeys);
    glutKeyboardUpFunc(keyboard_up);
    glutSpecialFunc(processSpecialKeys);
    glutPassiveMotionFunc(processMouseMovement);

    glutMainLoop();

    return 1;
}
