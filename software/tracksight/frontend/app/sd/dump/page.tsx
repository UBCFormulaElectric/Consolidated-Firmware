"use client";

import { useEffect, useRef, useState } from "react";

import useListSDCards from "@/lib/hooks/useListSDCards";
import useSDCardFiles from "@/lib/hooks/useSDCardFiles";
import useDumpSDCardFile, { DumpSDCardFileError } from "@/lib/mutations/useDumpSDCardFile";
import { CardSim, Database, FileDigitIcon, FileIcon, FileUp } from "lucide-react";

type SDCardFile = string;

type SDCardFileInfoPropsMulti = {
  selectedFiles: SDCardFile[];
  onDump: () => void;
  onClear: () => void;
};

const SD_CARD_ERROR_MESSAGES: Record<number, string> = {
  409: "Failed to dump {{fileName}}. The file will overwrite existing data in the database. If you want to proceed with dumping, please confirm.",
};

type SDCardDumpErrorModalProps = {
  errorMessage: string;
  onCancel: () => void;

  options?: {
    label: string;
    onClick: () => void;
  }[];
};

const SDCardDumpErrorModal = (props: SDCardDumpErrorModalProps) => {
  const { errorMessage, options = [], onCancel } = props;
  const modalRef = useRef<HTMLDivElement>(null);

  useEffect(() => {
    const handleClickOutside = (event: MouseEvent) => {
      if (modalRef.current && !modalRef.current.contains(event.target as Node)) {
        onCancel();
      }
    };

    document.addEventListener("mousedown", handleClickOutside);
    return () => {
      document.removeEventListener("mousedown", handleClickOutside);
    };
  }, [onCancel]);

  return (
    <div className="inset-0 bg-black/50 flex items-center justify-center z-50 absolute h-screen w-screen">
      <div className="bg-white p-6 rounded shadow-lg w-2xl flex flex-col justify-between gap-4" ref={modalRef}>
        <div>
          <h2 className="text-xl font-bold mb-4">Error Dumping SD Card</h2>
          <p className="mb-4">{errorMessage}</p>
        </div>
        <div className="flex flex-col gap-2">
          <button className="bg-gray-300 text-black w-full px-4 py-2 rounded hover:bg-gray-400 hover:cursor-pointer" onClick={onCancel}>
            Cancel
          </button>
          {options.length > 0 && (
            <div className="flex gap-2 justify-between">
              {options.map((option, index) => (
                <button
                  key={index}
                  onClick={option.onClick}
                  className="bg-blue-500 w-full text-white px-4 py-2 rounded hover:bg-blue-600 hover:cursor-pointer"
                >
                  {option.label}
                </button>
              ))}
            </div>
          )}
        </div>
      </div>
    </div>
  );
}

const SDCardFileInfoContent = (props: SDCardFileInfoPropsMulti) => {
  const { selectedFiles, onDump, onClear } = props;

  if (!selectedFiles || selectedFiles.length === 0) {
    return (
      <div className="h-full flex flex-col items-center gap-4 justify-center">
          <FileIcon className="size-12 text-gray-300 mt-20" />
          <div className="flex flex-col items-center gap-2">
            <span className="text-gray-500 max-w-48 text-center text-lg">No files selected.</span>
            <span className="text-gray-500 max-w-48 text-center opacity-75">Click on files from the list on the left to see details and dump options.</span>
          </div>
      </div>
    )
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
}

const SDCardFileInfo = (props: SDCardFileInfoPropsMulti) => {
  const { selectedFiles, onDump, onClear } = props;

  const hasFiles = selectedFiles && selectedFiles.length > 0;

  return (
    <div className="border border-black rounded h-full flex flex-col overflow-hidden" style={{ flexBasis: "33.3333%", width: "33.3333%" }}>
      <span className="font-bold text-lg border-b flex flex-row justify-between border-black w-full p-4 bg-gray-100">
        Selected Files
      </span>
      <div className="p-4 h-full">
        <SDCardFileInfoContent selectedFiles={selectedFiles} onDump={onDump} onClear={onClear} />
      </div>
      <div className="my-auto h-full" />
      <div className="border-t border-black w-full" />
      <div className="flex gap-2 flex-col p-4">
        <button
          className="border-blue-500 border bg-blue-200/50 w-full flex items-center justify-center gap-2 p-4 hover:bg-blue-200/70 rounded hover:cursor-pointer disabled:cursor-not-allowed disabled:bg-blue-100/50 disabled:border-blue-300 disabled:text-gray-600"
          onClick={onDump}
          disabled={!hasFiles}
        >
          <Database className="size-6" />
          Dump to Database
        </button>
        <button
          className="border-gray-500 border bg-gray-200/50 w-full flex items-center justify-center gap-2 p-4 hover:bg-gray-200/70 rounded hover:cursor-pointer disabled:cursor-not-allowed disabled:bg-gray-100/50 disabled:border-gray-300 disabled:text-gray-600"
          onClick={onClear}
          disabled={!hasFiles}
        >
          Clear Selection
        </button>
      </div>
    </div>
  );
};

type SDCardFileListPropsMulti = {
  files: SDCardFile[] | null;
  selectedFiles: SDCardFile[];
  onToggleFile: (file: SDCardFile) => void;
  noSDCardSelected: boolean;
};

const SDCardFileList = (props: SDCardFileListPropsMulti) => {
  const { files, selectedFiles, onToggleFile, noSDCardSelected } = props;

  if (noSDCardSelected) {
    return (
      <div className="bg-black/5 p-4 h-full flex items-center justify-center text-gray-400" style={{ flexBasis: "66.6667%", width: "66.6667%" }}>
        Select an SD card above to continue.
      </div>
    );
  }

  if (!files) {
    return (
      <div className="bg-black/5 p-4 h-full flex items-center justify-center" style={{ flexBasis: "66.6667%", width: "66.6667%" }}>
        No SD card detected. Please insert an SD card to view files.
      </div>
    );
  }

  if (files.length === 0) {
    return (
      <div className="bg-black/5 p-4 h-full flex items-center justify-center" style={{ flexBasis: "66.6667%", width: "66.6667%" }}>
        No files found on SD card.
      </div>
    );
  }

  return (
    <div className="border border-black rounded h-full flex flex-col overflow-y-scroll scrollbar-hidden" style={{ flexBasis: "66.6667%", width: "66.6667%" }}>
      <span className="font-bold text-lg border-b border-black w-full p-4 bg-gray-100">
        Files
      </span>
      <div className="flex flex-col">
        {files.map((file) => {
          const isSelected = selectedFiles.includes(file);

          return (
            <div
              key={file}
              className={`h-min p-4 select-none border-b flex flex-row items-center gap-4 border-black w-full hover:cursor-pointer ${isSelected ? "bg-blue-100" : "hover:bg-blue-100/50"}`}
              onClick={() => onToggleFile(file)}
            >
              <input type="checkbox" checked={isSelected} readOnly className="accent-blue-500" />
              <FileDigitIcon className={`mb-1 size-8 ${isSelected ? "text-blue-500" : "opacity-75"}`} />
              <div className="flex justify-between flex-col">
                <span>{file.split("/").pop()}</span>
                <span className="text-xs text-gray-500 font-mono">{file.split("/").slice(0, -1).join("/") || "/"}</span>
              </div>
            </div>
          );
        })}
      </div>
    </div>
  );
};

const SDCardFileNavigator = (props: { sdCard: string | null }) => {
  const { sdCard } = props;

  const availableFiles = useSDCardFiles(sdCard);
  const dumpSDCardFile = useDumpSDCardFile();

  const [selectedFiles, setSelectedFiles] = useState<SDCardFile[]>([]);
  const [dumpError, setDumpError] = useState<{
    message: string;
    statusCode: number;
    fileName: string;
  } | null>(null);
  const [pendingDump, setPendingDump] = useState<{
    fileName: string;
    remainingFiles: SDCardFile[];
  } | null>(null);

  const handleToggleFile = (file: SDCardFile) => {
    setSelectedFiles((prev) => {
      const exists = prev.includes(file);

      if (exists) {
        return prev.filter((f) => f !== file);
      } else {
        return [...prev, file];
      }
    });
  };

  const handleDump = async (filesToDump: SDCardFile[], overwrite: boolean = false) => {
    if (!sdCard || filesToDump.length === 0) {
      return;
    }

    for (let index = 0; index < filesToDump.length; index += 1) {
      const file = filesToDump[index];

      try {
        await dumpSDCardFile.mutateAsync({
          drive: sdCard,
          file,
          overwrite,
        });

        setSelectedFiles((currentSelectedFiles) => currentSelectedFiles.filter((selectedFile) => selectedFile !== file));
        setDumpError(null);
        setPendingDump(null);
        overwrite = false;
      } catch (error) {
        const statusCode = error instanceof DumpSDCardFileError ? error.statusCode : 0;
        const message = error instanceof Error ? error.message : `Failed to dump ${file}.`;

        if (statusCode === 409) {
          setPendingDump({
            fileName: file,
            remainingFiles: filesToDump.slice(index + 1),
          });
        } else {
          setPendingDump(null);
        }

        setDumpError({
          message: statusCode === 409 ? SD_CARD_ERROR_MESSAGES[409].replace("{{fileName}}", file) : message,
          statusCode,
          fileName: file,
        });

        console.error(`Error dumping file ${file}:`, error);
        return;
      }
    }
  };

  const files = !sdCard
    ? null
    : availableFiles.error
      ? null
      : (availableFiles.data ?? null);

  return (
    <div className="w-full h-full flex flex-row gap-8" style={{ height: "100%" }}>
      <SDCardFileList files={files} selectedFiles={selectedFiles} onToggleFile={handleToggleFile} noSDCardSelected={!sdCard} />
      <SDCardFileInfo selectedFiles={selectedFiles} onDump={() => { void handleDump(selectedFiles, false); }} onClear={() => setSelectedFiles([])} />

      {dumpError && (
        <SDCardDumpErrorModal
          errorMessage={dumpError.message}
          onCancel={() => {
            setDumpError(null);
            setPendingDump(null);
          }}
          options={
            dumpError.statusCode === 409 && pendingDump
              ? [
                {
                  label: "Ignore and Proceed",
                  onClick: () => {
                    const filesToContinue = pendingDump.remainingFiles;

                    setDumpError(null);
                    setSelectedFiles((currentSelectedFiles) =>
                      currentSelectedFiles.filter((selectedFile) => selectedFile !== pendingDump.fileName)
                    );
                    setPendingDump(null);
                    void handleDump(filesToContinue, false);
                  },
                },
                {
                  label: "Proceed",
                  onClick: () => {
                    const filesToContinue = [pendingDump.fileName, ...pendingDump.remainingFiles];

                    setDumpError(null);
                    setPendingDump(null);
                    void handleDump(filesToContinue, true);
                  },
                },
              ]
              : []
          }
        />
      )}
    </div>
  );
};

type SDCardPickerProps = {
  selected: string | null;
  onSelect: (sdCard: string) => void;
  sdCards: string[] | undefined;
  isLoading: boolean;
  error: unknown;
};

const SDCardPicker = (props: SDCardPickerProps) => {
  const { selected, onSelect, sdCards, isLoading, error } = props;

  if (isLoading) {
    return <div className="text-sm text-gray-400">Loading SD cards...</div>;
  }

  if (error || !sdCards) {
    return <div className="text-sm text-red-500">Failed to list SD cards.</div>;
  }

  if (sdCards.length === 0) {
    return <div className="text-sm text-gray-400">No SD cards detected. Please insert an SD card.</div>;
  }

  return (
    <div className="flex flex-row gap-3">
      {sdCards.map((sdCard) => (
        <button
          key={sdCard}
          onClick={() => onSelect(sdCard)}
          className={`rounded px-4 py-4 text-left flex gap-2 items-center transition-colors hover:cursor-pointer ${selected === sdCard
              ? "border-blue-500 border bg-blue-100"
              : "border-black hover:border-black/75 border"
            }`}
        >
          <CardSim className={`mb-1 ${selected === sdCard ? "text-blue-500" : ""}`} />
          <div className="flex flex-col leading-4">
            <span className="font-semibold text-sm">SD Card</span>
            <span className={`text-xs font-mono`}>{sdCard}</span>
          </div>
        </button>
      ))}
    </div>
  );
};

export default function HomePage() {
  const [selectedSDCard, setSelectedSDCard] = useState<string | null>(null);
  const { data: sdCards, isLoading, error } = useListSDCards();

  useEffect(() => {
    if (!sdCards || sdCards.length === 0) {
      return;
    }

    if (!selectedSDCard || !sdCards.includes(selectedSDCard)) {
      setSelectedSDCard(sdCards[0]);
    }
  }, [sdCards, selectedSDCard]);

  return (
    <div className="px-14 gap-8 min-h-screen flex flex-col pb-20 overflow-hidden" style={{ height: "100vh" }}>
      <div className="pt-20 w-screen" />
      <span className="text-3xl font-bold">Dump SD Card</span>
      <span className="text-sm text-gray-500">Select an SD card and files to dump their contents to the database.</span>
      <SDCardPicker selected={selectedSDCard} onSelect={setSelectedSDCard} sdCards={sdCards} isLoading={isLoading} error={error} />
      <div className="flex-1 flex h-full" style={{ height: "100%" }}>
        <SDCardFileNavigator sdCard={selectedSDCard} />
      </div>
    </div>
  );
}
