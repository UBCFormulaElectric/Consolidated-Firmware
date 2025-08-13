// DynamicSignalGraph.tsx
'use client';

import React from 'react';
import NumericalGraphComponent from '@/components/pages/live-data/Numerical';
import EnumerationGraphComponent from '@/components/pages/live-data/Enumeration';

interface DynamicSignalGraphProps {
  signalName: string;
  onDelete: () => void;
}

function isEnumSignal(signalName: string) {
  return false;
}

const DynamicSignalGraph: React.FC<DynamicSignalGraphProps> = ({ signalName, onDelete }) => {
  return isEnumSignal(signalName)
    ? <EnumerationGraphComponent signalName={signalName} onDelete={onDelete} />
    : <NumericalGraphComponent signalName={signalName} onDelete={onDelete} />;
};

export default DynamicSignalGraph;