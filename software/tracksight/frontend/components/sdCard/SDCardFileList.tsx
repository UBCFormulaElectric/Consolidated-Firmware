"use client";

import { FileDigitIcon } from "lucide-react";

const PANEL_SIZE = { flexBasis: "66.6667%", width: "66.6667%" } as const;

type SDCardFile = string;

type SDCardFileListProps = {
  files: SDCardFile[] | null;
  selectedFiles: SDCardFile[];
  onToggleFile: (file: SDCardFile) => void;
  isLoading: boolean;
  noSDCardSelected: boolean;
  hasError: boolean;
};

const SDCardFileListFrame = (props: { children: React.ReactNode }) => {
  return (
    <div
      className="border border-black rounded h-full flex flex-col overflow-y-scroll scrollbar-hidden"
      style={PANEL_SIZE}
    >
      <span className="font-bold text-lg border-b border-black w-full p-4 bg-gray-100">Files</span>
      {props.children}
    </div>
  );
};

const SDCardFileListMessage = (props: { children: React.ReactNode; tone?: "muted" | "error" }) => {
  const tone = props.tone ?? "muted";
  const toneClasses = tone === "error" ? "text-red-500" : "text-gray-400";

  return (
    <SDCardFileListFrame>
      <div className={`p-4 h-full flex items-center justify-center ${toneClasses}`}>
        {props.children}
      </div>
    </SDCardFileListFrame>
  );
};

const SDCardFileListSkeleton = () => {
  return (
    <SDCardFileListFrame>
      <div className="flex flex-col items-center justify-center gap-2 p-4 animate-pulse">
        Loading files...
      </div>
    </SDCardFileListFrame>
  );
};

const SDCardFileList = (props: SDCardFileListProps) => {
  const { files, selectedFiles, onToggleFile, isLoading, noSDCardSelected, hasError } = props;

  if (noSDCardSelected) {
    return <SDCardFileListMessage>Select an SD card above to continue.</SDCardFileListMessage>;
  }

  if (isLoading) {
    return <SDCardFileListSkeleton />;
  }

  if (hasError) {
    return (
      <SDCardFileListMessage tone="error">
        Failed to read files from this SD card. Please reconnect it and try again.
      </SDCardFileListMessage>
    );
  }

  if (!files) {
    return <SDCardFileListMessage>No SD card detected. Please insert an SD card to view files.</SDCardFileListMessage>;
  }

  if (files.length === 0) {
    return <SDCardFileListMessage>No files found on SD card.</SDCardFileListMessage>;
  }

  return (
    <SDCardFileListFrame>
      <div className="flex flex-col">
        {files.map((file) => {
          const isSelected = selectedFiles.includes(file);

          return (
            <div
              key={file}
              className={`h-min p-4 select-none border-b flex flex-row items-center gap-4 border-black w-full hover:cursor-pointer ${
                isSelected ? "bg-blue-100" : "hover:bg-blue-100/50"
              }`}
              onClick={() => onToggleFile(file)}
            >
              <input type="checkbox" checked={isSelected} readOnly className="accent-blue-500" />
              <FileDigitIcon
                className={`mb-1 size-8 ${isSelected ? "text-blue-500" : "opacity-75"}`}
              />
              <div className="flex justify-between flex-col">
                <span>{file.split("/").pop()}</span>
                <span className="text-xs text-gray-500 font-mono">
                  {file.split("/").slice(0, -1).join("/") || "/"}
                </span>
              </div>
            </div>
          );
        })}
      </div>
    </SDCardFileListFrame>
  );
};

export default SDCardFileList;
