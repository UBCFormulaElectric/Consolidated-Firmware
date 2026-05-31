"use client";

import { Database, FileIcon, FileUp } from "lucide-react";

import PanelEmptyState from "./PanelEmptyState";

const PANEL_SIZE = { flexBasis: "33.3333%", width: "33.3333%" } as const;

type SDCardFile = string;

type SDCardFileInfoProps = {
  selectedFiles: SDCardFile[];
  onDump: () => void;
  onClear: () => void;
  isDumping: boolean;
};

const SDCardFileInfoContent = (props: { selectedFiles: SDCardFile[] }) => {
  const { selectedFiles } = props;

  if (selectedFiles.length === 0) {
    return (
      <PanelEmptyState
        Icon={FileIcon}
        title="No files selected."
        description="Click on files from the list on the left to see details and dump options."
      />
    );
  }

  return (
    <div className="flex flex-col gap-2 h-full">
      <ul className="mb-2 flex flex-col gap-2 max-h-96 h-full overflow-y-scroll scrollbar-hidden">
        {selectedFiles.map((file) => (
          <li key={file} className="flex gap-2 items-center bg-gray-100 p-2 rounded">
            <FileUp className="mb-1 size-6 text-gray-500" />
            <div className="flex justify-between flex-col">
              <span>{file.split("/").pop()}</span>
            </div>
          </li>
        ))}
      </ul>
    </div>
  );
};

const SDCardFileInfo = (props: SDCardFileInfoProps) => {
  const { selectedFiles, onDump, onClear, isDumping } = props;

  const hasFiles = selectedFiles.length > 0;

  return (
    <div
      className="border border-black rounded h-full flex flex-col overflow-hidden"
      style={PANEL_SIZE}
    >
      <span className="font-bold text-lg border-b flex flex-row justify-between border-black w-full p-4 bg-gray-100">
        Selected Files
      </span>
      <div className="p-4 h-full">
        <SDCardFileInfoContent selectedFiles={selectedFiles} />
      </div>
      <div className="my-auto h-full" />
      <div className="border-t border-black w-full" />
      <div className="flex gap-2 flex-col p-4">
        <button
          className="border-blue-500 border bg-blue-200/50 w-full flex items-center justify-center gap-2 p-4 hover:bg-blue-200/70 rounded hover:cursor-pointer disabled:cursor-not-allowed disabled:bg-blue-100/50 disabled:border-blue-300 disabled:text-gray-600"
          onClick={onDump}
          disabled={!hasFiles || isDumping}
        >
          <Database className="size-6" />
          {isDumping ? "Dumping..." : "Dump to Database"}
        </button>
        <button
          className="border-gray-500 border bg-gray-200/50 w-full flex items-center justify-center gap-2 p-4 hover:bg-gray-200/70 rounded hover:cursor-pointer disabled:cursor-not-allowed disabled:bg-gray-100/50 disabled:border-gray-300 disabled:text-gray-600"
          onClick={onClear}
          disabled={!hasFiles || isDumping}
        >
          Clear Selection
        </button>
      </div>
    </div>
  );
};

export default SDCardFileInfo;
