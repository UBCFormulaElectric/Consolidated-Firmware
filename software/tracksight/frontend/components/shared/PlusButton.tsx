'use client';

import React from 'react';
import { Plus } from 'lucide-react';

export type PlusVariant = 'insertionCenter' | 'rowSide';

interface PlusButtonProps {
  onClick?: () => void;
  variant?: PlusVariant;
}

export const PlusButton: React.FC<PlusButtonProps> = ({
  onClick,
  variant = 'insertionCenter',
}) => {
  const base = 'flex items-center justify-center cursor-pointer rounded-full transition-colors';

  if (variant === 'rowSide') {
    return (
      <button onClick={onClick} className={`${base} w-8 h-8 bg-green-500 text-white`}>
        <Plus size={16} />
      </button>
    );
  }

  // insertionCenter
  return (
    <button
      onClick={onClick}
      className={`${base} w-10 h-10 bg-gray-400 text-white group-hover:bg-green-500`}
    >
      <Plus size={20} />
    </button>
  );
};