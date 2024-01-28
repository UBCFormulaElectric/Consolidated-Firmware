# Torque vectoring algorithm overview

This folder contains the functionality for the torque vectoring algorithm. 

The main parts are: 

* Power Limiting - calculate max power based on all available limiting factors
* Active Differential - modulate left and right torques while turning to help cornering
* Traction control - actively modulate torques to minimize the slip ratio of the wheels and stabilize the car

More details on this algorithm can be found here: 

[https://ubcformulaelectric.atlassian.net/wiki/spaces/UFE/pages/51904893/Software+-+Thruna+FW+-+DCM+-+Traction+Control+Torque+Vectoring](https://ubcformulaelectric.atlassian.net/wiki/spaces/UFE/pages/51904893/Software+-+Thruna+FW+-+DCM+-+Traction+Control+Torque+Vectoring)