export function scaleToFitRange(viewportWidth: number, rangeDuration: number, horizontalPadding: number): number {
    return Math.max(viewportWidth - horizontalPadding, 1) / Math.max(rangeDuration, 1);
}
