'use client';
import { Dispatch, SetStateAction, useState, useEffect } from 'react';

// TODO move to a util folder?
export function useLocalState<T>(name: string, defaultValue: T): [T, Dispatch<SetStateAction<T>>] {
	const [state, setState] = useState<T>(defaultValue);
	useEffect(() => {
		if (typeof window === "undefined") {
			return;
		}
		const saved = localStorage.getItem(name);
		if (saved !== null) setState(JSON.parse(saved));
	}, []);
	const setLocalState: Dispatch<SetStateAction<T>> = (value) => {
		localStorage.setItem(name, JSON.stringify(value));
		setState(value);
	};
	return [state, setLocalState];
}
