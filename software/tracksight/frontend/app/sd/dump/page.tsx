"use client";

import { useEffect, useRef, useState } from "react";

import useListSDCards from "@/lib/hooks/useListSDCards";
import useSDCardFiles from "@/lib/hooks/useSDCardFiles";
import useDumpSDCardFile, { DumpSDCardFileError } from "@/lib/mutations/useDumpSDCardFile";

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

const SDCardFileInfo = (props: SDCardFileInfoPropsMulti) => {
  const { selectedFiles, onDump, onClear } = props;

  if (!selectedFiles || selectedFiles.length === 0) {
    return (
      <div className="bg-black/5 p-4 h-full" style={{ flexBasis: "33.3333%", width: "33.3333%" }}>
        No files selected.
        Select files from the list to dump their contents to the database.
      </div>
    );
  }

  return (
    <div className="bg-black/5 p-4 h-full flex flex-col gap-2" style={{ flexBasis: "33.3333%", width: "33.3333%" }}>
      <span className="font-bold text-2xl">Selected Files:</span>
      <ul className="mb-2">
        {selectedFiles.map((file) => (
          <li key={file} className="flex flex-col">
            File Name: {file} <br />
            <hr className="my-2" />
          </li>
        ))}
      </ul>
      <div className="my-auto h-full" />
      <div className="flex gap-2">
        <button className="bg-gray-300 w-full text-black px-4 py-2 rounded hover:bg-gray-400 hover:cursor-pointer mr-2" onClick={onClear}>
          Clear Selection
        </button>
        <button className="bg-blue-500 w-full text-white px-4 py-2 rounded hover:bg-blue-600 hover:cursor-pointer" onClick={onDump}>
          Dump to Database
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
    <div className="bg-black/5 p-4 h-full flex flex-col" style={{ flexBasis: "66.6667%", width: "66.6667%" }}>
      <span className="font-bold text-2xl">
        Files
      </span>
      <div className="mt-4 grid grid-cols-2 gap-4">
        {files.map((file) => {
          const isSelected = selectedFiles.includes(file);

          return (
            <div
              key={file}
              className={`h-min p-4 select-none w-full rounded bg-black/5 hover:cursor-pointer ${isSelected ? "bg-blue-500 text-white" : "hover:bg-gray-200"}`}
              onClick={() => onToggleFile(file)}
            >
              <div className="flex justify-between">
                <span>{file}</span>
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
          className={`rounded px-4 py-2 text-left flex flex-col transition-colors hover:cursor-pointer ${
            selected === sdCard
              ? "bg-blue-500 text-white"
              : "bg-black/5 hover:bg-black/10"
          }`}
        >
          <span className="font-semibold text-sm">SD Card</span>
          <span className={`text-xs font-mono ${selected === sdCard ? "text-blue-100" : "text-gray-500"}`}>{sdCard}</span>
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
      <SDCardPicker selected={selectedSDCard} onSelect={setSelectedSDCard} sdCards={sdCards} isLoading={isLoading} error={error} />
      <div className="flex-1 flex h-full" style={{ height: "100%" }}>
        <SDCardFileNavigator sdCard={selectedSDCard} />
      </div>
    </div>
  );
}
