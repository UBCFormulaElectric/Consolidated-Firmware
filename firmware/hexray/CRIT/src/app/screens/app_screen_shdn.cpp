#include "app_brightness.hpp"
#include "app_screens.hpp"
#include "io_sevenSeg.hpp"
#include "app_canRx.hpp"
// /*********************** Function Definitions ***************************/
static void init_update()
{
    // std::array<io::seven_seg::digit, io::seven_seg::DIGITS> data_buffer{ {
    //     io::seven_seg::dot,
    //     io::seven_seg::dot,
    //     io::seven_seg::dot,
    //     io::seven_seg::dot,
    //     io::seven_seg::dot,
    //     io::seven_seg::dot,
    //     io::seven_seg::dot,
    //     io::seven_seg::dot,
    //     io::seven_seg::dot,
    // } };
    char data_buffer[io::seven_seg::DIGITS + 1];

    switch (app::can_rx::VC_FirstFaultNode_get())
    {
        case app::can_utils::ShutdownNode::BSPD_OK:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdbspd");
            break;
        case app::can_utils::ShutdownNode::BMS_OK:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdbms");
            break;
        case app::can_utils::ShutdownNode::IMD_OK:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdimd");
            break;
        case app::can_utils::ShutdownNode::TSMS:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdtsms");
            break;
        case app::can_utils::ShutdownNode::Cockpit_EStop:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdcestop");
            break;
        case app::can_utils::ShutdownNode::FL_ILCK:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdflilck");
            break;
        case app::can_utils::ShutdownNode::BOTS:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdbots");
            break;
        case app::can_utils::ShutdownNode::L_EStop:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdlestop");
            break;
        case app::can_utils::ShutdownNode::R_EStop:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdrestop");
            break;
        case app::can_utils::ShutdownNode::MSD_EMETER_ILCK:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdmsd");
            break;
        case app::can_utils::ShutdownNode::FR_ILCK:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdfrilck");
            break;
        case app::can_utils::ShutdownNode::RL_ILCK:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdrlilck");
            break;
        case app::can_utils::ShutdownNode::RR_ILCK:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdrrilck");
            break;
            break;
        case app::can_utils::ShutdownNode::HV_P_ILCK:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdhvp");
            break;
        case app::can_utils::ShutdownNode::HV_N_ILCK:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdhvn");
            break;
        case app::can_utils::ShutdownNode::OK:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "ok.lessgo");
            break;
        case app::can_utils::ShutdownNode::INERTIA:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "shdinert.");
            break;
        default:
            // leave all segments blank
            break;
    }

    LOG_IF_ERR(io::seven_seg::write(std::span<char, io::seven_seg::DIGITS>{ data_buffer, io::seven_seg::DIGITS }));
}

namespace app::screens
{
const Screen shdn_screen = { .ccw_callback = [] {}, .cw_callback = [] {}, .update = init_update };
}