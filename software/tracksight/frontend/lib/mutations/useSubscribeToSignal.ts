import { subscribeToSignal } from "@/lib/api/signalSubscriptions";
import socket from "@/lib/realtime/socket";
import { useMutation } from "@tanstack/react-query";

const useSubscribeToSignal = () => {
  return useMutation({
    mutationFn: async (signalName: string) => {
      while (!socket.connected) {
        await new Promise((resolve) => setTimeout(resolve, 100));
      }

      return subscribeToSignal(signalName);
    },
  });
};

export default useSubscribeToSignal;
