#include <gtest/gtest>
#include <array>
#include "app_math.hpp"

using app::math::matrix; 
using app::state_estimation::kf;

/**
* Writing tests for a simple 1D analytical case, the idea here is most of that work can be hand computed so we can 
* verify. Kalman filters can be difficult to verify so this will serve as a sanity check for us. Thus we will use the simple case of velocity estimation here
*/

/**
* For this lets have a two state system say x1 = position and x2 = velocity with a single input say pedal percentage 
* for this we can quickly identify our differential equations
* we know Torque = F* d, and per wheel this looks like Torque = F * r(wheel), now to make a simple assumption on the car we say Fcar = sum of Fwheel as the car is a rigid body
* to find F we do T/r and then we know the torque request comming in is Tmax (22 N*m) * pedal percent and then lets assume no torque vecotring so this is the torque to each motor so Tmax * pedel percent * 4 / wheel_rad
* is our total force but then the last thing we need to keep in mind is that there is a gear ratio to put the torque to motor to the wheel, for simplicity lets say thats 10
* additioanlly lets assume r to be 0.5 so we get F = (22 * 4 * 10 * / 0.5) * p
* now to complete our system equatiosn we need to convert this forfce to an accep so we know F=ma which now = (22*4*10*2) * p
* assuming m = 300 kg so a = (22*4*10*2 / 300) * p --> 5.87 * p  = a
* so we can now form our differential eqns (let ~state be the derrivative of the state)
* x1(t) = x1(t-1) + x2(t-1) * (delta t) + 5.87 * p * (delta t)^2 
* x2(t) = x2(t-1) + (5.87*p) * (delta t)
* this can then be used to form our F and B matrix 
* F = [(1) (delta t)]
      [ (1)    0     ]
* B = [5.87 * (delta t)^2]
*.    [5.87 * p          ]
*/

