import { useCallback, useMemo, useSyncExternalStore } from "react";

import {
  getScrollChannelSnapshot,
  setWindowDurationMs,
  subscribeScrollChannel,
} from "@/components/shared/scroll-sync/store";

type ScrollState = ReturnType<typeof getScrollChannelSnapshot>;

type Options = {
  windowDurationMs?: number;
  requireData?: boolean;
};

export type ScrollWindow = {
  startTime: number;
  endTime: number;
  channelProgress: number;
  channelWindowMs: number;
} | null;

function computeWindow(
  timestamps: number[] | undefined,
  state: ScrollState,
  options: Options
): ScrollWindow {
  if (!timestamps || timestamps.length === 0) {
    return options.requireData === false
      ? {
        startTime: 0,
        endTime: state.windowDurationMs,
        channelProgress: state.progress,
        channelWindowMs: state.windowDurationMs,
      }
      : null;
  }

  const first = timestamps[0];
  const last = timestamps[timestamps.length - 1];
  if (!Number.isFinite(first) || !Number.isFinite(last) || first === last) {
    return null;
  }

  const actualRange = last - first;
  const requestedWindow = options.windowDurationMs ?? state.windowDurationMs;
  const windowDuration = Math.max(
    1,
    Math.min(requestedWindow, Math.max(actualRange, 1))
  );

  const maxStart = last - windowDuration;

  const span = Math.max(maxStart - first, 0);
  const start =
    span === 0 ? first : first + span * Math.min(Math.max(state.progress, 0), 1);
  const end = Math.max(start + windowDuration, start + 1);

  return {
    startTime: start,
    endTime: end,
    channelProgress: state.progress,
    channelWindowMs: windowDuration,
  };
}

export function useScrollSyncWindow(
  channelId: string,
  timestamps: number[] | undefined,
  options: Options = {}
): ScrollWindow {
  const subscribe = useCallback(
    (listener: () => void) => subscribeScrollChannel(channelId, listener),
    [channelId]
  );

  const getSnapshot = useCallback(
    () => getScrollChannelSnapshot(channelId),
    [channelId]
  );

  const scrollState = useSyncExternalStore(subscribe, getSnapshot, getSnapshot);

  return useMemo(
    () => computeWindow(timestamps, scrollState, options),
    [timestamps, scrollState, options.windowDurationMs, options.requireData]
  );
}

export function forceWindowDuration(
  channelId: string,
  durationMs: number | undefined
) {
  if (typeof durationMs !== "number" || !Number.isFinite(durationMs)) return;
  setWindowDurationMs(channelId, durationMs);
}

