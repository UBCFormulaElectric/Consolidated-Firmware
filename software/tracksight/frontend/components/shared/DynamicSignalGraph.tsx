// DynamicSignalGraph.tsx
'use client';

import React from 'react';
import { SignalProvider, useSignals } from '@/lib/contexts/SignalContext';
import NumericalGraphComponent from '@/components/pages/live-data/Numerical';
import EnumerationGraphComponent from '@/components/pages/live-data/Enumeration';

interface DynamicSignalGraphProps {
  signalName: string;
  onDelete: () => void;
}

const DynamicSignalGraph: React.FC<DynamicSignalGraphProps> = ({ signalName, onDelete }) => {
  const { isEnumSignal } = useSignals();
  return isEnumSignal(signalName)
    ? <EnumerationGraphComponent signalName={signalName} onDelete={onDelete} />
    : <NumericalGraphComponent signalName={signalName} onDelete={onDelete} />;
};

export default DynamicSignalGraph;