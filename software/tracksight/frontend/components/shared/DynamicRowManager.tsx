"use client";

import { useDisplayControl } from "@/components/shared/PausePlayControl";
import React, { useEffect, useRef, useState, useCallback } from "react";
import { RowEditor, RowItem } from "./DropdownSearch";
import DynamicSignalGraph from "./DynamicSignalGraph";
import { InsertionBar } from "./InsertionBar";
import MockGraph, {
  MockGraphConfig,
} from "@/components/pages/live-data/MockGraph";

interface CreatedComponent {
  id: string;
  signalName?: string;
  rowIndex: number;
  isMock?: boolean;
  mockConfig?: MockGraphConfig;
}

const DynamicRowManager: React.FC = () => {
  const [rows, setRows] = useState<RowItem[]>([]);
  const [createdComponents, setCreatedComponents] = useState<
    CreatedComponent[]
  >([]);

  // Autoscroll functionality
  const { isAutoscrollEnabled, isPaused } = useDisplayControl();
  const scrollContainerRef = useRef<HTMLDivElement>(null);

  // Keep scroll position at max right when autoscroll is enabled
  useEffect(() => {
    if (!isAutoscrollEnabled || isPaused || !scrollContainerRef.current) {
      return;
    }

    const container = scrollContainerRef.current;

    // Function to scroll to the far right (instantly to stay locked)
    const scrollToRight = () => {
      const maxScrollLeft = container.scrollWidth - container.clientWidth;
      container.scrollLeft = maxScrollLeft;
    };

    // Scroll to right immediately
    scrollToRight();

    // Set up interval to maintain scroll position at exact max right
    const interval = setInterval(() => {
      const maxScrollLeft = container.scrollWidth - container.clientWidth;
      if (container.scrollLeft < maxScrollLeft) {
        container.scrollLeft = maxScrollLeft;
      }
    }, 50); // Check more frequently for responsive locking

    return () => clearInterval(interval);
  }, [isAutoscrollEnabled, isPaused]);

  const addRow = (index: number) => {
    setRows((prev) => {
      const next = [...prev];
      next.splice(index, 0, {
        isOpen: false,
        selectedSignal: undefined,
        hasCreatedComponent: false,
      });
      return next;
    });
  };
  const toggleRow = useCallback((idx: number) => {
    setRows((prev) =>
      prev.map((r, i) => (i === idx ? { ...r, isOpen: !r.isOpen } : r))
    );
  }, []);

  const selectOpt = useCallback((idx: number, signalName: string) => {
    setRows((prev) =>
      prev.map((r, i) =>
        i === idx ? { ...r, isOpen: false, selectedSignal: signalName } : r
      )
    );
  }, []);

  const deleteRow = (idx: number) => {
    // Remove any associated components first
    const componentsToDelete = createdComponents.filter(
      (comp) => comp.rowIndex === idx
    );
    componentsToDelete.forEach((comp) => deleteComponent(comp.id));

    // Remove the row
    setRows((prev) => prev.filter((_, i) => i !== idx));

    // Update row indices for remaining components
    setCreatedComponents((prev) =>
      prev.map((comp) => ({
        ...comp,
        rowIndex: comp.rowIndex > idx ? comp.rowIndex - 1 : comp.rowIndex,
      }))
    );
  };

  const createComponent = useCallback((idx: number, signalName: string) => {
    // Mark the row as having created a component
    setRows((prev) =>
      prev.map((r, i) => (i === idx ? { ...r, hasCreatedComponent: true } : r))
    );

    // Create a new component
    const newComponent: CreatedComponent = {
      id: `${signalName}-${Date.now()}`,
      signalName,
      rowIndex: idx,
      isMock: false,
    };

    setCreatedComponents((prev) => [...prev, newComponent]);
  }, []);

  const createMockComponent = useCallback(
    (idx: number, config: MockGraphConfig) => {
      // Mark the row as having created a component
      setRows((prev) =>
        prev.map((r, i) =>
          i === idx ? { ...r, hasCreatedComponent: true } : r
        )
      );

      const newComponent: CreatedComponent = {
        id: `MOCK-${config.signalName}-${Date.now()}`,
        rowIndex: idx,
        isMock: true,
        mockConfig: config,
      };

      setCreatedComponents((prev) => [...prev, newComponent]);
    },
    []
  );

  const deleteComponent = (componentId: string) => {
    // Remove the component from the list
    setCreatedComponents((prev) =>
      prev.filter((comp) => comp.id !== componentId)
    );

    // Find the associated row and mark it as not having a component
    const component = createdComponents.find((comp) => comp.id === componentId);
    if (component) {
      setRows((prev) =>
        prev.map((r, i) =>
          i === component.rowIndex ? { ...r, hasCreatedComponent: false } : r
        )
      );
    }
  };

  return (
    <div className="">
      <div className="overflow-x-scroll" ref={scrollContainerRef}>
        <div className="sticky inline-block min-w-[calc(100vw)] pb-10">
          {createdComponents.map((component) =>
            component.isMock && component.mockConfig ? (
              <MockGraph
                key={component.id}
                config={component.mockConfig}
                onDelete={() => deleteComponent(component.id)}
              />
            ) : (
              <DynamicSignalGraph
                key={component.id}
                signalName={component.signalName!}
                onDelete={() => deleteComponent(component.id)}
              />
            )
          )}

          {/* Render insertion rows */}
          {rows.map((row, idx) => (
            <RowEditor
              key={idx}
              index={idx}
              row={row}
              onToggle={toggleRow}
              onSelect={selectOpt}
              onCreateComponent={createComponent}
              onDeleteRow={deleteRow}
            />
          ))}

          {/* Only show the insertion bar if there are no rows or the last row has created a component */}
          {(rows.length === 0 ||
            rows[rows.length - 1]?.hasCreatedComponent) && (
            <div className="pb-8">
              <InsertionBar
                onInsert={() => addRow(rows.length)}
                onInsertMock={(config) => {
                  const newIndex = rows.length;

                  setRows((prev) => [
                    ...prev,
                    {
                      isOpen: false,
                      selectedSignal: undefined,
                      hasCreatedComponent: true,
                    },
                  ]);

                  const newComponent: CreatedComponent = {
                    id: `MOCK-${config.signalName}-${Date.now()}`,
                    rowIndex: newIndex,
                    isMock: true,
                    mockConfig: config,
                  };
                  setCreatedComponents((prev) => [...prev, newComponent]);
                }}
              />
            </div>
          )}
        </div>
      </div>
      {/* Render all created components first */}
    </div>
  );
};

export default DynamicRowManager;
