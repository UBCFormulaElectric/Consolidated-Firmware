// DynamicSignalGraph.tsx
'use client';

import React, { useMemo } from 'react';
import NumericalGraphComponent from '@/components/pages/live-data/Numerical';
import EnumerationGraphComponent from '@/components/pages/live-data/Enumeration';
import { useSignals } from '@/lib/contexts/SignalContext';

interface DynamicSignalGraphProps {
  signalName: string;
  onDelete: () => void;
}

// Latch cache: once a signal is classified we keep that decision
const classificationLatch = new Map<string, boolean>();

function useIsEnumSignal(signalName: string) {
  const { getSignalData } = useSignals() as any;
  return useMemo(() => {
    if (classificationLatch.has(signalName)) {
      return classificationLatch.get(signalName)!;
    }
    const data = getSignalData ? getSignalData(signalName) : [];
    let decided: boolean | undefined = undefined;
    for (let i = data.length - 1; i >= 0; i--) {
      const v = data[i].value;
      if (typeof v === 'string') { decided = true; break; }
      if (typeof v === 'number') { decided = false; break; }
    }
    if (decided === undefined) {
      decided = /state|mode|status|enum/i.test(signalName);
    }
    classificationLatch.set(signalName, decided);
    return decided;
  }, [getSignalData, signalName]);
}

const DynamicSignalGraph: React.FC<DynamicSignalGraphProps> = ({ signalName, onDelete }) => {
  const isEnum = useIsEnumSignal(signalName);
  return isEnum
    ? <EnumerationGraphComponent signalName={signalName} onDelete={onDelete} />
    : <NumericalGraphComponent signalName={signalName} onDelete={onDelete} />;
};

export default DynamicSignalGraph;