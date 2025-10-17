"use client";

import { ChevronLeft, ChevronRight } from "lucide-react";
import React, { useEffect, useRef, useState } from "react";

type SelectorCategory<T> = {
  label: string;
  next: SelectorCategoryNext<T>;
};

type SelectorCategoryNext<T> = () => (T | SelectorCategory<T>)[] | (T | SelectorCategory<T>)[];

type SelectorItemRenderer<T> = React.FC<{
  data: T;
  isSelected?: boolean;
}>;

type SelectorProps<T> = {
  options: (SelectorCategory<T> | T)[];

  selectedOption: T | null;
  onSelect: (option: T) => void;

  ItemRenderer: SelectorItemRenderer<T>;
  getSearchableText: (item: T) => string;

  buttonElement: React.RefObject<HTMLElement>;
};

const isSelectorCategory = <T extends any>(
  item: T | SelectorCategory<T>
): item is SelectorCategory<T> => {
  return (item as SelectorCategory<T>).label !== undefined;
};

const Selector = <T extends any>(props: SelectorProps<T>) => {
  const { options, selectedOption, onSelect, ItemRenderer, getSearchableText, buttonElement } =
    props;

  const [path, setPath] = useState<number[]>([]);
  const [isOpen, setIsOpen] = useState(false);
  const [searchQuery, setSearchQuery] = useState("");

  const dropdownRef = useRef<HTMLDivElement>(null);

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
          next: item.next(),
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
  }, [path, options]);

  useEffect(() => {
    const handleClickOutside = (event: MouseEvent) => {
      // Close dropdown if clicking outside both the dropdown and button
      if (buttonElement.current && !buttonElement.current.contains(event.target as Node)) {
        setIsOpen(false);
        setSearchQuery("");
        setPath([]);
      }
    };

    const handleButtonClick = (event: MouseEvent) => {
      if (
        !buttonElement.current ||
        !buttonElement.current.contains(event.target as Node) ||
        (dropdownRef.current && dropdownRef.current.contains(event.target as Node))
      )
        return;

      event.stopImmediatePropagation();

      setIsOpen((prev) => {
        if (prev) {
          setSearchQuery("");
          setPath([]);
        }

        return !prev;
      });
    };

    document.addEventListener("mousedown", handleClickOutside);
    buttonElement.current?.addEventListener("click", handleButtonClick);

    return () => {
      document.removeEventListener("mousedown", handleClickOutside);
      buttonElement.current?.removeEventListener("click", handleButtonClick);
    };
  }, [buttonElement, dropdownRef]);

  const flattenOptions = (
    opts: (SelectorCategory<T> | T)[],
    pathLabels: string[] = []
  ): Array<{ item: T; path: string[] }> => {
    const results: Array<{ item: T; path: string[] }> = [];

    for (const opt of opts) {
      if (isSelectorCategory(opt)) {
        const nextOpts = typeof opt.next === "function" ? opt.next() : opt.next;
        results.push(...flattenOptions(nextOpts, [...pathLabels, opt.label]));
      } else {
        results.push({ item: opt, path: pathLabels });
      }
    }

    return results;
  };

  const getFilteredOptions = (): Array<{ item: T; path: string[] }> => {
    if (!searchQuery.trim()) {
      return [];
    }

    const flattened = flattenOptions(options);
    const lowerQuery = searchQuery.toLowerCase();

    return flattened.filter(({ item, path }) => {
      const itemText = getSearchableText(item).toLowerCase();
      const pathText = path.join(" ").toLowerCase();
      return itemText.includes(lowerQuery) || pathText.includes(lowerQuery);
    });
  };

  const filteredOptions = searchQuery.trim() ? getFilteredOptions() : [];

  // TODO(evan): Completely redo the styling of this whole component.

  return (
    isOpen && (
      <div
        className="scrollbar-hidden absolute top-full z-10 mt-1 max-h-60 min-w-96 overflow-y-auto rounded border bg-white shadow-lg hover:cursor-auto"
        ref={dropdownRef}
      >
        <div className="sticky top-0 flex h-14 flex-row items-center gap-2 border-b bg-white p-2">
          {path.length > 0 ? (
            <div
              onClick={(e) => {
                e.stopPropagation();
                setPath(path.slice(0, -1));
              }}
              className="sticky top-0 flex w-full cursor-pointer items-center justify-between rounded border bg-white p-1 pr-3 font-medium hover:bg-gray-100"
            >
              <ChevronLeft size={16} />
              Back
            </div>
          ) : (
            <input
              type="text"
              placeholder="Search All..."
              value={searchQuery}
              onChange={(e) => {
                setSearchQuery(e.target.value);
                if (e.target.value.trim()) {
                  setPath([]);
                }
              }}
              onClick={(e) => {
                e.stopPropagation();
              }}
              className="w-full rounded border px-3 py-1 focus:outline-none"
            />
          )}
        </div>
        {searchQuery.trim() ? (
          // NOTE(evan): Show flattened search results
          filteredOptions.length > 0 ? (
            filteredOptions.map(({ item, path: itemPath }, index) => (
              <div key={index} className="border-b last:border-b-0">
                {itemPath.length > 0 && (
                  <div className="px-4 pt-2 pb-1 text-xs text-gray-500">{itemPath.join(" / ")}</div>
                )}
                <div
                  onClick={(e) => {
                    e.stopPropagation();
                    onSelect(item);
                    setSearchQuery("");
                    setPath([]);
                    setIsOpen(false);
                  }}
                  className="cursor-pointer px-4 py-2 hover:bg-gray-100"
                >
                  <ItemRenderer data={item} isSelected={selectedOption === item} />
                </div>
              </div>
            ))
          ) : (
            <div className="px-4 py-2 text-gray-500">No results found</div>
          )
        ) : (
          <>
            {currentOptions.map((item, index) => {
              if (isSelectorCategory(item)) {
                return (
                  <div
                    key={index}
                    onClick={(e) => {
                      e.stopPropagation();
                      setPath([...path, index]);
                    }}
                    className="flex cursor-pointer justify-between px-4 py-2 hover:bg-gray-100"
                  >
                    <div className="text-ellipsis">{item.label}</div>
                    <ChevronRight size={16} />
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
                  className="cursor-pointer px-4 py-2 hover:bg-gray-100"
                >
                  <ItemRenderer data={item} isSelected={selectedOption === item} />
                </div>
              );
            })}
          </>
        )}
      </div>
    )
  );
};

export type { SelectorItemRenderer };

export default Selector;
