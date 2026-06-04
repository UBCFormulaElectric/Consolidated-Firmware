import { useMutation } from "@tanstack/react-query";
import { API_BASE_URL, IS_MOCK } from "../constants";

const SD_CARD_API_VERSION = IS_MOCK ? "mock" : "v1";

export class FormatSDCardError extends Error {
    statusCode: number;

    constructor(statusCode: number, message: string) {
        super(message);
        this.name = "FormatSDCardError";
        this.statusCode = statusCode;
    }
}

const useFormatSDCard = () => {
  return useMutation({
        mutationFn: async (body: { drive: string }) => {
            const response = await fetch(`${API_BASE_URL}/api/${SD_CARD_API_VERSION}/sd/format`, {
                body: JSON.stringify(body),
                headers: {
                    "Content-Type": "application/json",
                },
                method: "POST",
            });

            if (!response.ok) {
                const text = (
                    await response.text() 
                    || response.statusText 
                    || "Failed to format SD card"
                );

                throw new FormatSDCardError(response.status, text);
            }

            const result = await response.text();

            return result;
    },
  });
};

export default useFormatSDCard;
