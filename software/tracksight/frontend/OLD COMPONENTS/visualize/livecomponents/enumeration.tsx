import React from "react";
import { Card } from "~/packages/ui";

export default function Enumeration(props: {
  signalName: string;
  states: string[];
}) {
  return (
    <Card className="w-full h-32 bg-slate-600 py-4">
      <div className="flex gap-10 mb-4">
        <p className="bg-blue-500 ml-5">{props.signalName}</p>
        <div className="flex gap-5">
          {props.states.map((state, index) => (
            <p key={index}>{state}</p>
          ))}
        </div>
      </div>
      <div className="w-full h-10 bg-red-400">Enumeration</div>
    </Card>
  );
}
