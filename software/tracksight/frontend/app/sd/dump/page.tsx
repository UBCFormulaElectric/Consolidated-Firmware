"use client";

import { useEffect, useRef, useState } from "react";

type SDCardFile = {
    name: string;
    date: string;
};

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
    
    options: {
        label: string;
        onClick: () => void;
    }[];
};

const SDCardDumpErrorModal = (props: SDCardDumpErrorModalProps) => {
    const { errorMessage, options, onCancel } = props;
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
                    <li key={file.name} className="flex flex-col">
                        File Name: {file.name} <br />
                        Date: {file.date}
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
};

const SDCardFileList = (props: SDCardFileListPropsMulti) => {
    const { files, selectedFiles, onToggleFile } = props;

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
                    const isSelected = selectedFiles.some(f => f.name === file.name);

                    return (
                        <div
                            key={file.name}
                            className={`h-min p-4 select-none w-full rounded bg-black/5 hover:cursor-pointer ${isSelected ? "bg-blue-500 text-white" : "hover:bg-gray-200"}`}
                            onClick={() => onToggleFile(file)}
                        >
                            <div className="flex justify-between">
                                <span>{file.name}</span>
                                <span>{file.date}</span>
                            </div>
                        </div>
                    );
                })}
            </div>
        </div>
    );
};

const SDCardFileNavigator = () => {
    const MOCK_FILES: SDCardFile[] | null = [
        { name: "file1.txt", date: "2024-06-01" },
        { name: "file2.txt", date: "2024-06-02" },
        { name: "file3.txt", date: "2024-06-03" },
        { name: "file4.txt", date: "2024-06-04" },
        { name: "file5.txt", date: "2024-06-05" },
    ];
    
    // const MOCK_FILES = null;

    // const MOCK_FILES: SDCardFile[] = [];

    const [selectedFiles, setSelectedFiles] = useState<SDCardFile[]>([]);
    const [dumpError, setDumpError] = useState<{
        message: string;
        statusCode: number;
    } | null>(null);

    const handleToggleFile = (file: SDCardFile) => {
        setSelectedFiles((prev) => {
            const exists = prev.some(f => f.name === file.name);
            if (exists) {
                return prev.filter(f => f.name !== file.name);
            } else {
                return [...prev, file];
            }
        });
    };

    const dumpFile = (file: SDCardFile, overwrite: boolean = false): {
        success: true
    } | {
        success: false;
        statusCode: number;
    } => {
        const randomOutcome = Math.random();

        if (randomOutcome < 0.5 || overwrite) {
            return { success: true };
        } else {
            return {
                success: false, 
                statusCode: 409,
            }
        }
    }

    const handleDump = (overwrite: boolean = false) => {
        const newSelectedFiles = [...selectedFiles];
        let hasError = overwrite;

        for (const file of selectedFiles) {
            const result = dumpFile(file, hasError);

            if (result.success) {
                newSelectedFiles.splice(newSelectedFiles.findIndex(f => f.name === file.name), 1);

                setDumpError(null);
                hasError = false;
                
                continue;
            }
            
            switch (result.statusCode) {
                case 409:
                    setDumpError({
                        message: SD_CARD_ERROR_MESSAGES[409].replace("{{fileName}}", file.name),
                        statusCode: 409,
                    });
                    break;
               
                default:
                    setDumpError({
                        message: `Failed to dump ${file.name} due to an unknown error.`,
                        statusCode: result.statusCode,
                    });
            }

            console.error(`Error dumping file ${file.name}:`, result);
            break;
        }

        setSelectedFiles(newSelectedFiles);
    };

    return (
        <div className="w-full h-full flex flex-row gap-8" style={{ height: "100%" }}>
            <SDCardFileList files={MOCK_FILES} selectedFiles={selectedFiles} onToggleFile={handleToggleFile} />
            <SDCardFileInfo selectedFiles={selectedFiles} onDump={() => handleDump(false)} onClear={() => setSelectedFiles([])} />

            {dumpError && (
                <SDCardDumpErrorModal
                    errorMessage={dumpError.message}
                    onCancel={() => {
                        setDumpError(null);
                    }}
                    options={[
                        {
                            label: "Ignore and Proceed",
                            onClick: () => {
                                setDumpError(null);
                                setSelectedFiles((selectedFiles) => selectedFiles.splice(selectedFiles.findIndex(f => f.name === selectedFiles[0].name), 1));
                                handleDump(false);
                            }
                        },
                        {
                            label: "Proceed",
                            onClick: () => {
                                handleDump(true);
                            },
                        },
                    ]}
                />
            )}
        </div>
    );
};

export default function HomePage() {
  return (
    <div className="px-14 gap-8 min-h-screen flex flex-col pb-20 overflow-hidden" style={{ height: "100vh" }}>
        <div className="pt-20 w-screen" />
        <span className="text-3xl font-bold">
            Dump SD Card
        </span>
        <div className="flex-1 flex h-full" style={{ height: "100%" }}>
            <SDCardFileNavigator />
        </div>
    </div>
  );
}
