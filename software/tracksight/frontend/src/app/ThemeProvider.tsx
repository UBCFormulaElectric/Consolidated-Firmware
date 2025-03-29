"use client";

import { useState, createContext } from "react";

export enum Theme {
  LIGHT,
  DARK,
}

export const ThemeContext = createContext({
  theme: Theme.LIGHT,
  setTheme: (theme: Theme) => {
    console.error("No theme provider found");
  },
});

export default function ThemeProvider({
  children,
}: {
  children: React.ReactNode;
}) {
  const [theme, setTheme] = useState(Theme.LIGHT);
  return (
    <ThemeContext.Provider value={{ theme, setTheme }}>
      <div
        id="themeHandler"
        className={`${theme === Theme.LIGHT ? "light" : "dark"}`}
      >
        {children}
      </div>
    </ThemeContext.Provider>
  );
}
