/**
 * @brief CAN controller library for STM32F0 and STM32F3
 */

#ifndef SHARED_CAN_H
#define SHARED_CAN_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "CanMsgs.h"

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

// TODO(#356): Remove board-specific ifdefs
#ifdef PDM
    #define CAN_TX_FIFO_OVERFLOW_STDID  CANMSGS_PDM_CAN_TX_FIFO_OVERFLOW_FRAME_ID
    #define CAN_TX_FIFO_OVERFLOW_DLC    CANMSGS_PDM_CAN_TX_FIFO_OVERFLOW_LENGTH
    #define PCB_STARTUP_STDID           CANMSGS_PDM_STARTUP_FRAME_ID
    #define PCB_STARTUP_DLC             CANMSGS_PDM_STARTUP_LENGTH
    #define PCB_STARTUP_DATA            &App_CanMsgsTx_GetCanTxPayloads()->pdm_startup
#elif FSM
    #define CAN_TX_FIFO_OVERFLOW_STDID  CANMSGS_FSM_CAN_TX_FIFO_OVERFLOW_FRAME_ID
    #define CAN_TX_FIFO_OVERFLOW_DLC    CANMSGS_FSM_CAN_TX_FIFO_OVERFLOW_LENGTH
    #define PCB_STARTUP_STDID           CANMSGS_FSM_STARTUP_FRAME_ID
    #define PCB_STARTUP_DLC             CANMSGS_FSM_STARTUP_LENGTH
    #define PCB_STARTUP_DATA            &App_CanMsgsTx_GetCanTxPayloads()->fsm_startup
#elif BMS
    #define CAN_TX_FIFO_OVERFLOW_STDID  CANMSGS_bms_can_tx_fifo_overflow_FRAME_ID
    #define CAN_TX_FIFO_OVERFLOW_DLC    CANMSGS_BMS_CAN_TX_FIFO_OVERFLOW_LENGTH
    #define PCB_STARTUP_STDID           CANMSGS_bms_startup_FRAME_ID
    #define PCB_STARTUP_DLC             CANMSGS_BMS_STARTUP_LENGTH
#elif DCM
    #define CAN_TX_FIFO_OVERFLOW_STDID  CANMSGS_DCM_CAN_TX_FIFO_OVERFLOW_FRAME_ID
    #define CAN_TX_FIFO_OVERFLOW_DLC    CANMSGS_DCM_CAN_TX_FIFO_OVERFLOW_LENGTH
    #define PCB_STARTUP_STDID           CANMSGS_DCM_STARTUP_FRAME_ID
    #define PCB_STARTUP_DLC             CANMSGS_DCM_STARTUP_LENGTH
    #define PCB_STARTUP_DATA            &App_CanMsgsTx_GetCanTxPayloads()->dcm_startup
#else
    #error "No valid PCB name selected"
#endif

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

/** Setup for CAN message callbacks */
#define SHAREDCAN_CAN_MSG_TO_CALLBACK_MAPPING(MSG_STD_ID, MSG_DATA_PTR) \
    uint8_t* ___msg_data = MSG_DATA_PTR; \
    switch(MSG_STD_ID)

/** 
 * @brief Define a callback function for a CAN msg within a 
 *        SHAREDCAN_CAN_MSG_TO_CALLBACK_MAPPING 
 * @param MSG_NAME the name of the CAN message
 * @param MSG_CALLBACK_FUNCTION this function should accept a struct
 *        of type CanMsgs_MSG_NAME_t. It will be called with the CAN
 *        msg when it is received
 */
// TODO (Issue #315): Do something with error code if unpacking fails here!
#define SHAREDCAN_IF_STDID_IS(MSG_NAME, MSG_CALLBACK_FUNCTION) \
    case CANMSGS_##MSG_NAME##_FRAME_ID: \
        { \
        struct CanMsgs_##MSG_NAME##_t ___msg_struct; \
        CanMsgs_##MSG_NAME##_unpack(&___msg_struct, ___msg_data, \
                                    CAN_PAYLOAD_MAX_NUM_BYTES);  \
        MSG_CALLBACK_FUNCTION(&___msg_struct); \
        } \
        break

/**
 * @brief Send the given CAN message
 * @param MSG_NAME the name of the message to send
 * @param MSG_STRUCT_PTR a pointer to a struct of type `CanMsgs_MSG_NAME_t`, 
 *                       that will be packed and sent over the CAN bus
 */
#define SHAREDCAN_SEND_CAN_MSG(MSG_NAME, MSG_STRUCT_PTR) \
    uint8_t ___data[CAN_PAYLOAD_MAX_NUM_BYTES]; \
    int ___size = CanMsgs_##MSG_NAME##_pack(&___data[0], MSG_STRUCT_PTR, \
                                            CAN_PAYLOAD_MAX_NUM_BYTES); \
    SharedCan_TransmitDataCan(CANMSGS_##MSG_NAME##_FRAME_ID, \
                              (size_t)___size, \
                              &___data[0])

/**
 * @brief Send the given CAN message with the given member set to 1 and all 
 * others set to 0
 * @param MSG_NAME The name of the can message to send
 * @param MSG_ELEM_NAME The name of the element in the message to set to 1. 
 * NOTE: This element must be a member of the CAN message struct of name MSG_NAME
 */
#define SHAREDCAN_SEND_CAN_MSG_WITH_SINGLE_BIT_SET(MSG_NAME, MSG_ELEM_NAME) \
    struct CanMsgs_##MSG_NAME##_t ___msg_struct = { 0 }; \
    ___msg_struct.MSG_ELEM_NAME = 1; \
    SHAREDCAN_SEND_CAN_MSG(MSG_NAME, &___msg_struct)


/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on

/** @brief PCB Names */
typedef enum
{
    BATTERY_MANAGEMENT_SYSTEM,
    DRIVE_CONTROL_MODULE,
    POWER_DISTRIBUTION_MODULE,
    FRONT_SENSOR_MODULE,
    PCB_COUNT
} Pcb_Enum;

/** @brief Struct to help initialize CAN filters */
typedef const struct
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

/** @brief All the information required to transmit a CAN message */
struct CanTxMsg
{
    uint32_t std_id;
    uint32_t dlc;
    uint8_t  data[CAN_PAYLOAD_MAX_NUM_BYTES];
};

/** @brief Combine HAL Rx CAN header with CAN payload */
struct CanRxMsg
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t             data[CAN_PAYLOAD_MAX_NUM_BYTES];
};

/******************************************************************************
 * Global Variables
 ******************************************************************************/
extern CAN_HandleTypeDef hcan;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief  Transmits a CAN message
 * @param  std_id Standard CAN ID of the message to send
 * @param  dlc Data length code, indicates the length of the message in bytes
 * @param  data Pointer to data, which should not exceed 64-bits in size.
 */
void SharedCan_TransmitDataCan(uint32_t std_id, uint32_t dlc, void *data);

/**
 * @brief  Overwrite the head item on the CAN TX queue
 * @param  message CAN message to overwrite with
 */
void SharedCan_ForceEnqueueTxMessageAtFront(struct CanTxMsg message);

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
void SharedCan_StartCanInInterruptMode(
    CAN_HandleTypeDef *        hcan,
    CanMaskFilterConfig_Struct filters[],
    uint32_t                   num_of_filers);

/**
 * @brief  Shared callback function for every receive FIFO (STM32F302x8's bxCAN
 *         peripheral has two - FIFO0 and FIFO1).
 * @param  hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @param  rx_fifo The Rx FIFO that triggered the callback
 */
void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo);

/**
 * @brief Get the CAN message that has just been received. This is usually
 *        called from within Can_RxCommonCallback.
 * @param  hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @param  rx_fifo The Rx FIFO to get the message from
 * @param  rx_msg This struct will be populated appropriately with the received
 *         CAN message
 * @return HAL_STATUS
 */
HAL_StatusTypeDef SharedCan_ReceiveDataCan(
    CAN_HandleTypeDef *hcan,
    uint32_t           rx_fifo,
    struct CanRxMsg *  rx_msg);

#endif /* SHARED_CAN_H */
