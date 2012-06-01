// A particle 
typedef struct particleData
{
	float   mass;
	float   position[3];
	float   speed[3];
	float   color[3];
}
particleData;



// A piece of debris
typedef struct debrisData
{
	float   mass;
	float   position[3];
	float   speed[3];
	float   orientation[3];        // Rotation angles around x, y, and z axes
	float   orientationSpeed[3];
	float   color[3];
	float   scale[3];
}
debrisData;


#define NUM_PARTICLES           500          //Number of particles  
#define NUM_DEBRIS              300           //Number of debris     

#define M_PI                    3.1415926535
#define FUEL_MAX                1000



class explosion
{
 private:

     particleData     particles[NUM_PARTICLES];
     debrisData       debris[NUM_DEBRIS];   
     int              fuel;
	 int              DebrisListID;
	 bool             Normalize;
	 float            rayon, color;
	 float            frictionCoeff;
	 float            GroundPos;
	 float            SpeedFactor;
     float            OrientationSpeedFactor;

	 void newSpeed(float dest[3], float Vinit[3]);
     void newOrientationSpeed (float dest[3]);
     void newParticule(const int& i, float pos[3], float Vinit[3]);
     void newDebris(const int& i, float pos[3], float Vinit[3]);

 public:

	 explosion();
	 ~explosion();

     void newExplosion(float pos[3], float Vinit[3]);
	 void Update(const float& dT, float Gravity[3]);
	 void DrawExplosion(float Spos[3]);
};
