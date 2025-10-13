"use client";

import { useEffect, useRef, useState } from "react";

type SelectorCategory<T> = {
    label: string;
    next: SelectorCategoryNext<T>;
}

type SelectorCategoryNext<T> = (
    () => (T | SelectorCategory<T>)[]
        | (T | SelectorCategory<T>)[]
)

type SelectorProps<T> = {
    options: (SelectorCategory<T> | T)[];

    selectedOption: T | null;
    onSelect: (option: T) => void;

    ItemRenderer: React.FC<{ data: T }>;
}

const isSelectorCategory = <T extends any>(item: T | SelectorCategory<T>): item is SelectorCategory<T> => {
    return (item as SelectorCategory<T>).label !== undefined;
}

const SelectorCategoryItem = <T extends any>(props: SelectorCategory<T> & {
    onClick: () => void;
}) => {
    const { label, next } = props;

    return (
        <div onClick={props.onClick}>
            {label}
        </div>
    );
};

const SelectorOptionItem = <T extends any>(props: {
    option: T;
    onClick: () => void;
}) => {
    const { option } = props;

    return (
        <div onClick={props.onClick}>
            {String(option)}
        </div>
    );
}

const Selector = <T extends any>(props: SelectorProps<T>) => {
    const {
        options,
        selectedOption,
        onSelect,
        ItemRenderer
    } = props;

    const [path, setPath] = useState<number[]>([]);
    const [isOpen, setIsOpen] = useState(false);

    // NOTE(evan): This stores the full set of options, with all next categories populated, at the current path.
    //             This is updated whenever the path changes. We don't want this to trigger a re-render,
    //             so we use a ref.
    const populatedOptions = useRef<(SelectorCategory<T> | T)[]>(options);

    // NOTE(evan): This stores the current options to be rendered, which is included within populatedOptions
    //             at the path specified by `path`.
    const [currentOptions, setCurrentOptions] = useState<(SelectorCategory<T> | T)[]>(options);

    useEffect(() => {
        // NOTE(evan): Populate the options at the level down from the current path.
        let opts = options;
        for (const index of path) {
            const item = opts[index];

            if (!isSelectorCategory(item)) {
                console.warn("Path leads to a non-category item.");
                return;
            }

            if (typeof item.next === "function") {
                opts[index] = {
                    label: item.label,
                    next: item.next()
                } as unknown as SelectorCategory<T>;

                continue;
            }

            opts = item.next;
        }
    }, [path, options]);

    useEffect(() => {
        let opts = populatedOptions.current;
        for (const index of path) {
            const item = opts[index];

            if (!isSelectorCategory(item)) {
                console.warn("Path leads to a non-category item.");
                return;
            }

            opts = typeof item.next === "function" ? item.next() : item.next;
        }

        setCurrentOptions(opts);
    }, [path, options])

    return (
        <div className="relative">
            <button
                onClick={() => setIsOpen(!isOpen)}
                className="px-4 py-2 border rounded w-48 overflow-ellipsis text-left"
            >
                {selectedOption ? <ItemRenderer data={selectedOption} /> : "Select an option"}
            </button>
            {isOpen && (
                <div className="absolute z-10 mt-1 bg-white border rounded shadow-lg max-h-60 overflow-y-auto scrollbar-hidden">
                    {
                        path.length > 0 && (
                            <div
                                onClick={(e) => {
                                    e.stopPropagation();
                                    setPath(path.slice(0, -1));
                                }}
                                className="px-4 py-2 w-full cursor-pointer hover:bg-gray-100 flex justify-between font-medium sticky top-0 bg-white"
                            >
                                <div>
                                    &lt;
                                </div>
                                <div>
                                    Back
                                </div>
                            </div>
                        )
                    }
                    {currentOptions.map((item, index) => {
                        if (isSelectorCategory(item)) {
                            return (
                                <div
                                    key={index}
                                    onClick={(e) => {
                                        e.stopPropagation();
                                        setPath([...path, index]);
                                    }}
                                    className="px-4 py-2 min-w-48 cursor-pointer hover:bg-gray-100 flex justify-between"
                                >
                                    <div className="text-ellipsis">
                                        {item.label}
                                    </div>
                                    <div>
                                        &gt;
                                    </div>
                                </div>
                            );
                        }
                        return (
                            <div
                                key={index}
                                onClick={(e) => {
                                    e.stopPropagation();
                                    onSelect(item);
                                    setPath([]);
                                    setIsOpen(false);
                                }}
                                className="px-4 py-2 cursor-pointer hover:bg-gray-100"
                            >
                                <ItemRenderer data={item} />
                            </div>
                        );
                    })}
                </div>
            )}
        </div>
    )
}

export default Selector;
