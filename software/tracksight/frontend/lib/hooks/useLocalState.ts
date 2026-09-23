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

  useEffect(() => {
    if (!isInitialized) {
      return;
    }

    try {
      localStorage.setItem(name, ser(state));
    } catch (error) {
      console.error(`Failed to save localStorage key \"${name}\"`, error);
    }
  }, [state, isInitialized, name, ser]);

  return [state, setState, isInitialized];
}
