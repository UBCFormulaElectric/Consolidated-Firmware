import { SignalMetadata, SignalType, EnumSignalMetadata } from "../types/Signal";

const isMetadataEnum = (metadata: Omit<SignalMetadata, "type">): metadata is Omit<EnumSignalMetadata, "type"> => {
  return "enum" in metadata;
}

const isMetadataNumerical = (metadata: Omit<SignalMetadata, "type">): metadata is Omit<EnumSignalMetadata, "type"> => {
  return "min_val" in metadata && "max_val" in metadata && !(metadata.min_val === 0 && metadata.max_val === 1);
}

const getSignalMetadataType = (metadata: Omit<SignalMetadata, "type">) => {
  if (isMetadataEnum(metadata)) {
    return SignalType.ENUM;
  } else if (isMetadataNumerical(metadata)) {
    return SignalType.NUMERICAL;
  }

  return SignalType.ALERT;
}

export default getSignalMetadataType;
