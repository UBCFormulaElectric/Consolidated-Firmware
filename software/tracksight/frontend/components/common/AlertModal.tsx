"use client";

import { useEffect, useRef } from "react";

type AlertModalProps = {
    title: string;
    errorMessage: string;
    onDismiss?: () => void;

    options?: {
        label: string;
        style: "default" | "positive" | "destructive";
        onClick: () => void;
    }[];
};

const AlertModal = (props: AlertModalProps) => {
    const { title, errorMessage, options = [], onDismiss } = props;
    const modalRef = useRef<HTMLDivElement>(null);

    useEffect(() => {
        const handleClickOutside = (event: MouseEvent) => {
            if (!modalRef.current || modalRef.current.contains(event.target as Node)) return;

            onDismiss?.();
        };

        document.addEventListener("mousedown", handleClickOutside);
        return () => {
            document.removeEventListener("mousedown", handleClickOutside);
        };
    }, [onDismiss]);

    return (
        <div className="fixed inset-0 z-50 bg-black/60 backdrop-blur-[1.5px] flex items-center justify-center">
            <div
                ref={modalRef}
                className="bg-background p-6 rounded-lg shadow-lg w-full max-w-lg flex flex-col gap-4"
            >
                <div>
                    <h2 className="text-lg font-semibold mb-2">{title}</h2>
                    <p className="text-sm text-muted-foreground mb-1">{errorMessage}</p>
                </div>
                <div className="flex flex-col-reverse sm:flex-row sm:justify-end gap-2 mt-4">
                    {options.length > 0 && (
                        <div className="flex w-full sm:w-auto gap-2">
                            {options.map((option, index) => (
                                <button
                                    key={index}
                                    onClick={option.onClick}
                                    className={`flex-1 sm:flex-none px-4 py-2 rounded hover:cursor-pointer ${
                                        option.style === "positive"
                                            ? "bg-green-400 text-white hover:bg-green-500/80"
                                            : option.style === "destructive"
                                            ? "bg-red-400 text-white hover:bg-red-500/80"
                                            : "bg-gray-400 text-white hover:bg-gray-500/80"
                                    }`}
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

export default AlertModal;
