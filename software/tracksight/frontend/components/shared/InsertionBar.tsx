'use client';

import React from 'react';
import { PlusButton } from './PlusButton';

interface InsertionBarProps {
  onInsert: () => void;
}

export const InsertionBar: React.FC<InsertionBarProps> = ({ onInsert }) => (
  <div
    onClick={onInsert}
    className="relative w-full h-8 flex items-center justify-center group cursor-pointer mt-4 sticky"
  >
    {/* background line */}
    <div className="absolute inset-0 flex items-center sticky">
      <div className="h-1 bg-gray-400 w-full group-hover:bg-green-500 transition-colors" />
    </div>
    {/* center plus */}
    <div className="relative z-10">
      <PlusButton />
    </div>
  </div>
);