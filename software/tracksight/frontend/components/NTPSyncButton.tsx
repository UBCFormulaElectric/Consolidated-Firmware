import { useState } from "react";

import { API_BASE_URL } from "@/lib/constants";
import { Clock } from "lucide-react";
import AlertModal from "./common/AlertModal";

function NTPSyncButton() {
    const [isSyncing, setIsSyncing] = useState(false);

    const [showAlert, setShowAlert] = useState(false);
    const [errorMessage, setErrorMessage] = useState("");

    const handleSyncClick = async () => {
        setIsSyncing(true);
        setErrorMessage("");

        try {
            const response = await fetch(`${API_BASE_URL}/api/v1/transmit/ntp`);

            if (!response.ok) {
                throw new Error(`NTP sync failed with status ${response.status}`);
            }
        } catch (error) {
            setErrorMessage(`NTP sync failed: ${error instanceof Error ? error.message : "Unknown error"}`);
        } finally {
            setShowAlert(true);
            setIsSyncing(false);
        }
    };

    return (
        <>
            <button onClick={handleSyncClick} type="button" className="flex items-center gap-2 rounded border border-black bg-white py-1.5 pl-3 pr-2 text-left transition-colors hover:cursor-pointer hover:border-black/75">
                <Clock className="flex items-center gap-1.5 text-[0.6rem] font-semibold uppercase tracking-wide text-gray-500 size-3" />
                <span className="fmax-w-44 truncate text-sm font-semibold text-gray-900">NTP Sync</span>
            </button>

            {showAlert && (
                <AlertModal
                    title="NTP Sync Error"
                    errorMessage={errorMessage || "Successfully synced with NTP server."}
                    options={
                        errorMessage
                            ? [
                                  {
                                      label: "Dismiss",
                                      style: "default",
                                      onClick: () => {
                                          setErrorMessage("");
                                          setShowAlert(false);
                                      },
                                  },
                                  {
                                      label: "Retry",
                                      style: "positive" as const,
                                      onClick: () => {
                                          setErrorMessage("");
                                          setShowAlert(false);
                                          handleSyncClick();
                                      },
                                  },
                              ]
                            : [
                                  {
                                      label: "Okay",
                                      style: "positive",
                                      onClick: () => {
                                          setErrorMessage("");
                                          setShowAlert(false);
                                      },
                                  },
                              ]
                    }
                    onDismiss={() => {
                        setErrorMessage("");
                        setShowAlert(false);
                    }}
                />
            )}
        </>
    );
}

export default NTPSyncButton;
