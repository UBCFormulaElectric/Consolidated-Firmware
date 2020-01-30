/**
 * @brief CAN controller library for STM32F0 and STM32F3
 */

#ifndef SHARED_CAN_H
#define SHARED_CAN_H

/******************************************************************************
 * Includes
 ******************************************************************************/
// Check for STM32 microcontroller family
#ifdef STM32F302x8
// Used in DCM 2017, BMS 2017, and PDM 2018
#include "stm32f3xx_hal.h"
#elif STM32F042x6
// Used in FSM 2017 (Shared CAN Library doesn't yet support this)
#include "stm32f0xx_hal.h"
#else
#error \
    "No valid architecture selected - unable to determine what HAL library to use"
#endif

/******************************************************************************
 * Fixes to allow us to use this library with STM32F0xx Boards
 ******************************************************************************/
#ifdef STM32F042x6
typedef CanRxMsgTypeDef       CAN_RxHeaderTypeDef;
typedef CanTxMsgTypeDef       CAN_TxHeaderTypeDef;
typedef CAN_FilterConfTypeDef CAN_FilterTypeDef;
#define CAN_FILTER_ENABLE ENABLE
#endif

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
#define CAN_PAYLOAD_MAX_NUM_BYTES 8 // Maximum number of bytes in a CAN payload
#define CAN_ExtID_NULL 0 // Set CAN Extended ID to 0 because we are not using it

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/
/**
 * @brief Used to initialize an element in mask_filters[]
 * @param id Can be MASKMODE_16BIT_ID_XXX, where XXX is the PCB name
 * @param mask Can be MASKMODE_16BIT_MASK_XXX, where XXX is the PCB name
 */
#define INIT_MASK_FILTER(filter_id, filter_mask) {.id = filter_id, .mask = filter_mask}

// The following filter IDs/masks must be used with 16-bit Filter Scale
// (FSCx = 0) and Identifier Mask Mode (FBMx = 0). In this mode, the identifier
// registers are associated with mask registers specifying which bits of the
// identifier are handled as "don't care" or as "must match". For each bit in
// the mask registers, 0 = Don't Care and 1 = Must Match.
//
// Bit mapping of a 16-bit identifier register and mask register:
// Standard CAN ID [15:5] RTR[4] IDE[3] Extended CAN ID [2:0]
//
// For example, with the following filter IDs/mask:
// =======================================================
// Identifier Register:    [000 0000 0000] [0] [0] [000]
// Mask Register:          [111 1110 0000] [1] [1] [000]
// =======================================================
// The filter will accept incoming messages that match the following criteria:
// [000 000x xxxx]    [0]    [0]         [xxx]
// Standard CAN ID    RTR    IDE     Extended CAN ID

// Bit definition for FxR1 and FxR2 Registers
#define MASKMODE_16BIT_STDID_Pos  (5U)
#define MASKMODE_16BIT_STDID_Mask (0x7FFU << MASKMODE_16BIT_STDID_Pos) /** 0xFFE0 */
#define MASKMODE_16BIT_RTR_Pos    (4U)
#define MASKMODE_16BIT_RTR_Mask   (0x1U   << MASKMODE_16BIT_RTR_Pos)   /** 0x0010 */
#define MASKMODE_16BIT_IDE_Pos    (3U)
#define MASKMODE_16BIT_IDE_Mask   (0x1U   << MASKMODE_16BIT_IDE_Pos)   /** 0x0008 */
#define MASKMODE_16BIT_EXTID_Pos  (0U)
#define MASKMODE_16BIT_EXTID_Mask (0x7U   << MASKMODE_16BIT_EXTID_Pos) /** 0x0007 */

/** @brief Helper macro to initialize FiRx register in 16-bit mode */
#define INIT_MASKMODE_16BIT_FiRx(std_id, rtr, ide, ext_id) \
      ( (((uint32_t)(std_id) << MASKMODE_16BIT_STDID_Pos) & MASKMODE_16BIT_STDID_Mask) | \
        (((uint32_t)(rtr)    << MASKMODE_16BIT_RTR_Pos)   & MASKMODE_16BIT_RTR_Mask)   | \
        (((uint32_t)(ide)    << MASKMODE_16BIT_IDE_Pos)   & MASKMODE_16BIT_IDE_Mask)   | \
        (((uint32_t)(ext_id) << MASKMODE_16BIT_EXTID_Pos) & MASKMODE_16BIT_EXTID_Mask) )

/** BMS filter - CAN ID Range: 0x00 to 0x1F, RTR: Data Frame, IDE: Standard ID */
// ID:   [000 0000 0000] [0] [0] [000] or 0x0000
// Mask: [111 1110 0000] [1] [1] [000] or 0xFC18
#define MASKMODE_16BIT_ID_BMS   INIT_MASKMODE_16BIT_FiRx(0x0,           \
                                                         CAN_ID_STD,    \
                                                         CAN_RTR_DATA,  \
                                                         CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_BMS INIT_MASKMODE_16BIT_FiRx(0x7E0, 0x1, 0x1, 0x0)

/** DCM filter - CAN ID Range: 0x20 - 0x3F, RTR: Data Frame, IDE: Standard ID */
// ID:   [000 0010 0000] [0] [0] [000] or 0x0400
// Mask: [111 1110 0000] [1] [1] [000] or 0xFC18
#define MASKMODE_16BIT_ID_DCM   INIT_MASKMODE_16BIT_FiRx(0x20,           \
                                                         CAN_ID_STD,     \
                                                         CAN_RTR_DATA,   \
                                                         CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_DCM INIT_MASKMODE_16BIT_FiRx(0x7E0, 0x1, 0x1, 0x0)

/** FSM filter - CAN ID Range: 0x40 - 0x5F, RTR: Data Frame, IDE: Standard ID */
// ID:   [000 0100 0000] [0] [0] [000] or 0x0800
// Mask: [111 1110 0000] [1] [1] [000] or 0xFC18
#define MASKMODE_16BIT_ID_FSM   INIT_MASKMODE_16BIT_FiRx(0x40,           \
                                                         CAN_ID_STD,     \
                                                         CAN_RTR_DATA,   \
                                                         CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_FSM INIT_MASKMODE_16BIT_FiRx(0x7E0, 0x1, 0x1, 0x0)

/** PDM filter - CAN ID Range: 0x60 - 0x7F, RTR: Data Frame, IDE: Standard ID */
// ID:   [000 0110 0000] [0] [0] [000] or 0x0C00
// Mask: [111 1110 0000] [1] [1] [000] or 0xFC18
#define MASKMODE_16BIT_ID_PDM   INIT_MASKMODE_16BIT_FiRx(0x60,           \
                                                         CAN_ID_STD,     \
                                                         CAN_RTR_DATA,   \
                                                         CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_PDM INIT_MASKMODE_16BIT_FiRx(0x7E0, 0x1, 0x1, 0x0)

/** Shared filter - CAN ID Range: 0x80 - 0x9F, RTR: Data Frame, IDE: Standard ID */
// ID:   [000 1000 0000] [0] [0] [000] or 0x1000
// Mask: [111 1110 0000] [1] [1] [000] or 0xFC18
#define MASKMODE_16BIT_ID_SHARED INIT_MASKMODE_16BIT_FiRx(0x80,          \
                                                         CAN_ID_STD,     \
                                                         CAN_RTR_DATA,   \
                                                         CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_SHARED INIT_MASKMODE_16BIT_FiRx(0x7E0, 0x1, 0x1, 0x0)

/** BAMOCAR Tx Filter - ID: 0x190 - 0x19F, RTR: Data Frame, IDE: Standard ID */
// ID:   [001 1001 0000] [0] [0] [000] or 0x3200
// Mask: [111 1111 0000] [1] [1] [000] or 0xFE18
#define MASKMODE_16BIT_ID_BAMOCAR_TX INIT_MASKMODE_16BIT_FiRx(0x190,     \
                                                         CAN_ID_STD,     \
                                                         CAN_RTR_DATA,   \
                                                         CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_BAMOCAR_TX INIT_MASKMODE_16BIT_FiRx(0x7F0, 0x1, 0x1, 0x0)

/** BAMOCAR Rx Filter - ID: 0x210- 0x21F, RTR: Data Frame, IDE: Standard ID */
// ID:   [010 0001 0000] [0] [0] [000] or 0x4200
// Mask: [111 1111 0000] [1] [1] [000] or 0xFE18
#define MASKMODE_16BIT_ID_BAMOCAR_RX INIT_MASKMODE_16BIT_FiRx(0x210,     \
                                                         CAN_ID_STD,     \
                                                         CAN_RTR_DATA,   \
                                                         CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_BAMOCAR_RX INIT_MASKMODE_16BIT_FiRx(0x7F0, 0x1, 0x1, 0x0)

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on
/** @brief Struct to help initialize CAN filters */
typedef struct
{
    // Used to initialize CAN_FilterTypeDef.FilterIdLow and
    // CAN_FilterTypeDef.FilterIdHigh.
    const uint32_t id;

    // Used to initialize CAN_FilterTypeDef.FilterMaskIdLow and
    // CAN_FilterTypeDef.FilterMaskIdHigh.
    const uint32_t mask;
} CanMaskFilterConfig_Struct;

/**
 * @brief The HAL library represents a CAN message using at least two structs:
 *
 *        Struct 1: One of CAN_TxHeaderTypedef and CAN_RxHeaderTypeDef
 *        Struct 2: uint8_t array of size 8
 *
 *        It is quite clunky having to use two structs to represent each CAN
 *        message, so we create a custom struct tailored for our use-case
 *        Note that we stripped away information that we don't need from
 *        CAN_TxHeaderTypeDef and CAN_RxHeaderTypeDef.
 */
struct CanMsg
{
    uint32_t std_id;
    uint32_t dlc;
    uint8_t  data[CAN_PAYLOAD_MAX_NUM_BYTES];
};

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief  Initialize CAN interrupts and CAN filters before starting the CAN
 *         module. After this, the node is active on the bus: it receive
 *         messages, and can send messages. This should be placed inside
 *         MX_CAN_Init() and in the USER CODE BLOCK after HAL_CAN_Init().
 * @param hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @param filters Array CAN filters.
 * @param num_of_filters Number of CAN filters in the array.
 */
void SharedCan_Init(
    CAN_HandleTypeDef *        hcan,
    CanMaskFilterConfig_Struct filters[],
    uint32_t                   num_of_filers);

/**
 * @brief Send a message to the back of the CAN TX queue
 * @param message CAN message to send to queue
 */
void App_SharedCan_TxMessageQueueSendtoBack(struct CanMsg *message);

/**
 * @brief  Overwrite the message at the front of the CAN TX queue
 * @param  message CAN message to overwrite with
 */
void App_SharedCan_TxMessageQueueForceSendToBack(struct CanMsg *message);

/**
 * @brief For messages that we couldn't handle in ISR context, read them into
 *        memory at the task level.
 */
void App_SharedCan_ReadRxMessagesIntoTableFromTask(void);

/**
 * @brief Transmit messages in the CAN TX queue over CAN bus
 */
void App_SharedCan_TransmitEnqueuedCanTxMessagesFromTask(void);

#endif /* SHARED_CAN_H */
