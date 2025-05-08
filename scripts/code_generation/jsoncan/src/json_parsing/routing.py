from collections import deque
from typing import List, Dict, Tuple, Optional, Set
from ..can_database import CanNode, CanBus
from .parse_error import InvalidCanJson
from .parse_bus import ForwarderConfigJson


def build_adj_list(forwarder_config: List[ForwarderConfigJson], nodes: Dict[str, CanNode],
                   busses: Dict[str, CanBus]) -> Dict[str, List[Tuple[str, str]]]:
    adj_list: Dict[str, List[Tuple[str, str]]] = {
        bus_name: []
        for bus_name in busses.keys()
    }
    for forwarder_json in forwarder_config:
        forwarder_node_name = forwarder_json["forwarder"]
        if forwarder_node_name not in nodes:
            raise InvalidCanJson(f"Forwarder node '{forwarder_node_name}' is not defined in the node JSON.")
        bus1 = forwarder_json["bus1"]
        bus2 = forwarder_json["bus2"]
        if bus1 not in busses:
            raise InvalidCanJson(f"Forwarder bus '{bus1}' is not defined in the node JSON.")
        if bus2 not in busses:
            raise InvalidCanJson(f"Forwarder bus '{bus2}' is not defined in the node JSON.")
        adj_list[bus1].append((bus2, forwarder_node_name))
        adj_list[bus2].append((bus1, forwarder_node_name))
    return adj_list


def fast_fourier_transform_stochastic_gradient_descent(adj_list: Dict[str, List[Tuple[str, str]]],
                                                       tx_node: CanNode,
                                                       rx_node: CanNode) -> \
        Tuple[str, str, List[Tuple[str, str, str]]]:
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
    previous_node: Dict[str, Optional[str]] = {}
    previous_edge: Dict[str, str] = {}
    destination_nodes: Set[str] = set()
    queue: deque[str] = deque()

    # populate w/ start nodes
    for bus_name in tx_node.bus_names:
        queue.append(bus_name)
        previous_node[bus_name] = None

    # population destination w/ end nodes
    for bus_name in rx_node.bus_names:
        destination_nodes.add(bus_name)

    # basic bfs
    target_node: Optional[str] = None
    while len(queue) > 0:
        cur_node = queue.popleft()
        if cur_node in destination_nodes:
            target_node = cur_node
            break
        for (next_node, edge) in adj_list[cur_node]:
            if next_node not in previous_node:
                previous_node[next_node] = cur_node
                previous_edge[next_node] = edge
                queue.append(next_node)

    # graph is disconnected
    if target_node is None:
        raise InvalidCanJson(
            f"Unable to navigate from {tx_node.name} to {rx_node.name} using the provided forwarder topology.")

    # recover path
    best_path: List[Tuple[str, Optional[str]]] = []
    while previous_node[target_node] is not None:
        best_path.append((target_node, previous_edge[target_node]))
        target_node = previous_node[target_node]
        if target_node in previous_node and target_node == previous_node[target_node]:
            raise InvalidCanJson(f"Unreachable CAN message, likely error in forwarder topology")
    best_path.append((target_node, None))
    best_path.reverse()  # TODO not necessary, just interpret the list backwards

    # if counter == 0:
    #     print(previous_node)
    #     print(previous_edge)
    #     print(tx_node)
    #     print(rx_node)
    #     print(adj_list)
    #     raise InvalidCanJson(f"Unreachable CAN message, likely error in forwarder topology")

    # parse some stuff
    initial_node = best_path[0][0]
    final_node = best_path[-1][0]
    rerouter_nodes: List[Tuple[str, str, str]] = []
    for index in range(1, len(best_path)):
        rerouter_nodes.append((best_path[index][1], best_path[index - 1][0], best_path[index][0]))

    return initial_node, final_node, rerouter_nodes
