const isEnumSignal = (signal: any): signal is { enum: Record<string, string | number> } => {
    return signal
        && typeof signal === 'object'
        && signal.enum
        && typeof signal.enum === 'object'
}

export default isEnumSignal;
