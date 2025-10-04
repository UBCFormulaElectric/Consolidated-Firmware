"use client";

import { SignalProvider } from "@/hooks/SignalContext";

export default function LiveDataPage() {
  return (
    <SignalProvider>
      <div className="overflow-y-scroll p-12 min-h-screen w-screen">
        Hello World!
      </div>
    </SignalProvider>
  );
}
