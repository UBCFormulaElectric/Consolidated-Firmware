import { useLocalState } from "@/lib/hooks/useLocalState";
import { WidgetData } from "@/lib/types/Widget";
import chroma from "chroma-js";
import { createContext, Dispatch, ReactNode, SetStateAction, useCallback, useContext, useMemo } from "react";
import { v4 as uuidv4 } from "uuid";

const LOCAL_STORAGE_KEY = "tracksight_widgets_config_v2";

type LayoutStore = {
    savedLayouts: Record<string, WidgetData[]>;
    activeLayouts: Record<string, string | null>;
    drafts: Record<string, WidgetData[]>;
};

const EMPTY_LAYOUT_STORE: LayoutStore = { savedLayouts: {}, activeLayouts: {}, drafts: {} };

export function widgetToStored(widget: WidgetData) {
    if (widget.type === "enumTimeline") {
        return {
            ...widget,
            options: {
                ...widget.options,
                colorPalette: Object.fromEntries(
                    Object.entries(widget.options.colorPalette).map(([signalName, enumColors]) => [
                        signalName,
                        {
                            color: enumColors.color.hex(),
                            enumValueColors: Object.fromEntries(Object.entries(enumColors.enumValueColors).map(([enumVal, color]) => [enumVal, color.hex()])),
                        },
                    ])
                ),
            },
        };
    }

    return {
        ...widget,
        options: {
            ...widget.options,
            colorPalette: Object.fromEntries(Object.entries(widget.options.colorPalette).map(([signalName, color]) => [signalName, color.hex()])),
        },
    };
}

export function widgetFromStored(widget: unknown): WidgetData {
    if (!widget || typeof widget !== "object" || !("type" in widget)) {
        throw new Error("Deserialized widget has invalid shape");
    }

    const candidate = widget as any;

    if (candidate.type === "enumTimeline") {
        return {
            id: candidate.id,
            type: "enumTimeline",
            data: candidate.data,
            name: candidate.name,
            signals: candidate.signals,
            options: {
                height: candidate.options.height,
                timeTickCount: candidate.options.timeTickCount,
                colorPalette: Object.fromEntries(
                    Object.entries(candidate.options.colorPalette).map(([signalName, palette]) => [
                        signalName,
                        {
                            color: chroma((palette as { color: string }).color),
                            enumValueColors: Object.fromEntries(Object.entries((palette as { enumValueColors: Record<string, string> }).enumValueColors).map(([enumVal, color]) => [enumVal, chroma(color)])),
                        },
                    ])
                ),
            },
        } satisfies WidgetData;
    }

    if (candidate.type === "numericalGraph") {
        return {
            id: candidate.id,
            name: candidate.name,
            type: "numericalGraph",
            signals: candidate.signals,
            data: candidate.data,
            options: {
                colorPalette: Object.fromEntries(Object.entries(candidate.options.colorPalette).map(([signalName, color]) => [signalName, chroma(color as string)])),
                height: candidate.options.height,
                timeTickCount: candidate.options.timeTickCount,
            },
        } satisfies WidgetData;
    }

    throw new Error(`Deserialized widget has invalid type: ${candidate.type}`);
}

function widgetListsToStored(widgetLists: Record<string, WidgetData[]>) {
    return Object.fromEntries(Object.entries(widgetLists).map(([key, widgets]) => [key, widgets.map(widgetToStored)]));
}

function widgetListsFromStored(widgetLists: unknown, fieldName: string): Record<string, WidgetData[]> {
    if (typeof widgetLists !== "object" || widgetLists === null) {
        throw new Error(`Deserialized layout config has invalid ${fieldName}`);
    }

    return Object.fromEntries(
        Object.entries(widgetLists).map(([key, widgets]) => {
            if (!Array.isArray(widgets)) {
                throw new Error(`Deserialized layout config has non-array widgets for ${fieldName}.${key}`);
            }

            return [key, widgets.map(widgetFromStored)];
        })
    );
}

function LayoutSerialize({ savedLayouts, activeLayouts, drafts }: LayoutStore): string {
    return JSON.stringify({
        savedLayouts: widgetListsToStored(savedLayouts),
        activeLayouts,
        drafts: widgetListsToStored(drafts),
    });
}

function LayoutDeserialize(layoutString: string): LayoutStore {
    const candidate = JSON.parse(layoutString) as any;

    if (!candidate || typeof candidate !== "object") {
        throw new Error("Deserialized layout config is not an object");
    }

    if (typeof candidate.activeLayouts !== "object" || candidate.activeLayouts === null) {
        throw new Error("Deserialized layout config has invalid activeLayouts");
    }

    return {
        savedLayouts: widgetListsFromStored(candidate.savedLayouts, "savedLayouts"),
        activeLayouts: candidate.activeLayouts,
        drafts: widgetListsFromStored(candidate.drafts, "drafts"),
    };
}

function areWidgetsEqual(first: WidgetData[], second: WidgetData[]): boolean {
    return JSON.stringify(first.map(widgetToStored)) === JSON.stringify(second.map(widgetToStored));
}

function getSavedWidgets(store: LayoutStore, pageKey: string): WidgetData[] {
    const activeLayoutName = store.activeLayouts[pageKey];

    if (!activeLayoutName) return [];

    return store.savedLayouts[activeLayoutName] ?? [];
}

function getCurrentWidgets(store: LayoutStore, pageKey: string): WidgetData[] {
    return store.drafts[pageKey] ?? getSavedWidgets(store, pageKey);
}

function setDraft(store: LayoutStore, pageKey: string, draft: WidgetData[] | null): LayoutStore {
    const { [pageKey]: _discardedDraft, ...otherDrafts } = store.drafts;

    if (draft === null || areWidgetsEqual(draft, getSavedWidgets(store, pageKey))) {
        return { ...store, drafts: otherDrafts };
    }

    return { ...store, drafts: { ...otherDrafts, [pageKey]: draft } };
}

function getNeighbouringLayoutName(store: LayoutStore, layoutName: string): string | null {
    const layoutNames = Object.keys(store.savedLayouts);
    const layoutIndex = layoutNames.indexOf(layoutName);

    return layoutNames[layoutIndex + 1] ?? layoutNames[layoutIndex - 1] ?? null;
}

function movePagesOffLayout(store: LayoutStore, layoutName: string): LayoutStore {
    const attachedPageKeys = Object.keys(store.activeLayouts).filter((pageKey) => store.activeLayouts[pageKey] === layoutName);
    const replacementLayoutName = getNeighbouringLayoutName(store, layoutName);

    return attachedPageKeys.reduce((nextStore, pageKey) => {
        const movedStore = { ...nextStore, activeLayouts: { ...nextStore.activeLayouts, [pageKey]: replacementLayoutName } };

        return setDraft(movedStore, pageKey, null);
    }, store);
}

function getDuplicateLayoutName(existingLayoutNames: string[], layoutName: string): string {
    const baseName = `${layoutName} copy`;

    if (!existingLayoutNames.includes(baseName)) return baseName;

    let copyNumber = 2;

    while (existingLayoutNames.includes(`${baseName} ${copyNumber}`)) {
        copyNumber++;
    }

    return `${baseName} ${copyNumber}`;
}

function saveLayout(store: LayoutStore, pageKey: string, layoutName: string, widgets: WidgetData[]): LayoutStore {
    const savedStore = {
        ...store,
        savedLayouts: { ...store.savedLayouts, [layoutName]: widgets },
        activeLayouts: { ...store.activeLayouts, [pageKey]: layoutName },
    };

    return setDraft(savedStore, pageKey, null);
}

interface WidgetLayoutContext {
    widgets: WidgetData[];
    setWidgets: Dispatch<SetStateAction<WidgetData[]>>;
    hasUnsavedChanges: boolean;
    revertChanges: () => void;

    activeLayoutName: string | null;
    layoutNames: string[];

    loadLayout: (layoutName: string) => void;
    saveCurrentLayout: (layoutName: string) => void;
    renameLayout: (oldName: string, newName: string) => void;
    deleteLayout: (layoutName: string) => void;

    duplicateLayout: (layoutName: string) => string;

    getSavedLayout: (layoutName: string) => WidgetData[] | undefined;
    importLayout: (layoutName: string, importedWidgets: WidgetData[]) => void;

    isInitialized: boolean;
}

const WidgetLayoutContext = createContext<WidgetLayoutContext | null>(null);

export function useWidgetLayout(): WidgetLayoutContext {
    const ctx = useContext(WidgetLayoutContext);

    if (!ctx) {
        throw new Error("useWidgetLayout must be used within a WidgetLayout");
    }

    return ctx;
}

export function WidgetLayout({ children, pageKey }: { children: ReactNode; pageKey: string }) {
    const [store, setStore, isInitialized] = useLocalState<LayoutStore>(LOCAL_STORAGE_KEY, EMPTY_LAYOUT_STORE, LayoutSerialize, LayoutDeserialize);

    const setWidgets: Dispatch<SetStateAction<WidgetData[]>> = useCallback(
        (action) => {
            setStore((previousStore) => {
                const currentWidgets = getCurrentWidgets(previousStore, pageKey);
                const nextWidgets = action instanceof Function ? action(currentWidgets) : action;

                if (nextWidgets === currentWidgets) return previousStore;

                return setDraft(previousStore, pageKey, nextWidgets);
            });
        },
        [setStore, pageKey]
    );

    const revertChanges = useCallback(() => {
        setStore((previousStore) => setDraft(previousStore, pageKey, null));
    }, [setStore, pageKey]);

    const loadLayout = useCallback(
        (layoutName: string) => {
            setStore((previousStore) => {
                const switchedStore = { ...previousStore, activeLayouts: { ...previousStore.activeLayouts, [pageKey]: layoutName } };

                return setDraft(switchedStore, pageKey, null);
            });
        },
        [setStore, pageKey]
    );

    const saveCurrentLayout = useCallback(
        (layoutName: string) => {
            setStore((previousStore) => saveLayout(previousStore, pageKey, layoutName, getCurrentWidgets(previousStore, pageKey)));
        },
        [setStore, pageKey]
    );

    const importLayout = useCallback(
        (layoutName: string, importedWidgets: WidgetData[]) => {
            setStore((previousStore) => saveLayout(previousStore, pageKey, layoutName, importedWidgets));
        },
        [setStore, pageKey]
    );

    const duplicateLayout = useCallback(
        (layoutName: string): string => {
            const duplicateName = getDuplicateLayoutName(Object.keys(store.savedLayouts), layoutName);

            setStore((previousStore) => {
                const sourceWidgets = previousStore.savedLayouts[layoutName];

                if (!sourceWidgets) return previousStore;

                const duplicatedWidgets = sourceWidgets.map((widget) => ({ ...widget, id: uuidv4() }));

                return { ...previousStore, savedLayouts: { ...previousStore.savedLayouts, [duplicateName]: duplicatedWidgets } };
            });

            return duplicateName;
        },
        [setStore, store.savedLayouts]
    );

    const getSavedLayout = useCallback((layoutName: string): WidgetData[] | undefined => store.savedLayouts[layoutName], [store.savedLayouts]);

    const renameLayout = useCallback(
        (oldName: string, newName: string) => {
            setStore((previousStore) => {
                if (oldName === newName || !(oldName in previousStore.savedLayouts)) return previousStore;

                const { [oldName]: renamedWidgets, ...otherLayouts } = previousStore.savedLayouts;

                return {
                    ...previousStore,
                    savedLayouts: { ...otherLayouts, [newName]: renamedWidgets },
                    activeLayouts: Object.fromEntries(Object.entries(previousStore.activeLayouts).map(([key, layoutName]) => [key, layoutName === oldName ? newName : layoutName])),
                };
            });
        },
        [setStore]
    );

    const deleteLayout = useCallback(
        (layoutName: string) => {
            setStore((previousStore) => {
                const movedStore = movePagesOffLayout(previousStore, layoutName);
                const { [layoutName]: _deletedWidgets, ...remainingLayouts } = movedStore.savedLayouts;

                return { ...movedStore, savedLayouts: remainingLayouts };
            });
        },
        [setStore]
    );

    const widgets = getCurrentWidgets(store, pageKey);
    const savedWidgets = getSavedWidgets(store, pageKey);
    const draft = store.drafts[pageKey];

    const hasUnsavedChanges = useMemo(() => draft !== undefined && !areWidgetsEqual(draft, savedWidgets), [draft, savedWidgets]);
    const layoutNames = useMemo(() => Object.keys(store.savedLayouts), [store.savedLayouts]);

    const value = useMemo(
        () => ({
            widgets,
            setWidgets,
            hasUnsavedChanges,
            revertChanges,
            activeLayoutName: store.activeLayouts[pageKey] ?? null,
            layoutNames,
            loadLayout,
            saveCurrentLayout,
            renameLayout,
            deleteLayout,
            duplicateLayout,
            getSavedLayout,
            importLayout,
            isInitialized,
        }),
        [widgets, setWidgets, hasUnsavedChanges, revertChanges, store.activeLayouts, pageKey, layoutNames, loadLayout, saveCurrentLayout, renameLayout, deleteLayout, duplicateLayout, getSavedLayout, importLayout, isInitialized]
    );

    return <WidgetLayoutContext.Provider value={value}>{children}</WidgetLayoutContext.Provider>;
}
