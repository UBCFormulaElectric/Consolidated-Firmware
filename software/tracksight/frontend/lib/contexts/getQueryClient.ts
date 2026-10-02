import { isServer, QueryClient } from "@tanstack/react-query";

const makeQueryClient = () =>
    new QueryClient({
        defaultOptions: {
            queries: {
                staleTime: 60 * 1000,
                refetchOnWindowFocus: false,
            },
        },
    });

let browserQueryClient: QueryClient | undefined;

export const getQueryClient = (): QueryClient => {
    if (isServer) {
        return makeQueryClient();
    }

    if (!browserQueryClient) {
        browserQueryClient = makeQueryClient();
    }

    return browserQueryClient;
};
