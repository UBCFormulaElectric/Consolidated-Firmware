'use client';
import { Dispatch, SetStateAction, useState, useEffect } from 'react';

// TODO move to a util folder?
export function useLocalState<T>(name: string, defaultValue: T): [T, Dispatch<SetStateAction<T>>, boolean] {
	const [state, setState] = useState<T>(defaultValue);
	const [isInitialized, setIsInitialized] = useState(false);
	useEffect(() => {
		if (typeof window === "undefined") {
			return;
		}
		const saved = localStorage.getItem(name);
		if (saved !== null) {
			try {
				const saved_json: unknown = JSON.parse(saved);
				setState(saved_json as T);
			} catch (e) {
				console.error("Failed to load widgets from local storage", e);
			}
		}
		setIsInitialized(true);
	}, [name]);
	const setLocalState: Dispatch<SetStateAction<T>> = (value) => {
		localStorage.setItem(name, JSON.stringify(value));
		setState(value);
	};
	return [state, setLocalState, isInitialized];
}
