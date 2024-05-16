'use client';

import { useState } from "react";
import { ThemeContext } from './Theme';
import { Theme } from './Theme';

const ThemeProvider = ({ children }: {
    children: React.ReactNode
}) => {
    const [theme, setTheme] = useState(Theme.LIGHT);
    return (
        <ThemeContext.Provider value={{theme, setTheme}}>
            <div id="themeHandler" className={`${theme === Theme.LIGHT ? "light" : "dark"}`}>
                {children}
            </div>
        </ThemeContext.Provider>
    );
}

export default ThemeProvider;
