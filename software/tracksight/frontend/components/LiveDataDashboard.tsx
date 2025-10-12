"use client";

import { useDashboardLayout } from "@/lib/contexts/DashboardLayout"
import Card from "@/components/common/Card";
import Widget from "@/components/widgets/Widget";
import { useEditMode } from "@/lib/contexts/EditModeContext";

const LiveDataDashboard: React.FC = () => {
  const { cards } = useDashboardLayout();

  return (
    <div className="flex flex-col w-full h-full">
      {
        cards.map((card) => (
          <Card
            title={card.title}
          >
            {
              card.widgets.map((widget) => (
                <Widget {...widget} />
              ))
            }
          </Card>
        ))
      }
    </div>
  )
}

export default LiveDataDashboard;
