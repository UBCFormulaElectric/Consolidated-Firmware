// DynamicSignalGraph.tsx
'use client';

import React from 'react';
import { SignalProvider, useSignals } from '@/lib/contexts/SignalContext';
import NumericalGraphComponent from '../live-data/Numerical';
import EnumerationGraphComponent from '../live-data/Enumeration';

interface DynamicSignalGraphProps {
  signalName: string;
  onDelete: () => void;
}

const DynamicSignalGraphInner: React.FC<DynamicSignalGraphProps> = ({ signalName, onDelete }) => {
  const { isEnumSignal } = useSignals();
  return isEnumSignal(signalName)
    ? <EnumerationGraphComponent signalName={signalName} onDelete={onDelete} />
    : <NumericalGraphComponent signalName={signalName} onDelete={onDelete} />;
};

const DynamicSignalGraph: React.FC<DynamicSignalGraphProps> = props => (
  <SignalProvider>
    <DynamicSignalGraphInner {...props} />
  </SignalProvider>
);

export default DynamicSignalGraph;