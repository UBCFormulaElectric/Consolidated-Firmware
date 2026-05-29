"use client";

import { useEffect, useRef } from "react";

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
    <div className="fixed inset-0 z-50 bg-black/60 backdrop-blur-[1.5px] flex items-center justify-center">
      <div
        ref={modalRef}
        className="bg-background p-6 rounded-lg shadow-lg w-full max-w-lg flex flex-col gap-4"
      >
        <div>
          <h2 className="text-lg font-semibold mb-2">Error Dumping SD Card</h2>
          <p className="text-sm text-muted-foreground mb-1">{errorMessage}</p>
        </div>
        <div className="flex flex-col-reverse sm:flex-row sm:justify-end gap-2 mt-4">
          <button
            className="w-full sm:w-auto bg-gray-100 text-black px-4 py-2 rounded hover:bg-gray-200"
            onClick={onCancel}
          >
            Cancel
          </button>
          {options.length > 0 && (
            <div className="flex w-full sm:w-auto gap-2">
              {options.map((option, index) => (
                <button
                  key={index}
                  onClick={option.onClick}
                  className="flex-1 sm:flex-none bg-blue-500 text-white px-4 py-2 rounded hover:bg-blue-600"
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
};

export default SDCardDumpErrorModal;
