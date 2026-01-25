use std::collections::{HashMap, HashSet};

use socketioxide::extract::SocketRef;

// Clients, their respective sockets, and their set of signals subscribed
pub struct Clients {
    sockets: HashMap<String, SocketRef>,

    // client -> signal
    client_to_signal: HashMap<String, HashSet<String>>,
    // Signals and its subscribed clients
    // signal -> client
    signal_to_client: HashMap<String, HashSet<String>>,
}

impl Clients {
    pub fn new() -> Self {
        return Self { 
            sockets: HashMap::new(),
            client_to_signal: HashMap::new(), 
            signal_to_client: HashMap::new() 
        };
    }

    /**
     * Add client to connected sockets
     */
    pub fn add_client(&mut self, client: &String, socket_ref: &SocketRef) {
        self.sockets.insert(client.clone(), socket_ref.clone());
    }

    /**
     * TODO temp function, shouldn't be needed
     */
    pub fn get_clients(&self) -> Vec<&String> {
        return self.sockets.keys().collect();
    }

    pub fn get_client_socket(&self, client: &String) -> Option<&SocketRef> {
        return self.sockets.get(client);
    }

    /**
     * Remove client from connected sockets and all signals
     */
    pub fn remove_client(&mut self, client: &String) {
        if let Some(client_socket) = self.sockets.remove(client) {
            client_socket.disconnect();
        }

        if let Some(client_signals) = self.client_to_signal.get(client) {
            for signal in client_signals {
                if let Some(signal_clients) = self.signal_to_client.get_mut(signal) {
                    signal_clients.remove(client);
                }
            };

            self.client_to_signal.remove(client);
        }
    }

    /**
     * Subscribe client to signal
     */
    pub fn subscribe_client_to_signal(&mut self, client: &String, signal: &String) {
        if !self.sockets.contains_key(client) {
            panic!("Client isn't connected!");
        }

        // check if client exists, and add
        if let Some(client_signals) = self.client_to_signal.get_mut(client) {
            client_signals.insert(signal.clone());
        } else {
            let mut new_client_signals = HashSet::new();
            new_client_signals.insert(signal.clone());
            self.client_to_signal.insert(client.clone(), new_client_signals);
        }
        
        // check if signal exists and add signal
        if let Some(signal_clients) = self.signal_to_client.get_mut(signal) {
            signal_clients.insert(client.clone());
        } else {
            let mut new_signal_clients = HashSet::new();
            new_signal_clients.insert(client.clone());
            self.signal_to_client.insert(signal.clone(), new_signal_clients);
        }
    }

    /**
     * Remove client from signal
     */
    pub fn unsubscribe_client_from_signal(&mut self, client: &String, signal: &String) {
        // check if client exists
        if let Some(client_signals) = self.client_to_signal.get_mut(client) {
            client_signals.remove(signal);
        }
        
        // check if signal exists and add signal
        if let Some(signal_clients) = self.signal_to_client.get_mut(signal) {
            signal_clients.remove(client);
        }
    }

    pub fn get_clients_of_signal(&self, signal: &String) -> Vec<&String> {
        return self.signal_to_client
            .get(signal)
            .map(|clients| clients.iter().collect())
            .unwrap_or_default();
    }

    /**
     * Check if client is subscribed to signal
     */
    pub fn is_client_subscribed_to(&self, client: &String, signal: &String) -> bool {
        self.client_to_signal
            .get(client)
            .map_or(false, |signals| signals.contains(signal))
    }

    /**
     * Check if signal is subscribed to by client
     */
    pub fn is_signal_subscribed_by(&self, signal: &String, client: &String) -> bool {
        self.signal_to_client
            .get(signal)
            .map_or(false, |clients| clients.contains(client))
    }
}

// /**
//  * Unit tests, not working
//  */
// #[cfg(test)]
// mod tests {
//     use super::*;

//     #[test]
//     fn test_subscribe_client_to_signal() {
//         let mut subs = Clients::new();
//         subs.subscribe_client_to_signal("client1".to_string(), "signal1".to_string());
        
//         assert!(subs.is_client_subscribed_to("client1".to_string(), "signal1".to_string()));
//         assert!(subs.is_signal_subscribed_by("signal1".to_string(), "client1".to_string()));
//     }

//     #[test]
//     fn test_subscribe_multiple_signals_to_client() {
//         let mut subs = Clients::new();
//         subs.subscribe_client_to_signal("client1".to_string(), "signal1".to_string());
//         subs.subscribe_client_to_signal("client1".to_string(), "signal2".to_string());
        
//         assert!(subs.is_client_subscribed_to("client1".to_string(), "signal1".to_string()));
//         assert!(subs.is_client_subscribed_to("client1".to_string(), "signal2".to_string()));
//     }

//     #[test]
//     fn test_subscribe_multiple_clients_to_signal() {
//         let mut subs = Clients::new();
//         subs.subscribe_client_to_signal("client1".to_string(), "signal1".to_string());
//         subs.subscribe_client_to_signal("client2".to_string(), "signal1".to_string());
        
//         let clients = subs.get_clients_of_signal("signal1".to_string());
//         assert_eq!(clients.len(), 2);
//         assert!(clients.contains(&&"client1".to_string()));
//         assert!(clients.contains(&&"client2".to_string()));
//     }

//     #[test]
//     fn test_unsubscribe_client_from_signal() {
//         let mut subs = Clients::new();
//         subs.subscribe_client_to_signal("client1".to_string(), "signal1".to_string());
//         subs.unsubscribe_client_from_signal("client1".to_string(), "signal1".to_string());
        
//         assert!(!subs.is_client_subscribed_to("client1".to_string(), "signal1".to_string()));
//         assert!(!subs.is_signal_subscribed_by("signal1".to_string(), "client1".to_string()));
//     }

//     #[test]
//     fn test_unsubscribe_removes_empty_entries() {
//         let mut subs = Clients::new();
//         subs.subscribe_client_to_signal("client1".to_string(), "signal1".to_string());
//         subs.unsubscribe_client_from_signal("client1".to_string(), "signal1".to_string());
        
//         assert!(subs.get_clients_of_signal("signal1".to_string()).is_empty());
//     }

//     #[test]
//     fn test_unsubscribe_one_of_multiple_signals() {
//         let mut subs = Clients::new();
//         subs.subscribe_client_to_signal("client1".to_string(), "signal1".to_string());
//         subs.subscribe_client_to_signal("client1".to_string(), "signal2".to_string());
//         subs.unsubscribe_client_from_signal("client1".to_string(), "signal1".to_string());
        
//         assert!(!subs.is_client_subscribed_to("client1".to_string(), "signal1".to_string()));
//         assert!(subs.is_client_subscribed_to("client1".to_string(), "signal2".to_string()));
//     }

//     #[test]
//     fn test_get_clients_of_nonexistent_signal() {
//         let subs = Clients::new();
//         let clients = subs.get_clients_of_signal("nonexistent".to_string());
//         assert!(clients.is_empty());
//     }

//     #[test]
//     fn test_double_subscribe_same_client_signal() {
//         let mut subs = Clients::new();
//         subs.subscribe_client_to_signal("client1".to_string(), "signal1".to_string());
//         subs.subscribe_client_to_signal("client1".to_string(), "signal1".to_string());
        
//         let clients = subs.get_clients_of_signal("signal1".to_string());
//         assert_eq!(clients.len(), 1);
//     }

//     #[test]
//     fn test_unsubscribe_client_from_all_signals() {
//         let mut subs = Clients::new();
//         subs.subscribe_client_to_signal("client1".to_string(), "signal1".to_string());
//         subs.subscribe_client_to_signal("client1".to_string(), "signal2".to_string());
//         subs.subscribe_client_to_signal("client1".to_string(), "signal3".to_string());
        
//         subs.remove_client("client1".to_string());
        
//         assert!(!subs.is_client_subscribed_to("client1".to_string(), "signal1".to_string()));
//         assert!(!subs.is_client_subscribed_to("client1".to_string(), "signal2".to_string()));
//         assert!(!subs.is_client_subscribed_to("client1".to_string(), "signal3".to_string()));
//     }
    
//     #[test]
//     fn test_unsubscribe_client_removes_from_all_signal_maps() {
//         let mut subs = Clients::new();
//         subs.subscribe_client_to_signal("client1".to_string(), "signal1".to_string());
//         subs.subscribe_client_to_signal("client1".to_string(), "signal2".to_string());
//         subs.subscribe_client_to_signal("client2".to_string(), "signal1".to_string());
        
//         subs.remove_client("client1".to_string());
        
//         assert!(subs.get_clients_of_signal("signal1".to_string()).len() == 1);
//         assert!(subs.get_clients_of_signal("signal2".to_string()).is_empty());
//     }
    
//     #[test]
//     fn test_unsubscribe_nonexistent_client() {
//         let mut subs = Clients::new();
//         subs.subscribe_client_to_signal("client1".to_string(), "signal1".to_string());
        
//         subs.remove_client("nonexistent".to_string());
        
//         assert!(subs.is_client_subscribed_to("client1".to_string(), "signal1".to_string()));
//     }
// }
