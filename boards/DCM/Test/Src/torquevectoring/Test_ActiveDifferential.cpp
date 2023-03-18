/**
 * Notes:
 * extern "C": specifies C linkage for functions to compiler
 *
 *
 *
*/

#include "Test_Dcm.h"

extern "C"
{
#include "torquevectoring/App_PowerLimiting.h"
#include "torquevectoring/App_ActiveDifferential.h"
#include "torquevectoring/App_TorqueVectoringConstants.h"
#include "math.h"
}