import { dehydrate, HydrationBoundary } from "@tanstack/react-query";

import SDCardDumpPage from "@/components/sdCard/SDCardDumpPage";
import { getQueryClient } from "@/lib/contexts/getQueryClient";
import { listSDCardsQueryOptions } from "@/lib/hooks/useListSDCards";
import { sdCardFilesQueryOptions } from "@/lib/hooks/useSDCardFiles";
import { ToastProvider } from "@/lib/contexts/ToastContext";

export default async function SDDumpPage() {
  const queryClient = getQueryClient();

  const sdCards = await queryClient.fetchQuery(listSDCardsQueryOptions()).catch(() => null);

  if (sdCards && sdCards.length > 0) {
    await Promise.all(
      sdCards.map((card) => queryClient.prefetchQuery(sdCardFilesQueryOptions(card)))
    );
  }

  return (
    <div className="px-14 gap-8 h-screen flex flex-col pb-20 overflow-hidden">
      <div className="pt-20 w-screen" />
      <span className="text-3xl font-bold">Dump SD Card</span>
      <span className="text-sm text-gray-500">
        Select an SD card and files to dump their contents to the database.
      </span>

      <HydrationBoundary state={dehydrate(queryClient)}>
        <ToastProvider>
          <SDCardDumpPage initialSelectedSDCard={sdCards?.[0] ?? null} />
        </ToastProvider>
      </HydrationBoundary>
    </div>
  );
}
