#include "Phaser.h"

using namespace PHASER;

Phaser::Phaser ()
{
	name_ = "Phaser";

	parameter_ = new float[kNumParams];

	for(int i = 0; i < kNumParams; ++i)
	{
		parameter_[i] = 0;
	}

	reset();
	// default Program Values
	paramSweepRate = 0.2f;
	paramWidth = 1.0f;
	paramFeedback = 0.0f;
	paramStages = 0.2f;
	wetLevel = 0.5f;
}

Phaser::~Phaser()
{
	//destructor
	if(parameter_)
		delete[] parameter_;
	parameter_ = 0;
}

void Phaser::resetBuffer()
{
}

void Phaser::resetCoeffs()
{
}

void Phaser::setSampleRate(float sampleRate)
{
	sampleRate_ = sampleRate;
	setSweep();
}

void Phaser::setParameter(int index, float value)
{
	switch(index)
	{
	case kRate :
		parameter_[kRate] = value;
		paramSweepRate = parameter_[kRate];
		break;
	
	case kWidth :
		parameter_[kWidth] = value;
		paramWidth = parameter_[kWidth];
		break;

	case kFeedback :
		parameter_[kFeedback] = value;
		paramFeedback = parameter_[kFeedback];
		break;

	case kStages :
		parameter_[kStages] = value;
		paramStages = parameter_[kStages];
		break;

	case kWetLevel :
		parameter_[kWetLevel] = value;
		wetLevel = parameter_[kWetLevel];
		break;
	}
}


void Phaser::setStages ()
{
	_stages = (int)(paramStages * 10.0 + 2);
}

void Phaser::setSweep()
{
	// calc the actual sweep rate
	double rate = pow(10.0,(double)paramSweepRate);
	rate -= 1.0f;
	rate *= 1.1f;
	rate += 0.1f;
	_sweepRate = rate;
	double range = paramWidth * 6.0f;

	// do the rest of the inits here just for fun
	_wp = _minwp = (myPI * BOTTOM_FREQ) / sampleRate_;
	_maxwp = (myPI * BOTTOM_FREQ * range) / sampleRate_;

	// figure out increment to multiply by each time
	_sweepFactor = pow( (double)range, (double)(_sweepRate / (sampleRate_ / 2.0)) );

	// init the all pass line
	_lx1 = _ly1 =
	_lx2 = _ly2 =
	_lx3 = _ly3 =
	_lx4 = _ly4 = 
	_lx5 = _ly5 = 
	_lx6 = _ly6 =
	_lx7 = _ly7 =
	_lx8 = _ly8 =
	_lx9 = _ly9 = 
	_lx10 = _ly10 = 0.0f;
}

void Phaser::process(float &in)
{
	switch( _stages)
	{
	case 2:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			float xn = in;
			double inmix = in  + paramFeedback  * _ly2;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;

			// develop output mix
			in = (1 - wetLevel) * xn + wetLevel * (float)_ly2;

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;

	case 4:
	default:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			float xn = in;
			double inmix = in + paramFeedback * _ly4;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;

			// develop output mix
			in = (float)tanh((1 - wetLevel) * xn + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	case 6:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			float xn = in;
			double inmix = in + paramFeedback * _ly6;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;
			_ly5 = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5 = _ly4;
			_ly6 = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6 = _ly5;

			// develop output mix
			in = (float)tanh((1 - wetLevel) * xn + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;

	case 8:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			float xn = in;
			double inmix = in + paramFeedback * _ly8;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;
			_ly5 = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5 = _ly4;
			_ly6 = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6 = _ly5;
			_ly7 = coef * (_ly7 + _ly6) - _lx7;			// do 7th filter
			_lx7 = _ly6;
			_ly8 = coef * (_ly8 + _ly7) - _lx8;			// do 8th filter 
			_lx8 = _ly7;

			// develop output mix
			in = (float)tanh((1 - wetLevel) * in + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	case 10:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			float xn = in;
			double inmix = in + paramFeedback * _ly10;

			// run thru the all pass filters
			_ly1  = coef * (_ly1 + inmix) - _lx1;			// do 1st filter
			_lx1  = inmix;
			_ly2  = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2  = _ly1;
			_ly3  = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3  = _ly2;
			_ly4  = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4  = _ly3;
			_ly5  = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5  = _ly4;
			_ly6  = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6  = _ly5;
			_ly7  = coef * (_ly7 + _ly6) - _lx7;			// do 7th filter
			_lx7  = _ly6;
			_ly8  = coef * (_ly8 + _ly7) - _lx8;			// do 8th filter 
			_lx8  = _ly7;
			_ly9  = coef * (_ly9 + _ly8) - _lx9;			// do 9th filter
			_lx9  = _ly8;
			_ly10 = coef * (_ly10 + _ly9) - _lx10;			// do 10th filter 
			_lx10 = _ly9;

			// develop output mix
			in = (float)tanh((1 - wetLevel) * xn + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	}

}


void Phaser::process(float &inL, float &inR)
{
	switch( _stages)
	{
	case 2:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			double input = (inL + inR)/2.f;
			double inmix = input  + paramFeedback  * _ly2;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;

			// develop output mix
			inL = (1.f - wetLevel) * (float)input + wetLevel *(float) _ly2;
			inR = (1.f - wetLevel) * (float)input + wetLevel * (float)_ly2;

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;

	case 4:
	default:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			float input = (inL + inR) / 2.0f;
			double inmix = input + paramFeedback * _ly4;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;

			// develop output mix
			inL = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);
			inR = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	case 6:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			float input = (inL + inR) / 2.0f;
			double inmix = input + paramFeedback * _ly6;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;
			_ly5 = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5 = _ly4;
			_ly6 = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6 = _ly5;

			// develop output mix
			inL = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);
			inR = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;

	case 8:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			float input = (inL + inR) / 2.0f;
			double inmix = input + paramFeedback * _ly8;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;
			_ly5 = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5 = _ly4;
			_ly6 = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6 = _ly5;
			_ly7 = coef * (_ly7 + _ly6) - _lx7;			// do 7th filter
			_lx7 = _ly6;
			_ly8 = coef * (_ly8 + _ly7) - _lx8;			// do 8th filter 
			_lx8 = _ly7;

			// develop output mix
			inL = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);
			inR = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	case 10:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			float input = (inL + inR) / 2.0f;
			double inmix = input + paramFeedback * _ly10;

			// run thru the all pass filters
			_ly1  = coef * (_ly1 + inmix) - _lx1;			// do 1st filter
			_lx1  = inmix;
			_ly2  = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2  = _ly1;
			_ly3  = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3  = _ly2;
			_ly4  = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4  = _ly3;
			_ly5  = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5  = _ly4;
			_ly6  = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6  = _ly5;
			_ly7  = coef * (_ly7 + _ly6) - _lx7;			// do 7th filter
			_lx7  = _ly6;
			_ly8  = coef * (_ly8 + _ly7) - _lx8;			// do 8th filter 
			_lx8  = _ly7;
			_ly9  = coef * (_ly9 + _ly8) - _lx9;			// do 9th filter
			_lx9  = _ly8;
			_ly10 = coef * (_ly10 + _ly9) - _lx10;			// do 10th filter 
			_lx10 = _ly9;

			// develop output mix
			inL = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);
			inR = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	}

}

void Phaser::process(double &in)
{
	switch( _stages)
	{
	case 2:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			double xn = in;
			double inmix = in  + paramFeedback  * _ly2;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;

			// develop output mix
			in = (1 - wetLevel) * xn + wetLevel * _ly2;

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;

	case 4:
	default:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			double xn = in;
			double inmix = in + paramFeedback * _ly4;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;

			// develop output mix
			in = (float)tanh((1 - wetLevel) * xn + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	case 6:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			double xn = in;
			double inmix = in + paramFeedback * _ly6;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;
			_ly5 = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5 = _ly4;
			_ly6 = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6 = _ly5;

			// develop output mix
			in = (float)tanh((1 - wetLevel) * xn + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;

	case 8:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			double xn = in;
			double inmix = in + paramFeedback * _ly8;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;
			_ly5 = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5 = _ly4;
			_ly6 = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6 = _ly5;
			_ly7 = coef * (_ly7 + _ly6) - _lx7;			// do 7th filter
			_lx7 = _ly6;
			_ly8 = coef * (_ly8 + _ly7) - _lx8;			// do 8th filter 
			_lx8 = _ly7;

			// develop output mix
			in = (float)tanh((1 - wetLevel) * in + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	case 10:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			double xn = in;
			double inmix = in + paramFeedback * _ly10;

			// run thru the all pass filters
			_ly1  = coef * (_ly1 + inmix) - _lx1;			// do 1st filter
			_lx1  = inmix;
			_ly2  = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2  = _ly1;
			_ly3  = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3  = _ly2;
			_ly4  = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4  = _ly3;
			_ly5  = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5  = _ly4;
			_ly6  = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6  = _ly5;
			_ly7  = coef * (_ly7 + _ly6) - _lx7;			// do 7th filter
			_lx7  = _ly6;
			_ly8  = coef * (_ly8 + _ly7) - _lx8;			// do 8th filter 
			_lx8  = _ly7;
			_ly9  = coef * (_ly9 + _ly8) - _lx9;			// do 9th filter
			_lx9  = _ly8;
			_ly10 = coef * (_ly10 + _ly9) - _lx10;			// do 10th filter 
			_lx10 = _ly9;

			// develop output mix
			in = (float)tanh((1 - wetLevel) * xn + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	}

}


void Phaser::process(double &inL, double &inR)
{
	switch( _stages)
	{
	case 2:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			double input = (inL + inR)/2.f;
			double inmix = input  + paramFeedback  * _ly2;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;

			// develop output mix
			inL = (1 - wetLevel) * input + wetLevel * _ly2;
			inR = (1 - wetLevel) * input + wetLevel * _ly2;

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;

	case 4:
	default:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			double input = (inL + inR) / 2.0f;
			double inmix = input + paramFeedback * _ly4;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;

			// develop output mix
			inL = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);
			inR = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	case 6:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			double input = (inL + inR) / 2.0f;
			double inmix = input + paramFeedback * _ly6;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;
			_ly5 = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5 = _ly4;
			_ly6 = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6 = _ly5;

			// develop output mix
			inL = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);
			inR = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;

	case 8:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			double input = (inL + inR) / 2.0f;
			double inmix = input + paramFeedback * _ly8;

			// run thru the all pass filters
			_ly1 = coef * (_ly1 + inmix) - _lx1;		// do 1st filter
			_lx1 = inmix;
			_ly2 = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2 = _ly1;
			_ly3 = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3 = _ly2;
			_ly4 = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4 = _ly3;
			_ly5 = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5 = _ly4;
			_ly6 = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6 = _ly5;
			_ly7 = coef * (_ly7 + _ly6) - _lx7;			// do 7th filter
			_lx7 = _ly6;
			_ly8 = coef * (_ly8 + _ly7) - _lx8;			// do 8th filter 
			_lx8 = _ly7;

			// develop output mix
			inL = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);
			inR = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	case 10:
		{
			double coef = (1.0 - _wp) / (1.0 + _wp);     // calc coef for current freq

			// get input value
			double input = (inL + inR) / 2.0f;
			double inmix = input + paramFeedback * _ly10;

			// run thru the all pass filters
			_ly1  = coef * (_ly1 + inmix) - _lx1;			// do 1st filter
			_lx1  = inmix;
			_ly2  = coef * (_ly2 + _ly1) - _lx2;			// do 2nd filter
			_lx2  = _ly1;
			_ly3  = coef * (_ly3 + _ly2) - _lx3;			// do 3rd filter
			_lx3  = _ly2;
			_ly4  = coef * (_ly4 + _ly3) - _lx4;			// do 4th filter 
			_lx4  = _ly3;
			_ly5  = coef * (_ly5 + _ly4) - _lx5;			// do 5th filter
			_lx5  = _ly4;
			_ly6  = coef * (_ly6 + _ly5) - _lx6;			// do 6th filter 
			_lx6  = _ly5;
			_ly7  = coef * (_ly7 + _ly6) - _lx7;			// do 7th filter
			_lx7  = _ly6;
			_ly8  = coef * (_ly8 + _ly7) - _lx8;			// do 8th filter 
			_lx8  = _ly7;
			_ly9  = coef * (_ly9 + _ly8) - _lx9;			// do 9th filter
			_lx9  = _ly8;
			_ly10 = coef * (_ly10 + _ly9) - _lx10;			// do 10th filter 
			_lx10 = _ly9;

			// develop output mix
			inL = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);
			inR = (float)tanh((1 - wetLevel) * input + wetLevel * _ly2);

			// step the sweep
			_wp *= _sweepFactor;                    // adjust freq of filters
			if(_wp > _maxwp || _wp < _minwp)		// max or min?
			{
				_sweepFactor = 1.0 / _sweepFactor;	// reverse
			}
		}
		break;
	}

}
