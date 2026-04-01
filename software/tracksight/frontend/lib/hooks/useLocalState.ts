"use client";
import { Dispatch, SetStateAction, useEffect, useState } from "react";

// TODO move to a util folder?
export function useLocalState<T>(
  name: string,
  defaultValue: T,
  ser: (item: T) => string = JSON.stringify,
  deser: (raw: string) => T = JSON.parse // please throw errors if the deser does not work :)
): [T, Dispatch<SetStateAction<T>>, boolean] {
  const [state, setState] = useState<T>(defaultValue);
  const [isInitialized, setIsInitialized] = useState(false);

  //loading
  useEffect(() => {
    if (typeof window === "undefined") {
      return;
    }
    const saved = localStorage.getItem(name);
    if (saved !== null && saved !== undefined) {
      try {
        setState(deser(saved));
      } catch (error) {
        console.error(
          `Failed to parse localStorage key \"${name}\" with value \"${saved}\"`,
          error
        );
        localStorage.removeItem(name);
      }
    }
    setIsInitialized(true);
  }, [name]);

  //setting
  const setLocalState: Dispatch<SetStateAction<T>> = (value) => {
    setState((previousState) => {
      const nextState = value instanceof Function ? value(previousState) : value;
      try {
        localStorage.setItem(name, ser(nextState));
      } catch (error) {
        console.error(`Failed to save localStorage key \"${name}\"`, error);
      }
      return nextState;
    });
  };

  return [state, setLocalState, isInitialized];
}
