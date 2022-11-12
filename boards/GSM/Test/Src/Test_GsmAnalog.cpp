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
        char* units1 = (char*)"psi";
        char* units2 = (char*)"rpm";
        //slope, y_int >0
        gsmAnalog1 = GSMAnalog_create(2,3, get_sensorAnalogVal_voltage, units1);
        //slope, y_int <0
        gsmAnalog2 = GSMAnalog_create(-1,-4, get_sensorAnalogVal_voltage, units2);
        //slope >0, y_int <0
        gsmAnalog3 = GSMAnalog_create(4,-2,get_sensorAnalogVal_voltage, units1);
        //slope <0, y_int >0
        gsmAnalog4 = GSMAnalog_create(-1,6,get_sensorAnalogVal_voltage, units1);
        //slope, y_int decimal
        gsmAnalog5 = GSMAnalog_create(1.234,5.678,get_sensorAnalogVal_voltage, units1);

    }
    void TearDown()override {
        TearDownObject(gsmAnalog1, App_GSMAnalog_Destroy);
        TearDownObject(gsmAnalog2,App_GSMAnalog_Destroy);
        TearDownObject(gsmAnalog3,App_GSMAnalog_Destroy);
        TearDownObject(gsmAnalog4,App_GSMAnalog_Destroy);
        TearDownObject(gsmAnalog5,App_GSMAnalog_Destroy);
    }
    struct GSMAnalog *gsmAnalog1;
    struct GSMAnalog *gsmAnalog2;
    struct GSMAnalog *gsmAnalog3;
    struct GSMAnalog *gsmAnalog4;
    struct GSMAnalog *gsmAnalog5;
};

//test1 :if pass in 0V with y-int = 2, slope =3 , check that output = 2
TEST_F(GsmAnalogTest, pass0V)
{
    get_sensorAnalogVal_voltage_fake.return_val = 0;
    ASSERT_EQ(get_output(gsmAnalog1), 2);
}

//test1 :if pass in 3V3 with y-int = 2, slope =3 , check that output = 3.3
TEST_F(GsmAnalogTest, pass3V3)
{
    get_sensorAnalogVal_voltage_fake.return_val = 3.3;
    ASSERT_EQ(get_output(gsmAnalog1), 2);
}

//test1 :if pass in 1V with y-int = 2, slope =3 , check that output = 5
TEST_F(GsmAnalogTest, pass1V0)
{
    get_sensorAnalogVal_voltage_fake.return_val = 1;
    ASSERT_EQ(get_output(gsmAnalog1), 2);
}

//test2 :if pass in 2V3 with y-int = , slope = , check that output =
TEST_F(GsmAnalogTest, pass2V3)
{
    get_sensorAnalogVal_voltage_fake.return_val = 2.3;
    ASSERT_EQ(get_output(gsmAnalog2), 2);
}

//test3 :if pass in 3V3 with y-int = , slope = , check that output =


