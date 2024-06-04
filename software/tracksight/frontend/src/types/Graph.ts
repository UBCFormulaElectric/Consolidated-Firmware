export enum GraphType {
    LIVE = "live",
    HISTORICAL = "historical"
}
export interface GraphI {
    id: number
    timestamp: Date
    type: GraphType
}