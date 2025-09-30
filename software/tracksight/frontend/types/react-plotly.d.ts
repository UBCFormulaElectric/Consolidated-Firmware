// Type declaration for react-plotly.js
declare module 'react-plotly.js' {
    import React from 'react';

    interface PlotParams {
        data: Array<any>;
        layout?: Object;
        config?: Object;
        frames?: Array<Object>;
        style?: Object;
        useResizeHandler?: boolean;
        revision?: number;
        onInitialized?: (figure: Object, graphDiv: HTMLElement) => void;
        onUpdate?: (figure: Object, graphDiv: HTMLElement) => void;
        onPurge?: (figure: Object, graphDiv: HTMLElement) => void;
        onError?: (err: Error) => void;
        onClickAnnotation?: (event: any) => void;
        onClick?: (event: any) => void;
        onDoubleClick?: (event: any) => void;
        onHover?: (event: any) => void;
        onRelayout?: (event: any) => void;
        onRestyle?: (event: any) => void;
        onSelected?: (event: any) => void;
        onSelecting?: (event: any) => void;
        onAnimation?: (event: any) => void;
        onAfterExport?: (event: any) => void;
        onAfterPlot?: (event: any) => void;
        onAnimated?: (event: any) => void;
        onAnimatingFrame?: (event: any) => void;
        onAnimationInterrupted?: (event: any) => void;
        onAutoSize?: (event: any) => void;
        onBeforeExport?: (event: any) => void;
        onButtonClicked?: (event: any) => void;
        onClickAnnotations?: (event: any) => void;
        onDeselect?: (event: any) => void;
        onFramework?: (event: any) => void;
        onLegendClick?: (event: any) => void;
        onLegendDoubleClick?: (event: any) => void;
        onRealyout?: (event: any) => void;
        onRedraw?: (event: any) => void;
        onTransitioning?: (event: any) => void;
        onTransitionInterrupted?: (event: any) => void;
        onUnhover?: (event: any) => void;
        onWebGlContextLost?: (event: any) => void;
        divId?: string;
        className?: string;
    }

    const Plot: React.ComponentType<PlotParams>;
    export default Plot;
}
