#ifndef PHASER_H
#define PHASER_H

#include "math.h"
#include "FxBase.h"
#include "ModuleBase.h"

#define BOTTOM_FREQ 100
#define M_PI 3.141592653589793
#define M_PI_2 (M_PI/2.0)
#define PIN(n,min,max) ((n) > (max) ? max : ((n) < (min) ? (min) : (n)))

namespace PHASER
{
enum t
{
	// Parameters Tags
	kRate = 0,
	kWidth,
	kFeedback,
	kStages,
	kWetLevel,

	kNumParams
};
};

class Phaser : public ModuleBase
{
public:
	Phaser();
	~Phaser ();
	
	void resetBuffer();
	void resetCoeffs();
	
	void setParameter(int index, float value);
	void setSampleRate(float sampleRate);
	
	void setStages ();
	void setSweep();
	
	void process(float &in);
	void process(float &inL, float &inR);
	void process(double &in);
	void process(double &inL, double &inR);
	
private:	
	float *parameter_;

	double paramSweepRate;
	double paramWidth;	//(0-100%)
	double paramFeedback; //(0-1)
	double paramStages; //(2-10)
	float wetLevel;

	double _sweepRate;			// actual calc'd sweep rate
	int   _stages;				// calc'd # of stages 2-10

	double _wp;					// freq param for equation
	double  _minwp;
	double  _maxwp;
	double _sweepFactor;		// amount to multiply the freq by with each sample

	// the all pass line
	double _lx1;
	double _ly1;
	double _lx2;
	double _ly2;
	double _lx3;
	double _ly3;
	double _lx4;
	double _ly4;
	double _lx5;
	double _ly5;
	double _lx6;
	double _ly6;
	double _lx7;
	double _ly7;
	double _lx8;
	double _ly8;
	double _lx9;
	double _ly9;
	double _lx10;
	double _ly10;

};

#endif
