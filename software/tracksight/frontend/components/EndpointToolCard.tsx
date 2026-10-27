import { useState } from "react";

import { API_BASE_URL } from "@/lib/constants";
import ToolCard from "./ToolCard";
import AlertModal from "./common/AlertModal";

type EndpointToolCardProps = {
    title: string;
    description: string;
    endpoint: string;
    icon: React.ReactNode;
};

function EndpointToolCard({ title, description, endpoint, icon }: EndpointToolCardProps) {
    const [showAlert, setShowAlert] = useState(false);
    const [errorMessage, setErrorMessage] = useState("");

    const handleSyncClick = async () => {
        setErrorMessage("");

        try {
            const response = await fetch(`${API_BASE_URL}/${endpoint}`);

            if (!response.ok) {
                throw new Error(`${title} failed with status ${response.status}`);
            }
        } catch (error) {
            setErrorMessage(`${title} failed: ${error instanceof Error ? error.message : "Unknown error"}`);
        } finally {
            setShowAlert(true);
        }
    };

    return (
        <>
            <ToolCard title={title} description={description} type="button" onClick={handleSyncClick} icon={icon} />

            {showAlert && (
                <AlertModal
                    title={errorMessage ? `${title} Error` : `${title} Success`}
                    errorMessage={errorMessage || `Successfully completed ${title}.`}
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

export default EndpointToolCard;
