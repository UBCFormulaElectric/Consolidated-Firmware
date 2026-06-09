#include "io_canQueues.hpp"
io::queue<io::CanMsg, 250> vehicle_can_tx_queue("VehicleCanTXQueue");
io::queue<io::CanMsg, 128> charger_can_tx_queue("ChargerCanTXQueue");
io::queue<io::CanMsg, 128> can_rx_queue("CanRXQueue");
