"use client";

import Card from "@/components/common/Card";
import StateTimeline from "@/components/widgets/StateTimeline";
import { EditModeProvider } from "@/lib/contexts/EditModeContext";

export default function LiveDataPage() {
  return (
    <EditModeProvider>
      <div className="overflow-y-scroll p-12 w-screen">
        <Card
          title="Battery Information"
        >
          <StateTimeline
            signals={["VC_STATE_ONE", "VC_STATE_TWO", "VC_STATE_THREE", "VC_STATE_FOUR"]}
            options={{
              colorPalette: [
                "#FF637E",
                "#FFB86A",
                "#05DF72",
                "#51A2FF"
              ]
            }}
          /> 
        </Card>
      </div>
    </EditModeProvider>
  );
}
