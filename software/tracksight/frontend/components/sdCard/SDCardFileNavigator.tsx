"use client";

import { useEffect, useState } from "react";

import useSDCardFiles from "@/lib/hooks/useSDCardFiles";
import useDumpSDCardFile, { DumpSDCardFileError } from "@/lib/mutations/useDumpSDCardFile";

import AlertModal from "../common/AlertModal";
import SDCardFileInfo from "./SDCardFileInfo";
import SDCardFileList from "./SDCardFileList";

type SDCardFile = string;

const SD_CARD_ERROR_MESSAGES: Record<number, string> = {
  409: "Failed to dump {{fileName}}. The file will overwrite existing data in the database. If you want to proceed with dumping, please confirm.",
  500: "Failed to read {{fileName}} from the SD card. Please ensure the file is not corrupted and try again.",
};

type SDCardFileNavigatorProps = {
  sdCard: string | null;
};

const SDCardFileNavigator = (props: SDCardFileNavigatorProps) => {
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

  useEffect(() => {
    setSelectedFiles([]);
    setDumpError(null);
    setPendingDump(null);
  }, [sdCard]);

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

        setSelectedFiles((currentSelectedFiles) =>
          currentSelectedFiles.filter((selectedFile) => selectedFile !== file)
        );
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
          message:
            SD_CARD_ERROR_MESSAGES[statusCode]?.replace("{{fileName}}", file) ?? message,
          statusCode,
          fileName: file,
        });

        console.error(`Error dumping file ${file}:`, error);
        return;
      }
    }
  };

  const noSDCardSelected = !sdCard;
  const isLoadingFiles = !!sdCard && availableFiles.isLoading;
  const hasError = !!sdCard && !!availableFiles.error;
  const files = !sdCard || availableFiles.error ? null : (availableFiles.data ?? null);

  return (
    <div className="w-full h-full flex flex-row gap-8">
      <SDCardFileList
        files={files}
        selectedFiles={selectedFiles}
        onToggleFile={handleToggleFile}
        isLoading={isLoadingFiles}
        noSDCardSelected={noSDCardSelected}
        hasError={hasError}
      />
      <SDCardFileInfo
        selectedFiles={selectedFiles}
        onDump={() => {
          void handleDump(selectedFiles, false);
        }}
        onClear={() => setSelectedFiles([])}
        isDumping={dumpSDCardFile.isPending}
      />

      {dumpError && (
        <AlertModal
          title="Error Dumping SD Card File"
          errorMessage={dumpError.message}
          onDismiss={() => {
            setDumpError(null);
            setPendingDump(null);
          }}
          options={
            [
              {
                label: "Cancel",
                style: "destructive",
                onClick: () => {
                  setDumpError(null);
                  setPendingDump(null);
                },
              },
              ...(
                dumpError.statusCode === 409 && pendingDump
                  ? [
                      {
                        label: "Ignore and Proceed",
                        style: "default",
                        onClick: () => {
                          const filesToContinue = pendingDump.remainingFiles;

                          setDumpError(null);
                          setSelectedFiles((currentSelectedFiles) =>
                            currentSelectedFiles.filter(
                              (selectedFile) => selectedFile !== pendingDump.fileName
                            )
                          );
                          setPendingDump(null);
                          void handleDump(filesToContinue, false);
                        },
                      },
                      {
                        label: "Proceed",
                        style: "positive",
                        onClick: () => {
                          const filesToContinue = [pendingDump.fileName, ...pendingDump.remainingFiles];

                          setDumpError(null);
                          setPendingDump(null);
                          void handleDump(filesToContinue, true);
                        },
                      },
                    ] as const
                  : []
              )
            ]
          }
        />
      )}
    </div>
  );
};

export default SDCardFileNavigator;
