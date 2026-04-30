import { useMutation } from "@tanstack/react-query";
import { API_BASE_URL } from "../constants";

const ERROR_CODES = {
    CONFLICT: "Could not dump file as doing so would overwrite an existing file",
    MALFORMED: "Could not dump file as it doesn't exists",
};

export class DumpSDCardFileError extends Error {
    statusCode: number;

    constructor(statusCode: number, message: string) {
        super(message);
        this.name = "DumpSDCardFileError";
        this.statusCode = statusCode;
    }
}

const useDumpSDCardFile = () => {
  return useMutation({
        mutationFn: async (body: { drive: string; file: string; overwrite?: boolean }) => {
            console.log(body);
            const response = await fetch(`${API_BASE_URL}/api/mock/sd/dump`, {
                body: JSON.stringify(body),
                headers: {
                    "Content-Type": "application/json",
                },
                method: "POST",
            });

            if (response.status === 409) {
                throw new DumpSDCardFileError(response.status, ERROR_CODES.CONFLICT);
            }

            if (response.status === 400) {
                throw new DumpSDCardFileError(response.status, ERROR_CODES.MALFORMED);
            }

            if (!response.ok) {
                throw new DumpSDCardFileError(response.status, response.statusText || "Failed to dump file");
            }

            const result = await response.text();

            return result;
    },
  });
};

export default useDumpSDCardFile;
