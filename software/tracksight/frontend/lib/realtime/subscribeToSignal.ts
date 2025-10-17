import { API_BASE_URL, IS_DEBUG } from '../constants';
import socket, { pendingSubscriptions } from './socket';

const subscribeToSignal = async (signalName: string) => {
  if (!socket.connected) {
    IS_DEBUG && console.log(
      `%c[Subscribe] %cSocket not connected, caching subscription for: ${signalName}`,
      'color: #ebcb8b; font-weight: bold;',
      'color: #d08770;'
    );
    
    pendingSubscriptions.add(signalName);
    return { cached: true };
  }

  const response = await fetch(`${API_BASE_URL}/subscribe`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({
      sid: socket.id,
      signal: signalName
    })
  });

  return response.json();
}

export default subscribeToSignal;

