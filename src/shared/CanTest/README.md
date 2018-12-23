## Overview
This folder contains our library for running tests over CAN. The goal is to provide a simple framework by which simple tests can be run, and the results seen, entirely over CAN.

## How to Use This Library
1. Setup our `SharedCan` library.

2. Add the board name to **Preprocessor Symbols** inside Keil. This will determine which CAN filters are activated.
<img src="https://user-images.githubusercontent.com/16970019/49477422-c5967e80-f7d1-11e8-95ee-74e2a36f4b73.png" width="50%" height="50%">

3. Add `CanTest_handleCanMsg` to `Can_RxCommonCallback`, ex:

```
void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    CanRxMsg_Struct rx_msg;

    HAL_CAN_GetRxMessage(hcan, rx_fifo, &rx_msg.rx_header, &rx_msg.data[0]);

    CanTest_handleCanMsg(&rx_msg);
}
```
this will allow the framework to check each CAN message to see if it should run a test based on that message or not.

4. Add a `Test.c` file to your project. Ex:

```
// Test.c
#include "CanTest.h"

CANTEST_TEST(7){
    CANTEST_ASSERT(1 == 2);
}

CANTEST_TEST(60000){
    CANTEST_ASSERT(2 == 2);
    CANTEST_SET_EXTRA_VALUES(1, 2, 3, 4);
}

CanTest_TestResult CanTest_runTestWithId(uint16_t test_id) {
    CANTEST_START_TESTS;

    CANTEST_ADD_TEST(7);
    CANTEST_ADD_TEST(60000);

    CANTEST_END_TESTS;
}
```
The above example creates two tests with id's 7 and 60000 respectively.
