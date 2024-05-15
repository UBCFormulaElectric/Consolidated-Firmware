import { createContext } from 'react';

export enum Theme {
  LIGHT,
  DARK
}

export const ThemeContext = createContext({
    theme: Theme.LIGHT,
    setTheme: (theme: Theme) => {}
});