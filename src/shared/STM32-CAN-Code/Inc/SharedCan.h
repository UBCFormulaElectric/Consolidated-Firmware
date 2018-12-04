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
#elif STM32F042x6
// Used in FSM 2017 (Shared CAN Library doesn't yet support this)
#include "stm32f0xx_hal.h"
#endif

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
// clang-format off
#define CAN_PAYLOAD_BYTE_SIZE 8 // Maximum number of bytes in a CAN payload
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

// TODO: Recalculate filter values
/** BAMOCAR Filter - ID: 0x190 - 0x211, RTR: Data Frame, IDE: Standard ID */
#define MASKMODE_16BIT_ID_BAMOCAR    (uint32_t)(0x0C00) // 0000 1100 0000 0000
#define MASKMODE_16BIT_MASK_BAMOCAR  (uint32_t)(0xFC18) // 1111 1100 0001 1000
// clang-format on

/******************************************************************************
* Typedefs
*******************************************************************************/
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

/** @brief Struct to enqueue CAN messages that could not be transmitted at the
 *         time of request */
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
extern CAN_HandleTypeDef hcan;

/******************************************************************************
* Function Prototypes
*******************************************************************************/
/**
 * @brief  Transmits a CAN message
 * @param  std_id: Standard CAN ID
 * @param  dlc Data to be transmitted (up to 8 bytes)
 * @param  data Pointer to an uint8_t array with 8 elements (64-bits in total)
 * @return None
 */
void SharedCan_TransmitDataCan(uint32_t std_id, uint32_t dlc, uint8_t *data);

/**
 * @brief  Initialize CAN interrupts and CAN filters before starting the CAN
 *         module. After this, the node is active on the bus: it receive
 *         messages, and can send messages. This should be placed inside
 *         MX_CAN_Init() and in the USER CODE BLOCK after HAL_CAN_Init().
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

#endif /* SHARED_CAN_H */
