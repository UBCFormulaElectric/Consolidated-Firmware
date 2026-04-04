import { subscribeToSignal } from "@/lib/api/signalSubscriptions";
import { useMutation } from "@tanstack/react-query";

const useSubscribeToSignal = () => {
  return useMutation({
    mutationFn: async (signalName: string) => {
      return await subscribeToSignal(signalName);
    },
  });
};

export default useSubscribeToSignal;
