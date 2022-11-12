//
// Created by formulae on 05/11/22.
//
#include "Test_Gsm.h"

extern "C"
{
#include "App_GsmAnalog.h"
}

FAKE_VALUE_FUNC(float, get_sensorAnalogVal_voltage);



class GsmAnalogTest : public testing:: Test
{
protected:
    void SetUp() override {
        char* units = (char*)"psi";
        gsmAnalog = GSMAnalog_create(2,3, get_sensorAnalogVal_voltage, units);
    }
    void TearDown()override {
        TearDownObject(gsmAnalog, App_GSMAnalog_Destroy);
    }
    struct GSMAnalog *gsmAnalog;
};

//test1 :if pass in 0V with y-int = 2, slope =3 , check that output = 2
TEST_F(GsmAnalogTest, pass0V)
{
    get_sensorAnalogVal_voltage_fake.return_val = 0;
    ASSERT_EQ(get_output(gsmAnalog), 2);
}

//test2 :if pass in 2V3 with y-int = , slope = , check that output =

//test3 :if pass in 3V3 with y-int = , slope = , check that output =


