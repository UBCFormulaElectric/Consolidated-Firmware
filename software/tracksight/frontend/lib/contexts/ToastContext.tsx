"use client";

import { CheckCheckIcon, CircleCheck, CircleX, Info, X } from "lucide-react";
import { createContext, ReactNode, useContext, useRef, useState } from "react";

type ToastContextType = {
    notify: (message: string, title: string, type?: "success" | "error" | "info") => void;
};

const ToastContext = createContext<ToastContextType | null>(null);

export const useToast = () => {
  const context = useContext(ToastContext);

  if (!context) {
    throw new Error("useToast must be used within a ToastProvider");
  }

  return context;
};

type ToastProviderProps = {
    children: ReactNode;
};

export const ToastProvider: React.FC<ToastProviderProps> = ({ children }) => {
    const toastRef = useRef<HTMLDivElement>(null);

    const [toasts, setToasts] = useState<{ 
        id: number; 
        message: string; 
        title: string; 
        type: "success" | "error" | "info",
        isVisible?: boolean;
    }[]>([]);

    const notify = (title: string, message: string, type: "success" | "error" | "info" = "info") => {
        const id = Math.random();

        setToasts((prev) => {
            const newToasts = [...prev, { id, message, title, type, isVisible: true }];

            while (newToasts.map((toast) => toast.isVisible).filter(Boolean).length > 5) {
                const oldestVisibleToastIndex = newToasts.findIndex((toast) => toast.isVisible);

                if (oldestVisibleToastIndex === -1) break;

                newToasts[oldestVisibleToastIndex].isVisible = false;
                setTimeout(() => {
                    setToasts((prev) => prev.filter((toast) => toast.id !== newToasts[oldestVisibleToastIndex].id));
                }, 300);
            }

            return newToasts;
        });

        setTimeout(() => {
            setToasts((prev) => prev.map((toast) => toast.id === id ? { ...toast, isVisible: false } : toast));

            setTimeout(() => {
                setToasts((prev) => prev.filter((toast) => toast.id !== id));
            }, 300);
        }, 3000);
    };

    return (
        <ToastContext.Provider value={{ notify }}>
            {children}
            <div ref={toastRef} className="fixed bottom-8 right-8 z-50">
                {toasts.map((toast) => (
                    <div
                        key={toast.id}
                        className={`
                            mb-4 w-96 rounded shadow-lg p-4 flex gap-4 items-start border-l-4 bg-white 
                            ${toast.type === "success" ? "border-green-500" : toast.type === "error" ? "border-red-500" : "border-blue-500"}
                            ${toast.isVisible ? "opacity-100 translate-y-0" : "opacity-0 translate-y-4"}
                            transition-all duration-300 hover:cursor-pointer group
                        `} 
                        onClick={() => {
                            setToasts((prev) => prev.map((t) => t.id === toast.id ? { ...t, isVisible: false } : t));

                            setTimeout(() => {
                                setToasts((prev) => prev.filter((t) => t.id !== toast.id));
                            }, 300);
                        }}
                    >
                        <div>
                            {
                                toast.type === "success" ? <CircleCheck className="text-green-500" /> :
                                toast.type === "error" ? <CircleX className="text-red-500" /> :
                                <Info className="text-blue-500" />
                            }
                        </div>
                        <div>
                            <span className="font-bold">
                                {toast.title}
                            </span>
                            <br />
                            <span>
                                {toast.message}
                            </span>
                        </div>
                    </div>
                ))}
            </div>
        </ToastContext.Provider>
    );
}