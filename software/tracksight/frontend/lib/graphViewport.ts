export function scaleToFitRange(viewportWidth: number, rangeDuration: number, horizontalPadding: number): number {
    return Math.max(viewportWidth - horizontalPadding, 1) / Math.max(rangeDuration, 1);
}

export function scrollLeftToShowRange(globalStart: number, visibleStart: number, scale: number): number {
    return Math.max((visibleStart - globalStart) * scale, 0);
}
