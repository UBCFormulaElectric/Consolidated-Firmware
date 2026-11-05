"use client";

import { Check, ChevronDown, Copy, Download, LayoutDashboard, LucideIcon, MoreHorizontal, Pencil, Trash2, Upload } from "lucide-react";
import { ChangeEvent, useRef, useState } from "react";

import { Dialog, DialogClose, DialogContent, DialogDescription, DialogFooter, DialogHeader, DialogTitle } from "@/components/ui/dialog";
import { Popover, PopoverContent, PopoverTrigger } from "@/components/ui/popover";
import { downloadLayoutFile, findMissingSignalNames, parseLayoutFile } from "@/components/widgets/layoutFile";
import { useWidgetLayout } from "@/components/widgets/WidgetLayoutContext";
import { useToast } from "@/lib/contexts/ToastContext";
import { useAvailableSignals } from "@/lib/hooks/useAvailableSignals";
import { WidgetData } from "@/lib/types/Widget";
import { cn } from "@/lib/utils";

const UNSAVED_LAYOUT_LABEL = "Unsaved";
const MISSING_SIGNAL_PREVIEW_COUNT = 3;

function describeMissingSignals(missingSignalNames: string[]): string {
    const previewedNames = missingSignalNames.slice(0, MISSING_SIGNAL_PREVIEW_COUNT).join(", ");
    const remainingCount = missingSignalNames.length - MISSING_SIGNAL_PREVIEW_COUNT;
    const signalCountLabel = missingSignalNames.length === 1 ? "1 signal isn't" : `${missingSignalNames.length} signals aren't`;

    return `${signalCountLabel} in the current catalog: ${previewedNames}${remainingCount > 0 ? ` and ${remainingCount} more` : ""}.`;
}

type Confirmation = {
    title: string;
    description: string;

    confirmLabel: string;
    onConfirm: () => void;
};

function LayoutNameInput({ initialName, onSubmit }: { initialName: string; onSubmit: (layoutName: string) => void }) {
    const [layoutName, setLayoutName] = useState(initialName);
    const trimmedLayoutName = layoutName.trim();

    const handleSubmit = (event: ChangeEvent) => {
        event.preventDefault();

        if (!trimmedLayoutName) return;

        onSubmit(trimmedLayoutName);
    };

    return (
        <form onSubmit={handleSubmit} className="flex flex-1 items-center gap-1">
            <input autoFocus value={layoutName} onChange={(event) => setLayoutName(event.target.value)} placeholder="Layout name" className="min-w-0 flex-1 rounded border border-gray-300 px-2 py-1 text-sm outline-none focus:border-blue-500" />
            <button type="submit" disabled={!trimmedLayoutName} aria-label="Confirm layout name" className="rounded p-1.5 text-gray-600 transition-colors hover:cursor-pointer hover:bg-gray-100 disabled:cursor-not-allowed disabled:opacity-40">
                <Check className="size-4" strokeWidth={2.5} />
            </button>
        </form>
    );
}

type LayoutRowAction = {
    label: string;
    icon: LucideIcon;
    onSelect: () => void;
    isDestructive?: boolean;
};

function LayoutRowMenu({ layoutName, actions }: { layoutName: string; actions: LayoutRowAction[] }) {
    const [isOpen, setIsOpen] = useState(false);

    const handleSelect = (action: LayoutRowAction) => {
        setIsOpen(false);
        action.onSelect();
    };

    return (
        <Popover open={isOpen} onOpenChange={setIsOpen}>
            <PopoverTrigger asChild>
                <button type="button" aria-label={`Actions for ${layoutName}`} className="mr-1 rounded p-1.5 text-gray-500 opacity-0 transition-opacity hover:cursor-pointer hover:text-gray-900 group-hover:opacity-100 focus:opacity-100 data-[state=open]:opacity-100">
                    <MoreHorizontal className="size-4" />
                </button>
            </PopoverTrigger>

            <PopoverContent className="z-50 w-40 rounded-md border border-black bg-white p-1 shadow-lg" align="start" side="right" sideOffset={4} onCloseAutoFocus={(event) => event.preventDefault()}>
                <div className="flex flex-col">
                    {actions.map((action) => (
                        <button key={action.label} type="button" onClick={() => handleSelect(action)} className={cn("flex items-center gap-2 rounded px-2 py-1.5 text-left text-sm transition-colors hover:cursor-pointer", action.isDestructive ? "text-red-600 hover:bg-red-50" : "text-gray-800 hover:bg-gray-100")}>
                            <action.icon className="size-3.5" />
                            {action.label}
                        </button>
                    ))}
                </div>
            </PopoverContent>
        </Popover>
    );
}

function ConfirmationDialog({ confirmation, onClose }: { confirmation: Confirmation | null; onClose: () => void }) {
    const handleConfirm = () => {
        confirmation?.onConfirm();
        onClose();
    };

    return (
        <Dialog open={confirmation !== null} onOpenChange={(isOpen) => !isOpen && onClose()}>
            <DialogContent className="max-w-md bg-white">
                <DialogHeader>
                    <DialogTitle>{confirmation?.title}</DialogTitle>
                    <DialogDescription className="text-gray-700">{confirmation?.description}</DialogDescription>
                </DialogHeader>
                <DialogFooter className="gap-2">
                    <DialogClose asChild>
                        <button type="button" className="rounded border border-gray-300 px-3 py-1.5 text-sm font-semibold text-gray-800 transition-colors hover:cursor-pointer hover:bg-gray-100">
                            Cancel
                        </button>
                    </DialogClose>
                    <button type="button" onClick={handleConfirm} className="rounded bg-red-600 px-3 py-1.5 text-sm font-semibold text-white transition-colors hover:cursor-pointer hover:bg-red-700">
                        {confirmation?.confirmLabel}
                    </button>
                </DialogFooter>
            </DialogContent>
        </Dialog>
    );
}

const LayoutManager = () => {
    const { activeLayoutName, layoutNames, hasUnsavedChanges, loadLayout, saveCurrentLayout, renameLayout, deleteLayout, duplicateLayout, revertChanges, getSavedLayout, importLayout } = useWidgetLayout();
    const { notify } = useToast();
    const { data: availableSignals } = useAvailableSignals();

    const importInputRef = useRef<HTMLInputElement>(null);

    const [isOpen, setIsOpen] = useState(false);
    const [renamingLayoutName, setRenamingLayoutName] = useState<string | null>(null);
    const [isNamingNewLayout, setIsNamingNewLayout] = useState(false);
    const [confirmation, setConfirmation] = useState<Confirmation | null>(null);

    const handleOpenChange = (nextIsOpen: boolean) => {
        setIsOpen(nextIsOpen);

        if (nextIsOpen) return;

        setRenamingLayoutName(null);
        setIsNamingNewLayout(false);
    };

    const requestConfirmation = (nextConfirmation: Confirmation) => {
        handleOpenChange(false);
        setConfirmation(nextConfirmation);
    };

    const confirmIfNameTaken = (layoutName: string, action: () => void) => {
        if (!layoutNames.includes(layoutName)) {
            action();
            return;
        }

        requestConfirmation({
            title: `Overwrite "${layoutName}"?`,
            description: `A layout named "${layoutName}" already exists. It will be replaced on every page that uses it.`,
            confirmLabel: "Overwrite",
            onConfirm: action,
        });
    };

    const handleSelectLayout = (layoutName: string) => {
        if (layoutName === activeLayoutName && !hasUnsavedChanges) {
            handleOpenChange(false);
            return;
        }

        if (!hasUnsavedChanges) {
            loadLayout(layoutName);
            handleOpenChange(false);
            return;
        }

        requestConfirmation({
            title: "Discard unsaved changes?",
            description: `Your changes to "${activeLayoutName ?? UNSAVED_LAYOUT_LABEL}" will be lost when loading "${layoutName}".`,
            confirmLabel: "Discard",
            onConfirm: () => loadLayout(layoutName),
        });
    };

    const handleRename = (oldName: string, newName: string) => {
        setRenamingLayoutName(null);

        if (newName === oldName) return;

        confirmIfNameTaken(newName, () => renameLayout(oldName, newName));
    };

    const handleDelete = (layoutName: string) => {
        requestConfirmation({
            title: `Delete "${layoutName}"?`,
            description: "Pages using this layout will switch to the next one and lose any unsaved changes.",
            confirmLabel: "Delete",
            onConfirm: () => deleteLayout(layoutName),
        });
    };

    const handleDuplicate = (layoutName: string) => {
        setRenamingLayoutName(duplicateLayout(layoutName));
    };

    const handleSave = () => {
        if (!activeLayoutName) {
            setIsNamingNewLayout(true);
            return;
        }

        saveCurrentLayout(activeLayoutName);
        handleOpenChange(false);
    };

    const handleSaveNewLayout = (layoutName: string) => {
        handleOpenChange(false);
        confirmIfNameTaken(layoutName, () => saveCurrentLayout(layoutName));
    };

    const handleRevert = () => {
        revertChanges();
        handleOpenChange(false);
    };

    const handleExport = (layoutName: string) => {
        const savedWidgets = getSavedLayout(layoutName);

        if (!savedWidgets) {
            notify("Export failed", `Layout "${layoutName}" no longer exists.`, "error");
            return;
        }

        downloadLayoutFile(layoutName, savedWidgets);
    };

    const applyImport = (layoutName: string, importedWidgets: WidgetData[]) => {
        importLayout(layoutName, importedWidgets);

        const missingSignalNames = availableSignals ? findMissingSignalNames(importedWidgets, availableSignals) : [];

        if (missingSignalNames.length === 0) {
            notify("Layout imported", `"${layoutName}" is now loaded.`, "success");
            return;
        }

        console.warn(`Imported layout "${layoutName}" references signals missing from the current catalog:`, missingSignalNames);
        notify(`Imported "${layoutName}"`, describeMissingSignals(missingSignalNames), "info");
    };

    const handleImportFile = async (event: ChangeEvent<HTMLInputElement>) => {
        const file = event.target.files?.[0];

        event.target.value = "";

        if (!file) return;

        handleOpenChange(false);

        let layoutFile: ReturnType<typeof parseLayoutFile>;

        try {
            layoutFile = parseLayoutFile(await file.text());
        } catch (error) {
            notify("Import failed", `${file.name}: ${error instanceof Error ? error.message : String(error)}`, "error");
            return;
        }

        const { name: layoutName, widgets: importedWidgets } = layoutFile;
        const importConsequences: string[] = [];

        if (layoutNames.includes(layoutName)) {
            importConsequences.push(`A layout named "${layoutName}" already exists and will be replaced on every page that uses it.`);
        }

        if (hasUnsavedChanges) {
            importConsequences.push(`Your unsaved changes to "${activeLayoutName ?? UNSAVED_LAYOUT_LABEL}" will be lost.`);
        }

        if (importConsequences.length === 0) {
            applyImport(layoutName, importedWidgets);
            return;
        }

        requestConfirmation({
            title: `Import "${layoutName}"?`,
            description: importConsequences.join(" "),
            confirmLabel: "Import",
            onConfirm: () => applyImport(layoutName, importedWidgets),
        });
    };

    return (
        <>
            <Popover open={isOpen} onOpenChange={handleOpenChange}>
                <PopoverTrigger asChild>
                    <button type="button" className="flex h-8 items-center gap-2 rounded border border-black bg-white py-1.5 pl-3 pr-2 text-left transition-colors hover:cursor-pointer hover:border-black/75">
                        <span className="flex items-center gap-1.5 text-[0.6rem] font-semibold uppercase tracking-wide text-gray-500">
                            <LayoutDashboard className="size-3" />
                            Layout
                        </span>
                        <span className={cn("max-w-44 truncate text-sm font-semibold", activeLayoutName ? "text-gray-900" : "italic text-gray-500")}>{activeLayoutName ?? UNSAVED_LAYOUT_LABEL}</span>
                        {hasUnsavedChanges && <span className="size-1.5 shrink-0 rounded-full bg-blue-600" aria-label="Unsaved changes" />}
                        <ChevronDown className="size-4 shrink-0 text-gray-500" strokeWidth={2.25} />
                    </button>
                </PopoverTrigger>
                <PopoverContent className="z-50 w-72 rounded-md border border-black bg-white p-1 shadow-lg" align="end" sideOffset={4}>
                    <div className="mb-1 flex items-center justify-between border-b border-gray-200 pb-1 pl-3">
                        <span className="text-[0.6rem] font-semibold uppercase tracking-wide text-gray-500">Layouts</span>
                        <button type="button" onClick={() => importInputRef.current?.click()} className="flex items-center gap-1.5 rounded px-2 py-1 text-xs font-semibold text-gray-700 transition-colors hover:cursor-pointer hover:bg-gray-100">
                            <Upload className="size-3.5" />
                            Import
                        </button>
                    </div>

                    <div className="flex max-h-72 flex-col overflow-y-auto">
                        {layoutNames.length === 0 && <p className="px-3 py-2 text-sm text-gray-500">No saved layouts yet.</p>}

                        {layoutNames.map((layoutName) => {
                            const isActive = layoutName === activeLayoutName;

                            if (layoutName === renamingLayoutName) {
                                return (
                                    <div key={layoutName} className="flex px-1 py-1">
                                        <LayoutNameInput initialName={layoutName} onSubmit={(newName) => handleRename(layoutName, newName)} />
                                    </div>
                                );
                            }

                            return (
                                <div key={layoutName} className={cn("group flex items-center rounded", isActive ? "bg-blue-100" : "hover:bg-gray-100")}>
                                    <button type="button" onClick={() => handleSelectLayout(layoutName)} className={cn("flex min-w-0 flex-1 items-center gap-2 px-3 py-2 text-left text-sm hover:cursor-pointer", isActive ? "font-semibold text-blue-600" : "text-gray-800")}>
                                        <span className="truncate">{layoutName}</span>
                                        {isActive && <Check className="size-4 shrink-0 text-blue-600" strokeWidth={2.5} />}
                                    </button>
                                    <LayoutRowMenu
                                        layoutName={layoutName}
                                        actions={[
                                            { label: "Duplicate", icon: Copy, onSelect: () => handleDuplicate(layoutName) },
                                            { label: "Export", icon: Download, onSelect: () => handleExport(layoutName) },
                                            { label: "Rename", icon: Pencil, onSelect: () => setRenamingLayoutName(layoutName) },
                                            { label: "Delete", icon: Trash2, onSelect: () => handleDelete(layoutName), isDestructive: true },
                                        ]}
                                    />
                                </div>
                            );
                        })}
                    </div>

                    {hasUnsavedChanges && (
                        <div className="mt-1 flex items-center gap-1 border-t border-gray-200 px-1 pt-1">
                            {isNamingNewLayout ? (
                                <LayoutNameInput initialName="" onSubmit={handleSaveNewLayout} />
                            ) : (
                                <>
                                    <span className="flex flex-1 items-center gap-1.5 px-2 text-xs text-gray-500">
                                        <span className="size-1.5 shrink-0 rounded-full bg-blue-600" />
                                        Unsaved changes
                                    </span>
                                    <button type="button" onClick={handleRevert} className="whitespace-nowrap rounded px-2 py-1.5 text-sm font-semibold text-gray-800 transition-colors hover:cursor-pointer hover:bg-gray-100">
                                        Revert
                                    </button>
                                    <button type="button" onClick={handleSave} className="whitespace-nowrap rounded bg-blue-600 px-3 py-1.5 text-sm font-semibold text-white transition-colors hover:cursor-pointer hover:bg-blue-700">
                                        Save
                                    </button>
                                </>
                            )}
                        </div>
                    )}
                </PopoverContent>
            </Popover>

            <input ref={importInputRef} type="file" accept=".json,application/json" onChange={handleImportFile} className="hidden" />

            <ConfirmationDialog confirmation={confirmation} onClose={() => setConfirmation(null)} />
        </>
    );
};

export default LayoutManager;
