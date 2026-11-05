import { SignalMetadata } from "@/lib/types/Signal";
import { WidgetData } from "@/lib/types/Widget";
import { widgetFromStored, widgetToStored } from "./WidgetLayoutContext";

const LAYOUT_FILE_KIND = "tracksight-layout";
const LAYOUT_FILE_VERSION = 1;
const LAYOUT_FILE_EXTENSION = ".tracksight-layout.json";

type LayoutFile = {
    name: string;
    widgets: WidgetData[];
};

export function serializeLayoutFile(layoutName: string, widgets: WidgetData[]): string {
    return JSON.stringify(
        {
            kind: LAYOUT_FILE_KIND,
            version: LAYOUT_FILE_VERSION,
            name: layoutName,
            exportedAt: new Date().toISOString(),
            widgets: widgets.map(widgetToStored),
        },
        null,
        2
    );
}

export function parseLayoutFile(fileContents: string): LayoutFile {
    let candidate: any;

    try {
        candidate = JSON.parse(fileContents);
    } catch {
        throw new Error("The file is not valid JSON.");
    }

    if (!candidate || typeof candidate !== "object" || candidate.kind !== LAYOUT_FILE_KIND) {
        throw new Error("The file is not a Tracksight layout export.");
    }

    if (candidate.version !== LAYOUT_FILE_VERSION) {
        throw new Error(`Layout file version ${candidate.version} is not supported (expected ${LAYOUT_FILE_VERSION}).`);
    }

    if (typeof candidate.name !== "string" || !candidate.name.trim()) {
        throw new Error("The layout file is missing a name.");
    }

    if (!Array.isArray(candidate.widgets)) {
        throw new Error("The layout file has no widget list.");
    }

    return { name: candidate.name.trim(), widgets: candidate.widgets.map(widgetFromStored) };
}

export function downloadLayoutFile(layoutName: string, widgets: WidgetData[]) {
    const blob = new Blob([serializeLayoutFile(layoutName, widgets)], { type: "application/json" });
    const url = URL.createObjectURL(blob);
    const link = document.createElement("a");

    link.href = url;
    link.download = `${layoutName.replace(/[^a-z0-9-_ ]/gi, "_")}${LAYOUT_FILE_EXTENSION}`;
    link.click();

    URL.revokeObjectURL(url);
}

export function findMissingSignalNames(widgets: WidgetData[], availableSignals: SignalMetadata[]): string[] {
    const availableSignalNames = new Set(availableSignals.map((signal) => signal.name));
    const layoutSignalNames = new Set(widgets.flatMap((widget) => widget.signals.map((signal) => signal.name)));

    return [...layoutSignalNames].filter((signalName) => !availableSignalNames.has(signalName));
}
