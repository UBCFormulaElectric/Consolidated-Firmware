type ScrollChannelState = {
  progress: number;
  windowDurationMs: number;
};

const DEFAULT_STATE: ScrollChannelState = {
  progress: 0,
  windowDurationMs: 10_000,
};

const channels = new Map<string, ScrollChannelState>();
const listeners = new Map<string, Set<() => void>>();

function clamp(value: number, min: number, max: number) {
  return Math.min(Math.max(value, min), max);
}

function getOrCreateChannel(channelId: string): ScrollChannelState {
  let state = channels.get(channelId);
  if (!state) {
    state = { ...DEFAULT_STATE };
    channels.set(channelId, state);
  }
  return state;
}

function getChannelListeners(channelId: string) {
  let subs = listeners.get(channelId);
  if (!subs) {
    subs = new Set();
    listeners.set(channelId, subs);
  }
  return subs;
}

function notify(channelId: string) {
  const subs = listeners.get(channelId);
  if (!subs) return;
  subs.forEach((cb) => cb());
}

export function setScrollProgress(channelId: string, progress: number) {
  const state = getOrCreateChannel(channelId);
  const clamped = clamp(progress, 0, 1);
  if (state.progress === clamped) return;
  const nextState = { ...state, progress: clamped };
  channels.set(channelId, nextState);
  notify(channelId);
}

export function setWindowDurationMs(channelId: string, durationMs: number) {
  const state = getOrCreateChannel(channelId);
  const clamped = Math.max(durationMs, 1);
  if (state.windowDurationMs === clamped) return;
  const nextState = { ...state, windowDurationMs: clamped };
  channels.set(channelId, nextState);
  notify(channelId);
}

export function getScrollChannelSnapshot(channelId: string): ScrollChannelState {
  return getOrCreateChannel(channelId);
}

export function subscribeScrollChannel(
  channelId: string,
  callback: () => void
) {
  const subs = getChannelListeners(channelId);
  subs.add(callback);
  return () => {
    subs.delete(callback);
  };
}

export function resetScrollChannel(channelId: string) {
  channels.set(channelId, { ...DEFAULT_STATE });
  notify(channelId);
}

