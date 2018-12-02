/**
 * @file  SharedCan.h
 * @brief CAN controller library for STM32F0 and STM32F3
 */

#ifndef SHARED_CAN_H
#define SHARED_CAN_H

/******************************************************************************
* Includes
*******************************************************************************/
#include "main.h"
#include "CanDefinitions.h"

// Used in DCM 2017, BMS 2017, and PDM 2018
#ifdef STM32F302x8
#include "stm32f3xx_hal.h"
// TODO: Extend support for FSM 2017(STM32F0)
//#elif defined(xxx)
//#include ".h"
#endif

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
// clang-format off
#define CAN_PAYLOAD_SIZE 8 // Maximum number of bytes in a CAN payload
#define CAN_ExtID_NULL 0 // Set CAN Extended ID to 0 because we are not using it
#define CAN_TX_MSG_FIFO_SIZE 20 // Size of CAN FIFO is arbitrary at the moment

/******************************************************************************
* Preprocessor Macros
*******************************************************************************/
/**
 * @brief Used to initialize an element in mask_filters[]
 * @param id Can be MASKMODE_16BIT_ID_XXX, where XXX is the PCB name
 * @param mask Can be MASKMODE_16BIT_MASK_XXX, where XXX is the PCB name
 */
#define INIT_MASK_FILTER(filter_id, filter_mask) {.id = filter_id, .mask = filter_mask}


// [Warning] The following filter IDs/masks must be used with 16-bit Filter Scale
// (FSCx = 0) and Identifier Mask Mode (FBMx = 0). In this mode, the identifier
// registers are associated with mask registers specifying which bits of the
// identifier are handled as "don't care" or as "must match". For each bit in
// the mask registers, 0 = Don't Care and 1 = Must Match.

// Bit definition for FxR1 and FxR2 Registers
#define MASKMODE_16BIT_STDID_Pos  (5U)
#define MASKMODE_16BIT_STDID_Mask (0x7FFU << MASKMODE_16BIT_STDID_Pos) /** 0xFFE0 */
#define MASKMODE_16BIT_RTR_Pos    (4U)
#define MASKMODE_16BIT_RTR_Mask   (0x1U   << MASKMODE_16BIT_RTR_Pos)   /** 0x0010 */
#define MASKMODE_16BIT_IDE_Pos    (3U)
#define MASKMODE_16BIT_IDE_Mask   (0x1U   << MASKMODE_16BIT_IDE_Pos)   /** 0x0008 */
#define MASKMODE_16BIT_EXTID_Pos  (0U)
#define MASKMODE_16BIT_EXTID_Mask (0x7U   << MASKMODE_16BIT_EXTID_Pos) /** 0x0007 */

#define INIT_MASKMODE_16BIT_FiRx(std_id, rtr, ide, ext_id) \
      ( (((uint32_t)(std_id) << MASKMODE_16BIT_STDID_Pos) & MASKMODE_16BIT_STDID_Mask) | \
        (((uint32_t)(rtr)    << MASKMODE_16BIT_RTR_Pos)   & MASKMODE_16BIT_RTR_Mask)   | \
        (((uint32_t)(ide)    << MASKMODE_16BIT_IDE_Pos)   & MASKMODE_16BIT_IDE_Mask)   | \
        (((uint32_t)(ext_id) << MASKMODE_16BIT_EXTID_Pos) & MASKMODE_16BIT_EXTID_Mask) )

// BMS filter - CAN ID Range: 0x00 to 0x1F, RTR: Data Frame, IDE: Standard ID
// ID:   [000 0000 0000] [0] [0] [000] or 0x0000
// Mask: [111 1110 0000] [1] [1] [000] or 0xFC18
#define MASKMODE_16BIT_ID_BMS   INIT_MASKMODE_16BIT_FiRx(0x0,           \
                                                         CAN_ID_STD,    \
                                                         CAN_RTR_DATA,  \
                                                         CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_BMS INIT_MASKMODE_16BIT_FiRx(0x7E0, 0x1, 0x1, 0x0)

// DCM filter - CAN ID Range: 0x20 - 0x3F, RTR: Data Frame, IDE: Standard ID
// ID:   [000 0010 0000] [0] [0] [000] or 0x0400
// Mask: [111 1110 0000] [1] [1] [000] or 0xFC18
#define MASKMODE_16BIT_ID_DCM   INIT_MASKMODE_16BIT_FiRx(0x20,           \
                                                         CAN_ID_STD,     \
                                                         CAN_RTR_DATA,   \
                                                         CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_DCM INIT_MASKMODE_16BIT_FiRx(0x7E0, 0x1, 0x1, 0x0)

// FSM filter - CAN ID Range: 0x40 - 0x5F, RTR: Data Frame, IDE: Standard ID
// ID:   [000 0100 0000] [0] [0] [000] or 0x0800
// Mask: [111 1110 0000] [1] [1] [000] or 0xFC18
#define MASKMODE_16BIT_ID_FSM   INIT_MASKMODE_16BIT_FiRx(0x40,           \
                                                         CAN_ID_STD,     \
                                                         CAN_RTR_DATA,   \
                                                         CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_FSM INIT_MASKMODE_16BIT_FiRx(0x7E0, 0x1, 0x1, 0x0)

// PDM filter - CAN ID Range: 0x60 - 0x7F, RTR: Data Frame, IDE: Standard ID
// ID:   [000 0110 0000] [0] [0] [000] or 0x0C00
// Mask: [111 1110 0000] [1] [1] [000] or 0xFC18
#define MASKMODE_16BIT_ID_PDM   INIT_MASKMODE_16BIT_FiRx(0x60,           \
                                                         CAN_ID_STD,     \
                                                         CAN_RTR_DATA,   \
                                                         CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_PDM INIT_MASKMODE_16BIT_FiRx(0x7E0, 0x1, 0x1, 0x0)

// Shared filter - ID: 0x80 - 0x9F, RTR: Data Frame, IDE: Standard ID
// ID:   [000 1000 0000] [0] [0] [000] or 0x1000
// Mask: [111 1110 0000] [1] [1] [000] or 0xFC18
#define MASKMODE_16BIT_ID_SHARED INIT_MASKMODE_16BIT_FiRx(0x80,          \
                                                         CAN_ID_STD,     \
                                                         CAN_RTR_DATA,   \
                                                         CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_SHARED INIT_MASKMODE_16BIT_FiRx(0x7E0, 0x1, 0x1, 0x0)

// TODO: Recalculate filter values
// Create BACMO Filter - ID: 0x190 - 0x211, RTR: Data Frame, IDE: Standard ID
#define MASKMODE_16BIT_ID_BAMOCAR    (uint32_t)(0x0C00) // 0000 1100 0000 0000
#define MASKMODE_16BIT_MASK_BAMOCAR  (uint32_t)(0xFC18) // 1111 1100 0001 1000
// clang-format on

/******************************************************************************
* Typedefs
*******************************************************************************/
/** @brief Used as index for elements in can_headers[] */
typedef enum
{
    // BMS CAN IDs
    BMS_ERROR = 0,
    BMS_HEARTBEAT,
    BMS_ACCUMULATOR_STATUS,
    BMS_ACCUMULATOR_DISCHARGE_POWER_LIMIT,
    BMS_ACCUMULATOR_REGEN_POWER_LIMIT,
    BMS_ACCUMULATOR_SOC,
    BMS_TURN_ON_ALL_FETS,
    BMS_BALANCE_MOSFET,
    BMS_CELL_VOLTAGE,
    BMS_LATCH_RESET,
    BMS_MOTOR_REENABLE,
    BMS_AIR_SHUTDOWN_ERROR,

    // DCM CAN IDs
    DCM_ERROR,
    DCM_HEARTBEAT,
    DCM_LEFT_SLIP_RATIO,
    DCM_RIGHT_SLIP_RATIO,
    DCM_LEFT_MOTOR_OUTPUT,
    DCM_RIGHT_MOTOR_OUTPUT,
    DCM_LEFT_SLIP_ERROR,
    DCM_RIGHT_SLIP_ERROR,

    // FSM CAN IDs
    FSM_ERROR,
    FSM_HEARTBEAT,
    FSM_BRAKE_APPS,
    FSM_STEERING_ANGLE,
    FSM_LEFT_WHEEL_SPEED,
    FSM_RIGHT_WHEEL_SPEED,

    // PDM CAN IDs
    PDM_ERROR,
    PDM_HEARTBEAT,
    PDM_STARTUP_STATUS,

    // Shared CAN IDs
    MOTOR_SHUTDOWN_ERROR,

    // BAMOCAR D3 CAN IDs
    LEFT_TORQUE_REQUEST_TX,  // BAMOCAR D3 transmits on TX
    RIGHT_TORQUE_REQUEST_TX, // BAMOCAR D3 transmits on TX
    LEFT_TORQUE_REQUEST_RX,  // BAMOCAR D3 receives on RX
    RIGHT_TORQUE_REQUEST_RX, // BAMOCAR D3 receives on RX

    // Demo Example CAN IDs
    DEMO_1_DOUBLE_NUCLEO_TX,
    DEMO_1_DOUBLE_NUCLEO_RX,
    DEMO_2_FLOAT_NUCLEO_TX,
    DEMO_2_FLOAT_NUCLEO_RX,
    DEMO_2_UINT32_NUCLEO_TX,
    DEMO_2_UINT32_NUCLEO_RX,
    DEMO_4_UINT16_NUCLEO_TX,
    DEMO_4_UINT16_NUCLEO_RX,
    DEMO_8_UINT8_NUCLEO_TX,
    DEMO_8_UINT8_NUCLEO_RX,
    DEMO_2_INT32_NUCLEO_TX,
    DEMO_2_INT32_NUCLEO_RX,

    // Number of CAN IDS
    CAN_NODES_COUNT
} CanIds_Enum;

/** @brief Struct to help initialize CAN filters */
const typedef struct
{
    /**
     * Used to initialize CAN_FilterTypeDef.FilterIdLow and
     * CAN_FilterTypeDef.FilterIdHigh.
     */
    uint32_t id;

    /**
     * Used to initialize CAN_FilterTypeDef.FilterMaskIdLow and
     * CAN_FilterTypeDef.FilterMaskIdHigh.
     */
    uint32_t mask;
} CanMaskFilterConfig_Struct;

// TODO: give this updated description
// TODO: see if struct members should be volatile
typedef struct
{
    uint32_t std_id;
    uint32_t dlc;
    uint8_t data[8];
} CanTxMsgQueueItem_Struct;

/** @brief Combine HAL Rx CAN header with CAN payload */
// TODO: see if struct members should be volatile
typedef struct
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t data[8];
} CanRxMsg_Struct;

/** @brief Queue operation status code */
typedef enum
{
    FIFO_SUCCESS = 0,
    FIFO_IS_FULL = 1,
    FIFO_IS_EMPTY = 2,
    FIFO_ERROR = 3
} Fifo_Status_Enum;

/******************************************************************************
* Variables
*******************************************************************************/
/** @brief Expose CAN headesr to other C files */
extern CAN_HandleTypeDef hcan;

/******************************************************************************
* Function Prototypes
*******************************************************************************/
/**
 * @brief  Broadcasts a heartbeat message on the CAN bus
 * @param  module PCB name
 * @return None
 */
void SharedCan_BroadcastHeartbeat(Pcb_Enum module);

//TODO: Update param
/**
 * @brief  Transmits a CAN message
 * @param  std_id: Standard CAN ID
 * @param  dlc: Data to be transmitted (up to 8 bytes)
 * @return None
 */
/**
 * @brief Used to initialize an element in can_headers[].
 *        The following struct members of CAN_TxHeaderTypeDef are hard-coded:
 *        .ExtId: The standard 11-bit CAN identifier is more than sufficent, so
 *                we disable Extended CAN IDs be setting this fied to zero.
 *        .IDE: This field can be either Standard CAN or Extended CAN. See
 *              .ExtID to see why we set this .DIE to Standard CAN.
 *        .RTR: This field can be either Data Frame or Remote Frame. For our
 *              purpose, we only ever transmit Data Frames.
 *        .TransmitGlobalTime: Enabling this gives us a tick-based timestamp,
 *                             but we lose 2-bytes of CAN payload.
 * @param std_id Standard CAN ID
 * @param dlc Data Length Code
 */

void SharedCan_TransmitDataCan(uint32_t std_id, uint32_t dlc, uint8_t *data);

/**
 * @brief  Initialize CAN interrupts and CAN filters before starting the CAN
 *         module. After this, the node is active on the bus: it receive
 *         messages, and can send messages. This should be placed inside
 *         MX_CAN_Init() and  in the USER CODE BLOCK after HAL_CAN_Init().
 * @param  None
 * @return HAL_STATUS
 */
HAL_StatusTypeDef SharedCan_StartCanInInterruptMode(CAN_HandleTypeDef *hcan);

/**
 * @brief  Shared callback function for Rx FIFO 0 and 1
 * @param  hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @param  rx_fifo The Rx FIFO that triggered the callback
 * @return None
 */
void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo);

/**
 * @brief  Rx FIFO 0 message pending callback.
 * @param  hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

/**
 * @brief  Rx FIFO 1 message pending callback.
 * @param  hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);

/**
 * @brief  Shared callback function for transmission mailbox 0, 1, and 2
 * @param  hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @return None
 */
void Can_TxCommonCallback(CAN_HandleTypeDef *hcan);

/**
 * @brief  Transmission Mailbox 0 complete callback.
 * @param  hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan);

/**
 * @brief  Transmission Mailbox 1 complete callback.
 * @param  hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_TXMailbox1CompleteCallback(CAN_HandleTypeDef *hcan);

/**
 * @brief  Transmission Mailbox 2 complete callback.
 * @param  hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_TXMailbox2CompleteCallback(CAN_HandleTypeDef *hcan);

#endif /* SHARED_CAN_H */
