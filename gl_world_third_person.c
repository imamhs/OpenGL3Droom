/* OpenGL room
 * Copyright (C) 2013
 * Md Imam Hossain (emamhd at gmail dot com)
 * All rights reserved
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include <SDL.h>
#include <GL/gl.h>

/* Program Control */
int shut = 0;

/* GL Lighting */
GLfloat ambientLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat diffuseLight1[] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat plat[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat lightPos[] = { 0.0f, 10.0f, 30.0f, 1.0f };
GLfloat lightPos1[] = { 0.0f, 20.0f, 20.0f, 1.0f };

/* World Camera */
GLfloat beta = 0.0f;
GLfloat alpha = 0.0f;
GLfloat x = 0.0f, y = 0.0f, z = 20.0f;
GLfloat zoom = -10.0f;

/* Moving */
int move[8] = {0, 0, 0, 0, 0, 0, 0, 0};
GLfloat pi = 3.1416;
GLfloat speed = 0.5f;
GLfloat ani = 0.0f;
GLfloat langle = 0.0f;
GLfloat rangle = 0.0f;

/* SDL speed regulator */
Uint32 fps_reg = 0;
Uint32 ani_reg = 0;

/* SDL variables */
SDL_Surface *back_surface;
SDL_Event event;

/* SDL Mouse position */
int mposx = 0, mposy = 0;

void Draw_Cube (GLfloat size, GLfloat red, GLfloat green, GLfloat blue);
void Draw_RectPrism (GLfloat length, GLfloat width, GLfloat thickness, GLfloat red, GLfloat green, GLfloat blue);
void Setup_RC (void);
void Render_Scene (void);
void Render_Physics (void);
void Get_Input (void);

int main (int argc, char *argv[])
{
    
printf ("Initializing SDL\n");
if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) == -1) {
    fprintf (stderr, "Could not initialize SDL %s\n", SDL_GetError());
    exit (1);
}
printf ("Success\n");  
    
SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
SDL_WM_SetCaption ("OpenGL 3D room", NULL);
SDL_WM_GrabInput (SDL_GRAB_ON);
SDL_ShowCursor (SDL_DISABLE);

back_surface = SDL_SetVideoMode (640, 480, 32, SDL_OPENGL);
    
if (back_surface == NULL) {
    fprintf (stderr, "Could not set video mode %s\n", SDL_GetError());
    SDL_Quit ();
    exit (1);
}

Setup_RC ();
    
while (shut == 0) {
    if ((SDL_GetTicks() - fps_reg) > 20) {
        Get_Input ();
        Render_Physics ();
        fps_reg = SDL_GetTicks ();
    }
    if ((SDL_GetTicks() - ani_reg) > 10) {
        if (ani > 359) ani = 0.0f;
        else ani += 1.0f;
        ani_reg = SDL_GetTicks ();
    }
Render_Scene ();
}
    
SDL_Quit();
  
return 0;

}

void Draw_Cube (GLfloat size, GLfloat red, GLfloat green, GLfloat blue)
{

glBegin(GL_TRIANGLES);
  
    glColor3f (red, green, blue);

    glVertex3f(-size, -size, size);
    glVertex3f(size, -size, size);
    glVertex3f(size, size, size);

    glVertex3f(size, size, size);
    glVertex3f(-size, size, size);
    glVertex3f(-size, -size, size);

    glVertex3f(size, size, -size);
    glVertex3f(size, size, size);
    glVertex3f(size, -size, size);

    glVertex3f(size, -size, size);
    glVertex3f(size, -size, -size);
    glVertex3f(size, size, -size);

    glVertex3f(-size, -size, -size);
    glVertex3f(-size, size, -size);
    glVertex3f(size, size, -size);

    glVertex3f(size, size, -size);
    glVertex3f(size, -size, -size);
    glVertex3f(-size, -size, -size);

    glVertex3f(-size, -size, size);
    glVertex3f(-size, size, size);
    glVertex3f(-size, size, -size);

    glVertex3f(-size, size, -size);
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, -size, size);
  
    glVertex3f(-size, size, size);
    glVertex3f(size, size, size);
    glVertex3f(size, size, -size);

    glVertex3f(size, size, -size);
    glVertex3f(-size, size, -size);
    glVertex3f(-size, size, size);
  
    glVertex3f(size, -size, size);
    glVertex3f(-size, -size, size);
    glVertex3f(-size, -size, -size);

    glVertex3f(-size, -size, -size);
    glVertex3f(size, -size, -size);
    glVertex3f(size, -size, size);
  
glEnd();

}

void Draw_RectPrism (GLfloat length, GLfloat width, GLfloat thickness, GLfloat red, GLfloat green, GLfloat blue)
{
    
glBegin(GL_TRIANGLES);

    color[0] = red;
    color[1] = green;
    color[2] = blue;
    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glMaterialfv (GL_FRONT, GL_SPECULAR, specref);

    glNormal3f (0.0f, 0.0f, 1.0f);
    glVertex3f(-length, -thickness, width);
    glVertex3f(length, -thickness, width);
    glVertex3f(length, thickness, width);

    glNormal3f (0.0f, 0.0f, 1.0f);
    glVertex3f(length, thickness, width);
    glVertex3f(-length, thickness, width);
    glVertex3f(-length, -thickness, width);
  
    color[0] = green;
    color[1] = red;
    color[2] = blue;
    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  
    glNormal3f (1.0f, 0.0f, 0.0f);
    glVertex3f(length, thickness, -width);
    glVertex3f(length, thickness, width);
    glVertex3f(length, -thickness, width);

    glNormal3f (1.0f, 0.0f, 0.0f);
    glVertex3f(length, -thickness, width);
    glVertex3f(length, -thickness, -width);
    glVertex3f(length, thickness, -width);
  
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glMaterialfv (GL_FRONT, GL_SPECULAR, specref);
  
    glNormal3f (0.0f, 0.0f, -1.0f);
    glVertex3f(-length, -thickness, -width);
    glVertex3f(-length, thickness, -width);
    glVertex3f(length, thickness, -width);

    glNormal3f (0.0f, 0.0f, -1.0f);
    glVertex3f(length, thickness, -width);
    glVertex3f(length, -thickness, -width);
    glVertex3f(-length, -thickness, -width);

    color[0] = green;
    color[1] = red;
    color[2] = blue;
    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

    glNormal3f (-1.0f, 0.0f, 0.0f);
    glVertex3f(-length, -thickness, width);
    glVertex3f(-length, thickness, width);
    glVertex3f(-length, thickness, -width);

    glNormal3f (-1.0f, 0.0f, 0.0f);
    glVertex3f(-length, thickness, -width);
    glVertex3f(-length, -thickness, -width);
    glVertex3f(-length, -thickness, width);

    color[0] = green;
    color[1] = blue;
    color[2] = red;
    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

    glNormal3f (0.0f, 1.0f, 0.0f);
    glVertex3f(-length, thickness, width);
    glVertex3f(length, thickness, width);
    glVertex3f(length, thickness, -width);

    glNormal3f (0.0f, 1.0f, 0.0f);
    glVertex3f(length, thickness, -width);
    glVertex3f(-length, thickness, -width);
    glVertex3f(-length, thickness, width);
  
    color[0] = green;
    color[1] = blue;
    color[2] = red;
    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  
    glNormal3f (0.0f, -1.0f, 0.0f);
    glVertex3f(length, -thickness, width);
    glVertex3f(-length, -thickness, width);
    glVertex3f(-length, -thickness, -width);

    glNormal3f (0.0f, -1.0f, 0.0f);
    glVertex3f(-length, -thickness, -width);
    glVertex3f(length, -thickness, -width);
    glVertex3f(length, -thickness, width);
    
glEnd();

}

void Setup_RC (void)
{

glEnable (GL_DEPTH_TEST);

glFrontFace (GL_CCW);

glEnable (GL_CULL_FACE);

glShadeModel (GL_SMOOTH);

glEnable (GL_LIGHTING);

glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambientLight);
glLightModeli (GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuseLight);
glLightfv (GL_LIGHT0, GL_SPECULAR, specular);
glEnable (GL_LIGHT0);

glLightfv (GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
glEnable (GL_LIGHT1);

glMaterialf (GL_FRONT, GL_SHININESS, 128.0);

glClearColor (0.0f, 0.0f, 0.2f, 1.0f);

glViewport (0, 0, 640, 480);

glMatrixMode (GL_PROJECTION);
glLoadIdentity ();

glFrustum (-1.0, 1.0, -1.0, 1.0, 1.0, 200.0);

glMatrixMode (GL_MODELVIEW);

}

void Render_Scene (void)
{

glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

glLoadIdentity ();

/* Define World */

glTranslatef (0.0f, 0.0f, zoom);

glRotatef (alpha, 1.0f, 0.0f, 0.0f);
glRotatef (beta, 0.0f, 1.0f, 0.0f);
glTranslatef (x, y, -z);

/* Put Light */

glPushMatrix ();

glLightfv (GL_LIGHT0, GL_POSITION, lightPos);
glTranslatef (lightPos[0],lightPos[1],lightPos[2]);
Draw_RectPrism (0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f);

glPopMatrix ();
glPushMatrix ();

glRotatef (ani, 0.0f, 1.0f, 0.0f);
glLightfv (GL_LIGHT1, GL_POSITION, lightPos1);
glTranslatef (lightPos1[0], lightPos1[1], lightPos1[2]);
Draw_RectPrism (0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f);

/* Put Robo */

glPopMatrix ();
glPushMatrix ();

glTranslatef (-x, y, z);
glRotatef (-beta, 0.0f, 1.0f, 0.0f);

glTranslatef (0.0f, 0.9f, 0.0f);

Draw_RectPrism (0.5f, 0.5f, 1.2f, 0.0f, 0.0f, 1.0f);

glPushMatrix ();

glRotatef (langle, 1.0f, 0.0f, 0.0f);
glTranslatef (-0.4f, -2.0f, 0.0f);

Draw_RectPrism (0.2f, 0.2f, 0.8f, 1.0f, 0.0f, 1.0f);

glPopMatrix ();

glRotatef (rangle, 1.0f, 0.0f, 0.0f);
glTranslatef (0.4f, -2.0f, 0.0f);

Draw_RectPrism (0.2f, 0.2f, 0.8f, 1.0f, 0.0f, 1.0f);

/* Put Objects */

glPopMatrix ();
glPushMatrix ();

glTranslatef (0.0f, -2.0f, 0.0f);

glBegin(GL_QUADS);
    glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, plat);
    glNormal3f (0.0f, 1.0f, 0.0f);
	glVertex3f (-100.0f, 0.0f, -100.0f);
	glVertex3f (-100.0f, 0.0f,  100.0f);
	glVertex3f ( 100.0f, 0.0f,  100.0f);
    glVertex3f ( 100.0f, 0.0f, -100.0f);
glEnd();

glPopMatrix ();
glPushMatrix ();

glTranslatef (0.0f, 2.0f, 0.0f);

Draw_RectPrism (5.0f, 8.0f, 4.0f, 1.0f, 0.2f, 0.0f);

glPopMatrix ();
glPushMatrix ();

glTranslatef (20.0f, 8.0f, 0.0f);

Draw_RectPrism (5.0f, 8.0f, 10.0f, 0.5f, 0.0f, 0.5f);

glPopMatrix ();
glPushMatrix ();

glTranslatef (-30.0f, 3.0f, 0.0f);
glRotatef (45, 0.0f, 1.0f, 0.0f);

Draw_RectPrism (5.0f, 8.0f, 5.0f, 1.0f, 0.0f, 1.0f);

glPopMatrix ();
glPushMatrix ();

glTranslatef (0.0f, 20.0f, 0.0f);
glRotatef (ani, 0.0f, 1.0f, 0.0f);

Draw_RectPrism (4.0f, 4.0f, 4.0f, 1.0f, 0.5f, 0.5f);

glPopMatrix ();

SDL_GL_SwapBuffers ();

}

void Render_Physics (void)
{
if (move[0] == 1) {
    if (beta < 1) beta = 359.0f;
    else beta -= 1.0f;
}
if (move[1] == 1) {
    if (beta > 359) beta = 1.0f;
    else beta += 1.0f;
}
if (move[2] == 1) {
    if (alpha < 1) alpha = 359.0f;
    else alpha -= 1.0f;
}
if (move[3] == 1) {
    if (alpha > 359) alpha = 1.0f;
    else alpha += 1.0f;
}
if (move[4] == 1) {
    z -= sin (beta * (pi/180)) * speed;
    x -= -cos (beta * (pi/180)) * speed;
}
if (move[5] == 1) {
    z += sin (beta * (pi/180)) * speed;
    x += -cos (beta * (pi/180)) * speed;
}
if (move[6] == 1) {
    x += -sin (beta * (pi/180)) * speed;
    z += -cos (beta * (pi/180)) * speed;
    if (langle > 29) langle = -30.0;
    else langle += 5.0f;
    if (rangle < -29) rangle = 30.0;
    else rangle -= 5.0f;
}
if (move[7] == 1) {
    x -= -sin (beta * (pi/180)) * speed;
    z -= -cos (beta * (pi/180)) * speed;
    if (langle < -29) langle = 30.0;
    else langle -= 5.0f;
    if (rangle > 29) rangle = -30.0;
    else rangle += 5.0f;
}

}

void Get_Input (void)
{
while (SDL_PollEvent (&event)) {
    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    move[0] = 1;
                    break;
                case SDLK_RIGHT:
                    move[1] = 1;
                    break;
                case SDLK_UP:
                    move[2] = 1;
                    break;
                case SDLK_DOWN:
                    move[3] = 1;
                    break;
                case SDLK_a:
                    move[4] = 1;
                    break;
                case SDLK_d:
                    move[5] = 1;
                    break;
                case SDLK_w:
                    move[6] = 1;
                    break;
                case SDLK_s:
                    move[7] = 1;
                    break;
                case SDLK_m:
                    lightPos[2] += 5.0f;
                    break;
                case SDLK_n:
                    lightPos[2] -= 5.0f;
                    break;
                case SDLK_l:
                    SDL_WM_IconifyWindow ();
                    break;
                case SDLK_f:
                    SDL_WM_ToggleFullScreen (back_surface);
                    break;
                case SDLK_ESCAPE:
                    shut = 1;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    move[0] = 0;
                    break;
                case SDLK_RIGHT:
                    move[1] = 0;
                    break;
                case SDLK_UP:
                    move[2] = 0;
                    break;
                case SDLK_DOWN:
                    move[3] = 0;
                    break;
                case SDLK_a:
                    move[4] = 0;
                    break;
                case SDLK_d:
                    move[5] = 0;
                    break;
                case SDLK_w:
                    move[6] = 0;
                    break;
                case SDLK_s:
                    move[7] = 0;
                    break;
                default:
                    break;
            }
            break;
        case SDL_MOUSEMOTION:
            if (event.motion.x > mposx) {
                if (beta > 359) beta = 1.0f;
                else beta += 1.0f;
                mposx = event.motion.x;
            }
            if (event.motion.x < mposx) {
                if (beta < 1) beta = 359.0f;
                else beta -= 1.0f;
                mposx = event.motion.x;
            }
            if (event.motion.y > mposy) {
                if (alpha < 89) alpha += 1.0f;
                mposy = event.motion.y;
            }
            if (event.motion.y < mposy) {
                if (alpha > -2) alpha -= 1.0f;
                else if (zoom < -5) zoom += 0.1;
                mposy = event.motion.y;
            }
            if (event.motion.x > 638 || event.motion.y > 478 || event.motion.x < 1 || event.motion.y < 1) SDL_WarpMouse (640/2, 480/2);
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button) {
                case SDL_BUTTON_WHEELUP:
                    if (zoom < -5) zoom += 1.0;
                    break;
                case SDL_BUTTON_WHEELDOWN:
                    if (zoom > -20) zoom -= 1.0;
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }
}
}
