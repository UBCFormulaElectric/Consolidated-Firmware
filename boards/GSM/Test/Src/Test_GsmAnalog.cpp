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
        char* units1 = (char*) "psi";
        char* units2 = (char*) "rpm";
        char* units3 = (char*) "km";
        char* units4 = (char*) "L/min";
        char* units5 = (char*) "Pa"; //pascals
        //slope, y_int >0
        gsmAnalog1 = GSMAnalog_create(2,3, get_sensorAnalogVal_voltage, units1);
        //slope, y_int <0
        gsmAnalog2 = GSMAnalog_create(-1,-4, get_sensorAnalogVal_voltage, units2);
        //slope >0, y_int <0
        gsmAnalog3 = GSMAnalog_create(4,-2,get_sensorAnalogVal_voltage, units3);
        //slope <0, y_int >0
        gsmAnalog4 = GSMAnalog_create(-1,6,get_sensorAnalogVal_voltage, units4);
        //slope, y_int decimal
        gsmAnalog5 = GSMAnalog_create(1.234,5.678,get_sensorAnalogVal_voltage, units5);

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

/*
 * CHECK OUTPUT
 */
//test: gsmAnalog1, pass 0V
TEST_F(GsmAnalogTest, 1pass0V)
{
    get_sensorAnalogVal_voltage_fake.return_val = 0;
    ASSERT_EQ(get_output(gsmAnalog1), 2);
}

//test: gsmAnalog1, pass 3V3, check output
TEST_F(GsmAnalogTest, 1pass3V3)
{
    get_sensorAnalogVal_voltage_fake.return_val = 3.3;
    ASSERT_EQ(get_output(gsmAnalog1), 9.6);
}

//test: gsmAnalog1, pass 1V, check output
TEST_F(GsmAnalogTest, 1pass1V0)
{
    get_sensorAnalogVal_voltage_fake.return_val = 1;
    ASSERT_EQ(get_output(gsmAnalog1), 5);
}

//test: gsmAnalog2, pass 0V
TEST_F(GsmAnalogTest, 2pass0V)
{
    get_sensorAnalogVal_voltage_fake.return_val = 0;
    ASSERT_EQ(get_output(gsmAnalog2), -4);
}

//test: gsmAnalog2, pass 3V3, check output
TEST_F(GsmAnalogTest, 2pass3V3)
{
    get_sensorAnalogVal_voltage_fake.return_val = 3.3;
    ASSERT_EQ(get_output(gsmAnalog2), -7.4);
}

//test: gsmAnalog2, pass 1V, check output
TEST_F(GsmAnalogTest, 2pass1V0)
{
    get_sensorAnalogVal_voltage_fake.return_val = 1;
    ASSERT_EQ(get_output(gsmAnalog2), -5);
}

//test: gsmAnalog3, pass 0V
TEST_F(GsmAnalogTest, 3pass0V)
{
    get_sensorAnalogVal_voltage_fake.return_val = 0;
    ASSERT_EQ(get_output(gsmAnalog3), -2);
}

//test: gsmAnalog3, pass 3V3, check output
TEST_F(GsmAnalogTest, 3pass3V3)
{
    get_sensorAnalogVal_voltage_fake.return_val = 3.3;
    ASSERT_EQ(get_output(gsmAnalog3), 11.2);
}

//test: gsmAnalog3, pass 1V, check output
TEST_F(GsmAnalogTest, 3pass1V0)
{
    get_sensorAnalogVal_voltage_fake.return_val = 1;
    ASSERT_EQ(get_output(gsmAnalog3), 2);
}
//test: gsmAnalog4, pass 0V
TEST_F(GsmAnalogTest, 4pass0V)
{
    get_sensorAnalogVal_voltage_fake.return_val = 0;
    ASSERT_EQ(get_output(gsmAnalog4), 6);
}

//test: gsmAnalog4, pass 3V3, check output
TEST_F(GsmAnalogTest, 4pass3V3)
{
    get_sensorAnalogVal_voltage_fake.return_val = 3.3;
    ASSERT_EQ(get_output(gsmAnalog4), 2.7);
}

//test: gsmAnalog4, pass 1V, check output
TEST_F(GsmAnalogTest, 4pass1V0)
{
    get_sensorAnalogVal_voltage_fake.return_val = 1;
    ASSERT_EQ(get_output(gsmAnalog4), 5);
}
//test: gsmAnalog5, pass 0V
TEST_F(GsmAnalogTest, 5pass0V)
{
    get_sensorAnalogVal_voltage_fake.return_val = 0;
    ASSERT_EQ(get_output(gsmAnalog5), 5.678);
}

//test: gsmAnalog5, pass 3V3, check output
TEST_F(GsmAnalogTest, pass3V3)
{
    get_sensorAnalogVal_voltage_fake.return_val = 3.3;
    ASSERT_EQ(get_output(gsmAnalog5), 9.750);
}

//test: gsmAnalog5, pass 1V, check output
TEST_F(GsmAnalogTest, 5pass1V0)
{
    get_sensorAnalogVal_voltage_fake.return_val = 1;
    ASSERT_EQ(get_output(gsmAnalog5), 6.912);
}

/*
 * CHECK UNIT RETURN
 */
//test: gsmAnalog1, check unis
TEST_F(GsmAnalogTest, units1)
{
    get_sensorAnalogVal_voltage_fake.return_val = 0;
    ASSERT_EQ(get_units(gsmAnalog1), (char*)"psi");
}

//test: gsmAnalog2, check units
TEST_F(GsmAnalogTest, units2)
{
    get_sensorAnalogVal_voltage_fake.return_val = 3.3;
    ASSERT_EQ(get_units(gsmAnalog2), (char*)"rpm");
}

//test: gsmAnalog3, check units
TEST_F(GsmAnalogTest, units3)
{
    get_sensorAnalogVal_voltage_fake.return_val = 0;
    ASSERT_EQ(get_units(gsmAnalog3), (char*)"km");
}

//test: gsmAnalog4, check units
TEST_F(GsmAnalogTest, units4)
{
    get_sensorAnalogVal_voltage_fake.return_val = 1;
    ASSERT_EQ(get_units(gsmAnalog4), (char*)"L/min");
}

//test: gsmAnalog5, check units
TEST_F(GsmAnalogTest, units5)
{
    get_sensorAnalogVal_voltage_fake.return_val = 1;
    ASSERT_EQ(get_units(gsmAnalog5), (char*)"Pa");
}


