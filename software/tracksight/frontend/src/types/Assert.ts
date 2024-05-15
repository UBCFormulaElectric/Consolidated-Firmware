export function assertType<T>(val:unknown, assertion: boolean, err_msg?: string): asserts val is T {
    if(!assertion) throw new TypeError(err_msg ?? "Value is not the correct type");
}