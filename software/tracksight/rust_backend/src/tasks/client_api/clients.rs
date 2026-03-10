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
     * Retrieves a list of clients
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
            if let Err(e) = client_socket.disconnect() {
                eprintln!("Socket error while disconnecting {}'s socket: {}", client, e);
            };
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

    pub fn get_clients_of_signal(&self, signal: &String) -> Vec<String> {
        return self.signal_to_client
            .get(signal)
            .map(|clients| clients.iter().cloned().collect())
            .unwrap_or_default();
    }

    pub fn get_signals_of_client(&self, client: &String) -> Vec<String> {
        return self.client_to_signal
            .get(client)
            .map(|signals| signals.iter().cloned().collect())
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
