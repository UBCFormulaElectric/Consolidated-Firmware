import { unsubscribeFromSignal } from "@/lib/api/signalSubscriptions";
import { useMutation } from "@tanstack/react-query";

const useUnsubscribeToSignal = () => {
  return useMutation({
    mutationFn: async (signalName: string) => {
      return unsubscribeFromSignal(signalName);
    },
  });
};

export default useUnsubscribeToSignal;
