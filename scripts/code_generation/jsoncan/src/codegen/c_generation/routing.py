from collections import deque
from dataclasses import dataclass
from re import M
from typing import Dict, List, Optional, Set, Tuple

from ...can_database import AllRxMsgs, BusForwarder, CanBus, CanDatabase, CanNode
from ...json_parsing.parse_error import InvalidCanJson


def _build_adj_list(
    forwarder_config: List[BusForwarder],
    nodes: Dict[str, CanNode],
    busses: Dict[str, CanBus],
) -> Dict[str, List[Tuple[str, str]]]:
    adj_list: Dict[str, List[Tuple[str, str]]] = {
        bus_name: [] for bus_name in busses.keys()
    }
    for forwarder_json in forwarder_config:
        forwarder_node_name = forwarder_json.forwarder
        if forwarder_node_name not in nodes:
            raise InvalidCanJson(
                f"Forwarder node '{forwarder_node_name}' is not defined in the node JSON."
            )
        bus1 = forwarder_json.bus1
        bus2 = forwarder_json.bus2
        if bus1 not in busses:
            raise InvalidCanJson(
                f"Forwarder bus '{bus1}' is not defined in the node JSON."
            )
        if bus2 not in busses:
            raise InvalidCanJson(
                f"Forwarder bus '{bus2}' is not defined in the node JSON."
            )
        adj_list[bus1].append((bus2, forwarder_node_name))
        adj_list[bus2].append((bus1, forwarder_node_name))
    return adj_list


def enforce_tree_fermat_little_rsa(
    adj_list: Dict[str, List[Tuple[str, str]]], busses: Dict[str, CanBus]
):
    visited_list: Dict[str, bool] = {bus_name: False for bus_name in busses.keys()}
    previous_node: Dict[str, Optional[str]] = {}
    queue: deque[str] = deque()

    # we can start at an arbitrary bus
    start_node = list(busses.keys())[0]
    previous_node[start_node] = None
    queue.append(start_node)

    while len(queue) > 0:
        cur_node = queue.popleft()
        if visited_list[cur_node]:
            raise InvalidCanJson(f"Forwarder topology is not a tree!")
        visited_list[cur_node] = True
        for next_node, edge in adj_list[cur_node]:
            if previous_node[cur_node] != next_node:
                previous_node[next_node] = cur_node
                queue.append(next_node)


def _fast_fourier_transform_stochastic_gradient_descent(
    adj_list: Dict[str, List[Tuple[str, str]]], tx_node: CanNode, rx_node: CanNode
) -> Tuple[str, str, List[Tuple[str, str, str]]]:
    # make all forwarders
    # OVERVIEW
    # what we gotta do is make a graph representation of canBuses
    # where buses are connected by edges which are boards
    # then we create start nodes -> buses connected with TX node
    # and destination nodes -> buses connected with RX node
    # and we run BFS, keeping track of three things
    # 1. shortest path to each bus (all destination nodes init as 0)
    # 2. previous bus to each bus on shortest path
    # 3. previous edge to each bus on shortest path
    # we recover the path we want by finding the destination node w/ the shortest distance
    # and backtrack
    # lastly to update our config we do the following
    # we make the first tx board forward to the first bus
    # for the buses/boards in order
    # we add a rerouter config for board i from {prev bus, next bus}
    # we make the last rx board take shit in from the last bus

    # graph representation of canBuses
    # im sorry this is about to be just tuples and shit
    # return (source_node, destination_node, [(rerouter, bus1, bus2)])
    previous_node: Dict[str, Optional[str]] = {}
    previous_edge: Dict[str, str] = {}
    destination_nodes: Set[str] = set()
    queue: deque[str] = deque()

    # use sorted to make the algorithm determined
    # populate w/ start nodes
    for bus_name in sorted(tx_node.bus_names):
        queue.append(bus_name)
        previous_node[bus_name] = None

    # population destination w/ end nodes
    for bus_name in sorted(rx_node.bus_names):
        destination_nodes.add(bus_name)

    # basic bfs
    target_node: Optional[str] = None

    max_search_distance = 1
    # only reroute once for efficiency
    while len(queue) > 0 and max_search_distance >= 0:
        max_search_distance = max_search_distance - 1
        cur_node = queue.popleft()
        if cur_node in destination_nodes:
            target_node = cur_node
            break
        for next_node, edge in adj_list[cur_node]:
            if next_node not in previous_node:
                previous_node[next_node] = cur_node
                previous_edge[next_node] = edge
                queue.append(next_node)

    # graph is disconnected
    if target_node is None:
        raise InvalidCanJson(
            f"Unable to navigate from {tx_node.name} to {rx_node.name} using the provided forwarder topology."
        )

    # recover path
    best_path: List[Tuple[str, Optional[str]]] = []
    while previous_node[target_node] is not None:
        best_path.append((target_node, previous_edge[target_node]))
        target_node = previous_node[target_node]
        if target_node in previous_node and target_node == previous_node[target_node]:
            raise InvalidCanJson(
                f"Unreachable CAN message, likely error in forwarder topology"
            )
    best_path.append((target_node, None))
    best_path.reverse()  # TODO not necessary, just interpret the list backwards

    # parse some stuff
    initial_node = best_path[0][0]
    final_node = best_path[-1][0]
    rerouter_nodes: List[Tuple[str, str, str]] = []
    for index in range(1, len(best_path)):
        rerouter_nodes.append(
            (best_path[index][1], best_path[index - 1][0], best_path[index][0])
        )

    return initial_node, final_node, rerouter_nodes


def _simple_fast_fourier_transform_stochastic_gradient_descent(
    bus_forwarder: List[BusForwarder], tx_node: CanNode, rx_node: CanNode
) -> Tuple[str, str, str | None]:
    # difference from the algorithm above is that we don't do a BFS
    # the idea is to only allow the message to be forwarded once
    # so we just need to pick one edge from the adj_list
    # return (source_node, destination_node, rerouter)

    source_nodes = sorted(tx_node.bus_names)
    destination_nodes = sorted(rx_node.bus_names)

    # if we have a direct connection, just return that
    for source_node in source_nodes:
        for destination_node in destination_nodes:
            if source_node == destination_node:
                # no forwarding needed
                return source_node, destination_node, None

    # if no direct connection, we need to find a forwarding node
    for bus_forwarder_json in sorted(bus_forwarder):
        forwarder_node_name = bus_forwarder_json.forwarder
        bus1 = bus_forwarder_json.bus1
        bus2 = bus_forwarder_json.bus2

        # check if the forwarder is connected to both the source and destination nodes
        if bus1 in source_nodes and bus2 in destination_nodes:
            return bus1, bus2, forwarder_node_name
        elif bus2 in source_nodes and bus1 in destination_nodes:
            return bus2, bus1, forwarder_node_name

    # if no rerouter found, raise an error
    raise InvalidCanJson(
        f"Unable to navigate from {tx_node.name} to {rx_node.name} using the provided forwarder topology."
    )


class CanTxConfig:
    # each message can be sent on many busses
    _map_by_msg_name: Dict[str, Set[str]]

    # query by msg_name -> set of busses to broadcast them on

    def __init__(self):
        self._map_by_msg_name = {}

    def add_tx_msg(self, msg_name: str):
        """
        Makes the struct aware of a new msg_name to be broadcast
        """
        self._map_by_msg_name[msg_name] = set()

    def add_bus_to_tx_msg(self, msg_name: str, tx_bus: str):
        """
        :param msg_name: msg_name (as added before)
        :param tx_bus: bus to broadcast on
        """
        self._map_by_msg_name[msg_name].add(tx_bus)

    def get_busses_for_msg(self, msg_name: str) -> List[str]:
        """
        Get all the busses that a message is broadcast on, on a certain node
        :param msg_name:
        :return:
        """
        return list(self._map_by_msg_name[msg_name])

    def get_all_msg_names(self):
        """
        Gets all messages which are broadcast by a certain node
        :return:
        """
        return list(self._map_by_msg_name.keys())

    def __eq__(self, other):
        if not isinstance(other, CanTxConfig):
            return False
        return self._map_by_msg_name == other._map_by_msg_name


class CanRxConfig:
    _map_by_bus: Dict[str, Set[str]]  # each bus can receive many messages
    # each message can only be received by one bus
    _map_by_msg_name: Dict[str, str]

    def __init__(self):
        self._map_by_bus = {}
        self._map_by_msg_name = {}

    def add_rx_bus(self, bus_name: str):
        assert bus_name not in self._map_by_bus, "Bus already exists"
        self._map_by_bus[bus_name] = set()

    def add_rx_msg(self, msg_name: str, rx_bus: str):
        # do this instead of figuring out how to remove them
        assert msg_name not in self._map_by_msg_name
        self._map_by_bus[rx_bus].add(msg_name)
        self._map_by_msg_name[msg_name] = rx_bus

    def get_msgs_on_bus(self, bus_name: str) -> List[str]:
        return list(self._map_by_bus[bus_name])

    def get_bus_of_msg(self, msg_name: str) -> str:
        return self._map_by_msg_name[msg_name]

    def get_all_rx_msgs_names(self) -> List[str]:
        return list(self._map_by_msg_name.keys())

    def contains_rx_msg(self, msg_name: str) -> bool:
        return msg_name in self._map_by_msg_name

    def empty_bus(self, bus_name: str) -> bool:
        return len(self._map_by_bus[bus_name]) == 0

    def __eq__(self, other):
        if not isinstance(other, CanRxConfig):
            return False
        return (
            self._map_by_bus == other._map_by_bus
            and self._map_by_msg_name == other._map_by_msg_name
        )


@dataclass()
class CanForward:
    message_name: str  # name of the message
    forwarder_name: str  # name of the forwarder node
    from_bus_name: str  # name of the bus the message is forwarded from
    to_bus_name: str  # name of the bus the message is forwarded to

    def __eq__(self, other):
        return (
            self.message_name == other.message_name
            and self.forwarder_name == other.forwarder_name
            and self.from_bus_name == other.from_bus_name
            and self.to_bus_name == other.to_bus_name
        )

    def __hash__(self):
        return hash(
            (
                self.message_name,
                self.forwarder_name,
                self.from_bus_name,
                self.to_bus_name,
            )
        )

    def __repr__(self):
        return f"CanForward({self.message_name} from {self.from_bus_name} to {self.to_bus_name} via {self.forwarder_name})"


def resolve_tx_rx_reroute(
    can_db: CanDatabase,
) -> Tuple[Dict[str, CanTxConfig], Dict[str, CanRxConfig], Dict[str, Set[CanForward]]]:
    reroute_configs: Dict[
        str, Set[CanForward]
    ] = {  # TODO consider making the inside keyed by from_bus
        forwarder_json.forwarder: set() for forwarder_json in can_db.forwarding
    }
    tx_configs = {node_name: CanTxConfig() for node_name in can_db.nodes.keys()}
    rx_configs = {node_name: CanRxConfig() for node_name in can_db.nodes.keys()}

    # we want to guarantee that our given topology is a tree
    # we can BFS on the final adjacency list and just make sure
    # there are no cycles
    adj_list = _build_adj_list(can_db.forwarding, can_db.nodes, can_db.buses)
    # don't need to enforce to be a tree
    # enforce_tree_fermat_little_rsa(adj_list, can_db.buses)

    for msg in can_db.msgs.values():
        tx_configs[msg.tx_node_name].add_tx_msg(msg.name)

    for rx_node in can_db.nodes.values():
        for rx_bus_name in rx_node.bus_names:
            rx_configs[rx_node.name].add_rx_bus(rx_bus_name)

        if type(rx_node.rx_msgs_names) == AllRxMsgs:
            # TODO generate a new function to do this? This is a nontrivial usage of state??
            #    but also it is very specific to this application
            rx_msgs_names = set(
                [
                    msg.name
                    for msg in can_db.msgs.values()
                    if msg.tx_node_name != rx_node.name
                ]
            )
        else:
            rx_msgs_names = rx_node.rx_msgs_names
        for msg_name in rx_msgs_names:
            tx_node = can_db.nodes[can_db.msgs[msg_name].tx_node_name]

            # calculate the reroute
            # initial_node_tx_bus, final_node_rx_bus, rerouter_nodes = _fast_fourier_transform_stochastic_gradient_descent(
            #     adj_list, tx_node, rx_node)
            (
                initial_node_tx_bus,
                final_node_rx_bus,
                reroute_node,
            ) = _simple_fast_fourier_transform_stochastic_gradient_descent(
                can_db.forwarding, tx_node, rx_node
            )
            # process the calculation
            tx_configs[tx_node.name].add_bus_to_tx_msg(msg_name, initial_node_tx_bus)
            rx_configs[rx_node.name].add_rx_msg(msg_name, final_node_rx_bus)

            if reroute_node is not None:
                reroute_configs[reroute_node].add(
                    CanForward(
                        message_name=msg_name,
                        forwarder_name=reroute_node,
                        from_bus_name=initial_node_tx_bus,
                        to_bus_name=final_node_rx_bus,
                    )
                )
    return tx_configs, rx_configs, reroute_configs


# TODO have some check for tx_msgs without rx_msgs??
