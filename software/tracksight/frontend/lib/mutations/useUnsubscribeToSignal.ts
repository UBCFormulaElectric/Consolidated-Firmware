import { unsubscribeFromSignal } from "@/lib/api/signalSubscriptions";
import socket from "@/lib/realtime/socket";
import { useMutation } from "@tanstack/react-query";

const useUnsubscribeToSignal = () => {
  return useMutation({
    mutationFn: async (signalName: string) => {
      while (!socket.connected) {
        await new Promise((resolve) => setTimeout(resolve, 100));
      }

      return unsubscribeFromSignal(signalName);
    },
  });
};

export default useUnsubscribeToSignal;
