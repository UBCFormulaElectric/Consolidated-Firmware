#include "app_screens.hpp"
#include "io_sevenSeg.hpp"
#include "app_canRx.hpp"
#include "io_sevenSeg.hpp"

// /*********************** Function Definitions ***************************/
static void init_update()
{
    std::array<io::seven_seg::digit, io::seven_seg::DIGITS> data_buffer{ {
        io::seven_seg::dot,
        io::seven_seg::dot,
        io::seven_seg::dot,
        io::seven_seg::dot,
        io::seven_seg::dot,
        io::seven_seg::dot,
        io::seven_seg::dot,
        io::seven_seg::dot,
        io::seven_seg::dot,
    } };

    switch (app::can_rx::VC_FirstFaultNode_get())
    {
        case app::can_utils::ShutdownNode::BSPD_OK:
            data_buffer[0] = io::seven_seg::b;
            data_buffer[1] = io::seven_seg::s;
            data_buffer[2] = io::seven_seg::p;
            data_buffer[3] = io::seven_seg::d;
            break;
        case app::can_utils::ShutdownNode::BMS_OK:
            data_buffer[0] = io::seven_seg::b;
            data_buffer[1] = io::seven_seg::m;
            data_buffer[2] = io::seven_seg::s;
            break;
        case app::can_utils::ShutdownNode::IMD_OK:
            data_buffer[0] = io::seven_seg::i;
            data_buffer[1] = io::seven_seg::m;
            data_buffer[2] = io::seven_seg::d;
            break;
        case app::can_utils::ShutdownNode::TSMS:
            data_buffer[0] = io::seven_seg::t;
            data_buffer[1] = io::seven_seg::s;
            data_buffer[2] = io::seven_seg::m;
            data_buffer[3] = io::seven_seg::s;
            break;
        case app::can_utils::ShutdownNode::Cockpit_EStop:
            data_buffer[0] = io::seven_seg::c;
            data_buffer[1] = io::seven_seg::dot;
            data_buffer[2] = io::seven_seg::e;
            data_buffer[3] = io::seven_seg::s;
            data_buffer[4] = io::seven_seg::t;
            data_buffer[5] = io::seven_seg::o;
            data_buffer[6] = io::seven_seg::p;
            break;
        case app::can_utils::ShutdownNode::FL_INERTIA_ILCK:
            data_buffer[0] = io::seven_seg::f;
            data_buffer[1] = io::seven_seg::l;
            data_buffer[2] = io::seven_seg::dot;
            data_buffer[3] = io::seven_seg::i;
            data_buffer[4] = io::seven_seg::n;
            data_buffer[5] = io::seven_seg::e;
            data_buffer[6] = io::seven_seg::r;
            break;
        case app::can_utils::ShutdownNode::BOTS:
            data_buffer[0] = io::seven_seg::b;
            data_buffer[1] = io::seven_seg::o;
            data_buffer[2] = io::seven_seg::t;
            data_buffer[2] = io::seven_seg::s;
            break;
        case app::can_utils::ShutdownNode::L_EStop:
            data_buffer[0] = io::seven_seg::l;
            data_buffer[1] = io::seven_seg::dot;
            data_buffer[2] = io::seven_seg::e;
            data_buffer[3] = io::seven_seg::s;
            data_buffer[4] = io::seven_seg::t;
            data_buffer[5] = io::seven_seg::o;
            data_buffer[6] = io::seven_seg::p;
            break;
        case app::can_utils::ShutdownNode::R_EStop:
            data_buffer[0] = io::seven_seg::r;
            data_buffer[1] = io::seven_seg::dot;
            data_buffer[2] = io::seven_seg::e;
            data_buffer[3] = io::seven_seg::s;
            data_buffer[4] = io::seven_seg::t;
            data_buffer[5] = io::seven_seg::o;
            data_buffer[6] = io::seven_seg::p;
            break;
        case app::can_utils::ShutdownNode::MSD_EMETER_ILCK:
            data_buffer[0] = io::seven_seg::m;
            data_buffer[1] = io::seven_seg::s;
            data_buffer[2] = io::seven_seg::d;
            break;
        case app::can_utils::ShutdownNode::FR_ILCK:
            data_buffer[0] = io::seven_seg::f;
            data_buffer[1] = io::seven_seg::r;
            data_buffer[2] = io::seven_seg::dot;
            data_buffer[3] = io::seven_seg::i;
            data_buffer[4] = io::seven_seg::l;
            data_buffer[5] = io::seven_seg::c;
            data_buffer[5] = io::seven_seg::k;
            break;
        case app::can_utils::ShutdownNode::RL_ILCK:
            data_buffer[0] = io::seven_seg::r;
            data_buffer[1] = io::seven_seg::l;
            data_buffer[2] = io::seven_seg::dot;
            data_buffer[3] = io::seven_seg::i;
            data_buffer[4] = io::seven_seg::l;
            data_buffer[5] = io::seven_seg::c;
            data_buffer[6] = io::seven_seg::k;
            break;
        case app::can_utils::ShutdownNode::RR_ILCK:
            data_buffer[0] = io::seven_seg::r;
            data_buffer[1] = io::seven_seg::r;
            data_buffer[2] = io::seven_seg::dot;
            data_buffer[3] = io::seven_seg::i;
            data_buffer[4] = io::seven_seg::l;
            data_buffer[5] = io::seven_seg::c;
            data_buffer[6] = io::seven_seg::k;
            break;
        case app::can_utils::ShutdownNode::HV_P_ILCK:
            data_buffer[0] = io::seven_seg::h;
            data_buffer[1] = io::seven_seg::v;
            data_buffer[2] = io::seven_seg::dot;
            data_buffer[3] = io::seven_seg::p;
            data_buffer[4] = io::seven_seg::i;
            data_buffer[5] = io::seven_seg::l;
            data_buffer[6] = io::seven_seg::k;
            break;
        case app::can_utils::ShutdownNode::HV_N_ILCK:
            data_buffer[0] = io::seven_seg::h;
            data_buffer[1] = io::seven_seg::v;
            data_buffer[2] = io::seven_seg::dot;
            data_buffer[3] = io::seven_seg::n;
            data_buffer[4] = io::seven_seg::i;
            data_buffer[5] = io::seven_seg::l;
            data_buffer[6] = io::seven_seg::k;
            break;
        case app::can_utils::ShutdownNode::OK:
            data_buffer[0] = io::seven_seg::o;
            data_buffer[1] = io::seven_seg::k;
            break;
        default:
            // leave all segments blank
            break;
    }
    LOG_IF_ERR(io::seven_seg::write(data_buffer));
}

namespace app::screens
{
const Screen shdn_screen = { .ccw_callback = [] {}, .cw_callback = [] {}, .update = init_update };
}