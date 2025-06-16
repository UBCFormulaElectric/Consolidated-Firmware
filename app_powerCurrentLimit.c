commit a5c58f21722a0ef155a599789a4837bd7908b13b
Author: Jack Polloway <60236839+yawollopkcaj@users.noreply.github.com>
Date:   Sun Feb 2 19:07:29 2025 -0800

    changes from main: added io file for VC in quintuna, added LV battery monitor file and header and had cubemx gutogenerate code for configuring an alert pin
    
    changing a few files that have arm_math.h errors from cpp to c files seemed to make it build properly...
    
    remove unnecssary files

diff --git a/firmware/quintuna/VC/src/cubemx/Inc/main.h b/firmware/quintuna/VC/src/cubemx/Inc/main.h
index 6fe60cc77..99b6006e9 100644
--- a/firmware/quintuna/VC/src/cubemx/Inc/main.h
+++ b/firmware/quintuna/VC/src/cubemx/Inc/main.h
@@ -53,11 +53,14 @@ extern "C"
     /* Exported functions prototypes ---------------------------------------------*/
     void Error_Handler(void);
 
-    /* USER CODE BEGIN EFP */
+/* USER CODE BEGIN EFP */
 
-    /* USER CODE END EFP */
+/* USER CODE END EFP */
 
-    /* Private defines -----------------------------------------------------------*/
+/* Private defines -----------------------------------------------------------*/
+#define BAT_MTR_ALERT_Pin GPIO_PIN_11
+#define BAT_MTR_ALERT_GPIO_Port GPIOE
+#define BAT_MTR_ALERT_EXTI_IRQn EXTI15_10_IRQn
 
     /* USER CODE BEGIN Private defines */
 
diff --git a/firmware/quintuna/VC/src/cubemx/Inc/stm32h7xx_it.h b/firmware/quintuna/VC/src/cubemx/Inc/stm32h7xx_it.h
index 4d8a2f385..1169b1794 100644
--- a/firmware/quintuna/VC/src/cubemx/Inc/stm32h7xx_it.h
+++ b/firmware/quintuna/VC/src/cubemx/Inc/stm32h7xx_it.h
@@ -54,6 +54,7 @@ extern "C"
     void UsageFault_Handler(void);
     void DebugMon_Handler(void);
     void SysTick_Handler(void);
+    void EXTI15_10_IRQHandler(void);
     /* USER CODE BEGIN EFP */
 
     /* USER CODE END EFP */
diff --git a/firmware/quintuna/VC/src/cubemx/Src/main.c b/firmware/quintuna/VC/src/cubemx/Src/main.c
index 19754cd32..fda8d1943 100644
--- a/firmware/quintuna/VC/src/cubemx/Src/main.c
+++ b/firmware/quintuna/VC/src/cubemx/Src/main.c
@@ -54,8 +54,9 @@ const osThreadAttr_t defaultTask_attributes = {
 /* USER CODE END PV */
 
 /* Private function prototypes -----------------------------------------------*/
-void SystemClock_Config(void);
-void StartDefaultTask(void *argument);
+void        SystemClock_Config(void);
+static void MX_GPIO_Init(void);
+void        StartDefaultTask(void *argument);
 
 /* USER CODE BEGIN PFP */
 
@@ -93,6 +94,7 @@ int main(void)
     /* USER CODE END SysInit */
 
     /* Initialize all configured peripherals */
+    MX_GPIO_Init();
     /* USER CODE BEGIN 2 */
 
     /* USER CODE END 2 */
@@ -195,6 +197,34 @@ void SystemClock_Config(void)
     }
 }
 
+/**
+ * @brief GPIO Initialization Function
+ * @param None
+ * @retval None
+ */
+static void MX_GPIO_Init(void)
+{
+    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
+    /* USER CODE BEGIN MX_GPIO_Init_1 */
+    /* USER CODE END MX_GPIO_Init_1 */
+
+    /* GPIO Ports Clock Enable */
+    __HAL_RCC_GPIOE_CLK_ENABLE();
+
+    /*Configure GPIO pin : BAT_MTR_ALERT_Pin */
+    GPIO_InitStruct.Pin  = BAT_MTR_ALERT_Pin;
+    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
+    GPIO_InitStruct.Pull = GPIO_NOPULL;
+    HAL_GPIO_Init(BAT_MTR_ALERT_GPIO_Port, &GPIO_InitStruct);
+
+    /* EXTI interrupt init*/
+    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
+    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
+
+    /* USER CODE BEGIN MX_GPIO_Init_2 */
+    /* USER CODE END MX_GPIO_Init_2 */
+}
+
 /* USER CODE BEGIN 4 */
 
 /* USER CODE END 4 */
diff --git a/firmware/quintuna/VC/src/cubemx/Src/stm32h7xx_it.c b/firmware/quintuna/VC/src/cubemx/Src/stm32h7xx_it.c
index 65c894eab..c4a67eff5 100644
--- a/firmware/quintuna/VC/src/cubemx/Src/stm32h7xx_it.c
+++ b/firmware/quintuna/VC/src/cubemx/Src/stm32h7xx_it.c
@@ -182,6 +182,20 @@ void SysTick_Handler(void)
 /* please refer to the startup file (startup_stm32h7xx.s).                    */
 /******************************************************************************/
 
+/**
+ * @brief This function handles EXTI line[15:10] interrupts.
+ */
+void EXTI15_10_IRQHandler(void)
+{
+    /* USER CODE BEGIN EXTI15_10_IRQn 0 */
+
+    /* USER CODE END EXTI15_10_IRQn 0 */
+    HAL_GPIO_EXTI_IRQHandler(BAT_MTR_ALERT_Pin);
+    /* USER CODE BEGIN EXTI15_10_IRQn 1 */
+
+    /* USER CODE END EXTI15_10_IRQn 1 */
+}
+
 /* USER CODE BEGIN 1 */
 
 /* USER CODE END 1 */
diff --git a/firmware/quintuna/VC/src/cubemx/VC.ioc b/firmware/quintuna/VC/src/cubemx/VC.ioc
index c7da9aa17..37fbeb79b 100644
--- a/firmware/quintuna/VC/src/cubemx/VC.ioc
+++ b/firmware/quintuna/VC/src/cubemx/VC.ioc
@@ -20,10 +20,11 @@ Mcu.IP5=SYS
 Mcu.IPNb=6
 Mcu.Name=STM32H733VGTx
 Mcu.Package=LQFP100
-Mcu.Pin0=VP_FREERTOS_VS_CMSIS_V2
-Mcu.Pin1=VP_SYS_VS_Systick
-Mcu.Pin2=VP_MEMORYMAP_VS_MEMORYMAP
-Mcu.PinsNb=3
+Mcu.Pin0=PE11
+Mcu.Pin1=VP_FREERTOS_VS_CMSIS_V2
+Mcu.Pin2=VP_SYS_VS_Systick
+Mcu.Pin3=VP_MEMORYMAP_VS_MEMORYMAP
+Mcu.PinsNb=4
 Mcu.ThirdPartyNb=0
 Mcu.UserConstants=
 Mcu.UserName=STM32H733VGTx
@@ -31,6 +32,7 @@ MxCube.Version=6.12.0
 MxDb.Version=DB.6.0.120
 NVIC.BusFault_IRQn=true\:0\:0\:false\:false\:true\:false\:false\:false\:false
 NVIC.DebugMonitor_IRQn=true\:0\:0\:false\:false\:true\:false\:false\:false\:false
+NVIC.EXTI15_10_IRQn=true\:5\:0\:false\:false\:true\:true\:true\:true\:true
 NVIC.ForceEnableDMAVector=true
 NVIC.HardFault_IRQn=true\:0\:0\:false\:false\:true\:false\:false\:false\:false
 NVIC.MemoryManagement_IRQn=true\:0\:0\:false\:false\:true\:false\:false\:false\:false
@@ -43,6 +45,10 @@ NVIC.SavedSvcallIrqHandlerGenerated=true
 NVIC.SavedSystickIrqHandlerGenerated=true
 NVIC.SysTick_IRQn=true\:15\:0\:false\:false\:true\:true\:false\:true\:false
 NVIC.UsageFault_IRQn=true\:0\:0\:false\:false\:true\:false\:false\:false\:false
+PE11.GPIOParameters=GPIO_Label
+PE11.GPIO_Label=BAT_MTR_ALERT
+PE11.Locked=true
+PE11.Signal=GPXTI11
 PinOutPanel.RotationAngle=0
 ProjectManager.AskForMigrate=true
 ProjectManager.BackupPrevious=false
@@ -139,6 +145,8 @@ RCC.VCO3OutputFreq_Value=258000000
 RCC.VCOInput1Freq_Value=2000000
 RCC.VCOInput2Freq_Value=2000000
 RCC.VCOInput3Freq_Value=2000000
+SH.GPXTI11.0=GPIO_EXTI11
+SH.GPXTI11.ConfNb=1
 VP_FREERTOS_VS_CMSIS_V2.Mode=CMSIS_V2
 VP_FREERTOS_VS_CMSIS_V2.Signal=FREERTOS_VS_CMSIS_V2
 VP_MEMORYMAP_VS_MEMORYMAP.Mode=CurAppReg
@@ -146,3 +154,4 @@ VP_MEMORYMAP_VS_MEMORYMAP.Signal=MEMORYMAP_VS_MEMORYMAP
 VP_SYS_VS_Systick.Mode=SysTick
 VP_SYS_VS_Systick.Signal=SYS_VS_Systick
 board=custom
+rtos.0.ip=FREERTOS
diff --git a/firmware/quintuna/VC/src/cubemx/VC.ioc.md5 b/firmware/quintuna/VC/src/cubemx/VC.ioc.md5
index f2ec8fd05..b3307c776 100644
--- a/firmware/quintuna/VC/src/cubemx/VC.ioc.md5
+++ b/firmware/quintuna/VC/src/cubemx/VC.ioc.md5
@@ -1 +1 @@
-177bf4de6cb3688df18d7a671851b69c
\ No newline at end of file
+b8be052619827993824a13eb03b2c981
\ No newline at end of file
diff --git a/firmware/quintuna/VC/src/io/io_lowVoltageBattery.c b/firmware/quintuna/VC/src/io/io_lowVoltageBattery.c
new file mode 100644
index 000000000..5fdb63855
--- /dev/null
+++ b/firmware/quintuna/VC/src/io/io_lowVoltageBattery.c
@@ -0,0 +1,181 @@
+#include "io_lowVoltageBattery.h"
+#include "hw_i2c.h"
+#include "hw_hal.h"
+
+const uint8_t  BQ76922_I2C_ADDR      = 0x10;   // 7-bit I2C address
+const uint16_t CMD_DASTATUS6         = 0x0076; // Subcommand for accumulated charge
+const uint8_t  COMMAND_ADDRESS       = 0x3E;
+const uint8_t  REG_SUBCOMMAND_LSB    = 0x3E;
+const uint8_t  REG_SUBCOMMAND_MSB    = 0x3F;
+const uint8_t  REG_DATA_BUFFER       = 0x40;
+const uint8_t  REG_CHECKSUM          = 0x60;
+const uint8_t  REG_RESPONSE_LENGTH   = 0x61;
+const uint16_t CELL0_VOLTAGE_COMMAND = 0x1514;
+const uint16_t CELL1_VOLTAGE_COMMAND = 0x1716;
+const uint16_t CELL2_VOLTAGE_COMMAND = 0x1B1A;
+const uint16_t CELL4_VOLTAGE_COMMAND = 0x1D1C;
+const uint16_t STACK_VOLTAGE_COMMAND = 0x3534;
+
+#define R_SENSE 2.0f    // Sense resistor in mΩ
+#define Q_FULL 11200.0f // Battery full charge capacity in mAh
+
+extern I2C_HandleTypeDef hi2c1;
+
+static I2cInterface lvBatMon = { &hi2c1, BQ76922_I2C_ADDR, 100 };
+
+bool io_lowVoltageBattery_init()
+{
+    return hw_i2c_isTargetReady(&lvBatMon);
+}
+
+bool writeSubcommand(uint16_t subcommand)
+{
+    uint8_t data[2] = { subcommand & 0xFF, (subcommand >> 8) & 0xFF };
+    if (!hw_i2c_memWrite(&hi2c1, REG_SUBCOMMAND_LSB, data, 2))
+    {
+        return false;
+    }
+
+    return true;
+}
+
+uint8_t readResponseLength()
+{
+    uint8_t responseLength;
+    if (!hw_i2c_memRead(&lvBatMon, REG_RESPONSE_LENGTH, &responseLength, sizeof(responseLength)))
+    {
+        return 0;
+    }
+    if (responseLength > sizeof(uint8_t))
+    {
+        return 0;
+    }
+    return responseLength;
+}
+
+bool readCharge(uint32_t *charge, uint16_t *time)
+{
+    uint8_t responseLen = readResponseLength();
+
+    if (responseLen != 6)
+    {
+        return false;
+    }
+
+    uint8_t buffer[6];
+
+    if (!hw_i2c_memRead(&hi2c1, REG_DATA_BUFFER, buffer, 6))
+    {
+        return false;
+    }
+
+    uint8_t checksum;
+    if (!hw_i2c_memRead(&hi2c1, REG_CHECKSUM, &checksum, 1))
+    {
+        return false;
+    }
+
+    uint8_t calculated_checksum = (CMD_DASTATUS6 & 0xFF) + (CMD_DASTATUS6 >> 8) + responseLen;
+    for (int i = 0; i < responseLen; i++)
+    {
+        calculated_checksum += buffer[i];
+    }
+    calculated_checksum = ~calculated_checksum; // Invert bits
+
+    if (calculated_checksum != checksum)
+    {
+        return false;
+    }
+
+    *charge = (buffer[0] | (buffer[1] << 8) | (buffer[2] << 16));
+    *time   = (buffer[3] | (buffer[4] << 8));
+
+    return true;
+}
+
+float calculateSOC(uint32_t charge)
+{
+    float CC_GAIN    = 7.4768f / R_SENSE;
+    float charge_mAh = (charge * CC_GAIN) / 3600.0f;
+    return (charge_mAh / Q_FULL) * 100.0f;
+}
+
+bool io_lowVoltageBattery_getPackCharge(float *SOC)
+{
+    uint32_t *charge;
+    uint16_t *time;
+
+    if (!writeSubcommand(CMD_DASTATUS6))
+    {
+        return false;
+    }
+
+    if (!readCharge(charge, time))
+    {
+        return false;
+    }
+
+    *SOC = calculateSOC(charge);
+
+    return true;
+}
+
+bool io_lowVoltageBattery_getPackVoltage(uint16_t *packVoltage)
+{
+    // 1 & 2. Write lower byte of command to 0x3E and upper byte of command to 0x3F
+    uint8_t command_buffer[2];
+    command_buffer[0] = 0x36;
+    command_buffer[1] = 0x37;
+    if (!hw_i2c_memWrite(&lvBatMon, COMMAND_ADDRESS, command_buffer, sizeof(command_buffer)))
+    {
+        return false;
+    }
+
+    // 3. Read 0x3E and 0x3F until readback returns original commands
+    // NOTE: Is there a safer/better way to poll addresses 0x3E and 0x3F?
+    // Loop should break and return false after waiting too long
+    bool readDataReady = false;
+    while (!readDataReady)
+    {
+        uint8_t rx_buffer[2];
+        if (hw_i2c_memRead(&lvBatMon, COMMAND_ADDRESS, rx_buffer, sizeof(rx_buffer)))
+        {
+            if (rx_buffer[0] == command_buffer[0] && rx_buffer[1] == command_buffer[1])
+            {
+                readDataReady = true;
+            }
+        }
+    }
+
+    // 4. Read the length of response from 0x61
+    uint8_t responseLength;
+    if (!hw_i2c_memRead(&lvBatMon, 0x61, &responseLength, sizeof(responseLength)))
+    {
+        return false;
+    }
+    if (responseLength > sizeof(uint16_t))
+    {
+        return false; // Response length too large, not 16-bits
+    }
+    // 5. Read buffer starting at 0x40 using responseLength
+    uint16_t voltageBuffer;
+    if (!hw_i2c_memRead(&lvBatMon, 0x40, &voltageBuffer, responseLength))
+    {
+        return false;
+    }
+    // 6. Read the checksum at 0x60, if checkSum == data read, update packVoltage
+    uint8_t checkSum;
+    if (!hw_i2c_memRead(&lvBatMon, 0x60, &checkSum, sizeof(checkSum)))
+    {
+        return false;
+    }
+    if (checkSum != voltageBuffer)
+    {
+        return false;
+    }
+    else
+    {
+        *packVoltage = voltageBuffer;
+    }
+    return true;
+}
\ No newline at end of file
diff --git a/firmware/quintuna/VC/src/io/io_lowVoltageBattery.h b/firmware/quintuna/VC/src/io/io_lowVoltageBattery.h
new file mode 100644
index 000000000..8d5a8cfc6
--- /dev/null
+++ b/firmware/quintuna/VC/src/io/io_lowVoltageBattery.h
@@ -0,0 +1,14 @@
+#pragma once
+
+#include "hw_gpio.h"
+
+/**
+ * Initialize the LV battery monitor.
+ */
+bool io_lowVoltageBattery_init();
+
+/**
+ * Gets current from low voltage battery ()
+ * @return current from battery
+ */
+float io_lowVoltageBattery_getPackVoltage();
\ No newline at end of file
