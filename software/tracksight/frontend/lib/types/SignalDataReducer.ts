import { ParsedSignalPayload } from "@/lib/signalData";

/**
 * Reducer function for processing incoming signal data. The previous data
 * should not be recreated instead mutated for performance reasons.
 */
type SignalDataReducer<T> = (newData: ParsedSignalPayload, prev: T) => T;

export default SignalDataReducer;
