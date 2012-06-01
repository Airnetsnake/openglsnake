//#include "stdafx.h"

#include <math.h>
#include <windows.h>		
#include <gl/gl.h>
#include <gl/glu.h>	
#include "explosion.h"
#include "GLU.h"



explosion::explosion()
{
	Normalize              = true;
	GroundPos              = 0.1f;
	SpeedFactor            = 800.0f;
	frictionCoeff          = 1.0f;
    OrientationSpeedFactor = 3.0f;
}
	
explosion::~explosion()
{

}

//Gives new random speed
void explosion::newSpeed(float dest[3], float Vinit[3])
{
	float    x;
	float    y;
	float    z;
	
	float r ;
	float angle = 2;
	
	if (Normalize) r = SpeedFactor;
	
	else r = SpeedFactor * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX);
	
	float Tetha = float(angle * M_PI*( ((GLfloat) rand ()) / ((GLfloat) RAND_MAX) - 0.5f));
	float Phi   = float(2*M_PI*( ((GLfloat) rand ()) / ((GLfloat) RAND_MAX) - 0.5f));
	
	if(r < 100.0f) r += 100.0f;
	
	x = r*sin(Tetha)*sin(Phi);
	y = r*cos(Tetha);
	z = r*sin(Tetha)*cos(Phi);
	
	dest[0] = x + 1.5f*Vinit[0];
	dest[1] = y + 1.5f*Vinit[1];
	dest[2] = z + 1.5f*Vinit[2];
}


//Gives new random orientation 
void explosion::newOrientationSpeed(float dest[3])
{
	float x, y, z, len;
	
	x = (2.0f * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0f;
	y = (2.0f * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0f;
	z = (2.0f * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0f;
	
	if (Normalize)
    {
		len = sqrt (x*x + y*y + z*z);
		
		if (len)
		{
			x /= len;
			y /= len;
			z /= len;
		}
    }
	
	dest[0] = x * OrientationSpeedFactor;
	dest[1] = y * OrientationSpeedFactor;
	dest[2] = z * OrientationSpeedFactor;
}



// Creates a new particule
void explosion::newParticule(const int& i, float pos[3], float Vinit[3])
{
	particles[i].mass = 8*( ((GLfloat) rand ()) / ((GLfloat) RAND_MAX) + 1);
	
	particles[i].position[0] = pos[0];
	particles[i].position[1] = pos[1];
	particles[i].position[2] = pos[2];
	
	//explosion colors
	particles[i].color[0] = 1.0;
	particles[i].color[1] = 1.0;
	particles[i].color[2] = 0.0;	
	
	newSpeed(particles[i].speed, Vinit); 
	
	particles[i].speed[0] /= particles[i].mass;
	particles[i].speed[1] /= particles[i].mass;
	particles[i].speed[2] /= particles[i].mass;
}


//Creates a new debris
void explosion::newDebris(const int& i, float pos[3], float Vinit[3])
{
	debris[i].mass = 8.5f*(((GLfloat) rand ()) / ((GLfloat) RAND_MAX) + 1);
	
	debris[i].position[0] = pos[0];
	debris[i].position[1] = pos[1];
	debris[i].position[2] = pos[2];
	
	debris[i].orientation[0] = 0.0f;
	debris[i].orientation[1] = 0.0f;
	debris[i].orientation[2] = 0.0f;
	
	debris[i].color[0] = 0.7f;
	debris[i].color[1] = 0.0f;
	debris[i].color[2] = 0.0f;
	
	debris[i].scale[0] = (10.0f *((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0f;
	debris[i].scale[1] = (10.0f *((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0f;
	debris[i].scale[2] = (10.0f *((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0f;
	
	newSpeed(debris[i].speed, Vinit);
	
	debris[i].speed[0] /= debris[i].mass;
	debris[i].speed[1] /= debris[i].mass;
	debris[i].speed[2] /= debris[i].mass;
	
	newOrientationSpeed(debris[i].orientationSpeed);
}


// Creates a new explosion.
void explosion::newExplosion(float pos[3], float Vinit[3])
{	
	for (int i = 0; i < NUM_PARTICLES; i++)
    {
		newParticule(i, pos, Vinit);
    }
	
	for (int i = 0; i < NUM_DEBRIS; i++)
    {
		newDebris(i, pos, Vinit);
    }
	
	fuel                   = FUEL_MAX;
	rayon                  = 5.0f;
	color                  = 0.5f;
}



//Updates the explosion
void explosion::Update(const float& dT, float Gravity[3])
{
	int i;
 
	if (fuel > 0)
	{
	 for (i = 0; i < NUM_PARTICLES; i++)
	 {
	   particles[i].speed[0] *= frictionCoeff;
	   particles[i].speed[1] -= Gravity[1]*dT;
	   particles[i].speed[2] *= frictionCoeff;
					
	   particles[i].position[0] += particles[i].speed[0] * dT;
	   particles[i].position[1] += particles[i].speed[1] * dT - 0.5f*Gravity[1]*dT*dT;
	   particles[i].position[2] += particles[i].speed[2] * dT;
				
	   particles[i].color[0] -= 0.001f;
	   if (particles[i].color[0] < 0.0)
	   {
	    particles[i].color[0] = 0.0;
	   }
					
	   particles[i].color[1] -= 0.001f;
	   if (particles[i].color[1] < 0.0)
       {
	    particles[i].color[1] = 0.0;
	   }
					
	   particles[i].color[2] -= 0.001f;
	   if (particles[i].color[2] < 0.0)
	   {
	    particles[i].color[2] = 0.0;
	   }
	 }
			
	 for (i = 0; i < NUM_DEBRIS; i++)
	 {
	   debris[i].speed[0] *= frictionCoeff;
	   debris[i].speed[1] -= Gravity[1]*dT;
	   debris[i].speed[2] *= frictionCoeff;
					
	   debris[i].position[0] += debris[i].speed[0] * dT;
	   debris[i].position[1] += debris[i].speed[1] * dT - 0.5f*Gravity[1]*dT*dT;
	   debris[i].position[2] += debris[i].speed[2] * dT;
					
	   debris[i].orientation[0] += debris[i].orientationSpeed[0];
	   debris[i].orientation[1] += debris[i].orientationSpeed[1];
	   debris[i].orientation[2] += debris[i].orientationSpeed[2];  
	 }		
	 --fuel;
   }
}


// Draws the explosion
void explosion::DrawExplosion(float Spos[3])
{
  int i;
    
  //draw particles
  glBegin (GL_POINTS);
  for (i = 0; i < NUM_PARTICLES; i++)
  {
   if(particles[i].position[1] > GroundPos)
   {
    glColor3fv (particles[i].color);
    glVertex3fv (particles[i].position);
   }
  }		
  glEnd ();
 
  //Draw debris
  glNormal3f (0.0, 0.0, 1.0);
		
  for (i = 0; i < NUM_DEBRIS; i++)
  {
	  if(debris[i].position[1] > GroundPos-5)
	  {
       glColor3fv (debris[i].color);			
			
       glPushMatrix ();
			
       glTranslatef (debris[i].position[0], debris[i].position[1], debris[i].position[2]);
			
       glRotatef (debris[i].orientation[0], 1.0, 0.0, 0.0);
       glRotatef (debris[i].orientation[1], 0.0, 1.0, 0.0);
       glRotatef (debris[i].orientation[2], 0.0, 0.0, 1.0);
			
       glScalef (debris[i].scale[0], debris[i].scale[1], debris[i].scale[2]);
			
       glBegin (GL_TRIANGLES);
        glVertex3f (0.0, 0.5, 0.0);
        glVertex3f (-0.25, 0.0, 0.0);
        glVertex3f (0.25, 0.0, 0.0);
       glEnd ();
			
       glPopMatrix();
	  }
  }

  //Draw blended sphere
  if(color) 
  {
   rayon+=0.5f;
   color-=0.01f;
   

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE);
   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);
   glColor4f(color,color,0.0f,0.4f);
   glPushMatrix();
   glTranslatef (Spos[0], Spos[1], Spos[2]);
   //glutSolidSphere(rayon, 20.0f, 20.0f);
   glPopMatrix();
   glEnable(GL_LIGHTING);
   glDisable(GL_BLEND);	
   glEnable(GL_DEPTH_TEST);
  }
  
}
