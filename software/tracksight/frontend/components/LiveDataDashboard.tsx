"use client";

import { useDashboardLayout } from "@/lib/contexts/DashboardLayout"
import Card from "./common/Card";
import Widget from "./widgets/Widget";

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
