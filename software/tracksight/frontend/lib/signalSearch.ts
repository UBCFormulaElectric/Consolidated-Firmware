import { isEnumSignalMetadata, isNumericalSignalMetadata, SignalMetadata } from "@/lib/types/Signal";

// "BMS_TractiveSystemVoltage" -> ["bms", "tractive", "system", "voltage"], so a query can hit any word of a name
export function splitWords(text: string): string[] {
    return text
        .replace(/([a-z0-9])([A-Z])/g, "$1 $2")
        .replace(/([A-Z]+)([A-Z][a-z])/g, "$1 $2")
        .toLowerCase()
        .split(/[^a-z0-9%°/]+/)
        .filter(Boolean);
}

type SearchField = { words: string[]; compact: string; initials: string };

export type SearchableSignal<T extends SignalMetadata> = { signal: T; name: SearchField; other: SearchField };

function toField(words: string[]): SearchField {
    return { words, compact: words.join(""), initials: words.map((word) => word[0]).join("") };
}

export function prepareSignalSearch<T extends SignalMetadata>(signals: T[]): SearchableSignal<T>[] {
    // sorted once here so an empty query lists signals A→Z instead of in the backend's hash map order
    return [...signals]
        .sort((left, right) => left.name.localeCompare(right.name))
        .map((signal) => {
            const other = [signal.msg_name, signal.tx_node];
            if (isNumericalSignalMetadata(signal) && signal.unit) other.push(signal.unit);
            if (isEnumSignalMetadata(signal)) other.push(signal.enum_signal.enum_name);
            return { signal, name: toField(splitWords(signal.name)), other: toField(other.flatMap(splitWords)) };
        });
}

/** smallest edit distance (with adjacent swaps) between `token` and any prefix of `word`, giving up past `limit` */
function prefixEditDistance(token: string, word: string, limit: number): number {
    if (word.length < token.length - limit) return limit + 1;

    let beforePrevious: number[] = [];
    let previous = Array.from({ length: word.length + 1 }, (_, j) => j);
    for (let i = 1; i <= token.length; i++) {
        const current = [i];
        let rowMin = i;
        for (let j = 1; j <= word.length; j++) {
            const cost = token[i - 1] === word[j - 1] ? 0 : 1;
            let value = Math.min(previous[j] + 1, current[j - 1] + 1, previous[j - 1] + cost);
            if (i > 1 && j > 1 && token[i - 1] === word[j - 2] && token[i - 2] === word[j - 1]) value = Math.min(value, beforePrevious[j - 2] + 1);
            current.push(value);
            rowMin = Math.min(rowMin, value);
        }
        if (rowMin > limit) return limit + 1;
        beforePrevious = previous;
        previous = current;
    }
    return Math.min(...previous);
}

function isSubsequence(token: string, text: string): boolean {
    let index = 0;
    for (const char of text) {
        if (char === token[index]) index++;
        if (index === token.length) return true;
    }
    return false;
}

// abbreviations like "wheelspd" or "cooltmp": letters in order, starting a word and spilling into at most the next one
function isAbbreviation(token: string, words: string[]): boolean {
    return words.some((word, index) => word[0] === token[0] && isSubsequence(token, word + (words[index + 1] ?? "")));
}

// best to worst: whole word, word prefix, across words ("systemvolt"), acronym ("tsv"), inside a word, one or two typos, abbreviation
function scoreToken(token: string, field: SearchField): number {
    let best = 0;
    for (const word of field.words) {
        if (word === token) return 100;
        if (word.startsWith(token)) best = Math.max(best, 80);
        else if (word.includes(token)) best = Math.max(best, 45);
    }
    if (best >= 80) return best;
    if (best === 0 && field.compact.includes(token)) best = 60;
    if (token.length >= 2 && field.initials.includes(token)) best = Math.max(best, 55);
    if (best > 0) return best;

    if (token.length >= 4) {
        const limit = token.length >= 7 ? 2 : 1;
        for (const word of field.words) {
            const distance = prefixEditDistance(token, word, limit);
            if (distance <= limit) best = Math.max(best, 35 - 10 * distance);
        }
        if (best > 0) return best;
    }
    if (token.length >= 3 && isAbbreviation(token, field.words)) return 10;
    return 0;
}

export function tokenizeQuery(query: string): string[] {
    return query.split(/\s+/).flatMap(splitWords);
}

/**
 * Lenient ranked search: every query word has to land somewhere (signal name counts double over message,
 * node, unit and enum name), but it can be a prefix, an acronym, a typo or letters spread across a word.
 */
export function searchSignals<T extends SignalMetadata>(entries: SearchableSignal<T>[], query: string): T[] {
    const tokens = tokenizeQuery(query);
    if (tokens.length === 0) return entries.map((entry) => entry.signal);

    const queryCompact = tokens.join("");
    const scored: { signal: T; score: number }[] = [];
    for (const entry of entries) {
        let score = 0;
        for (const token of tokens) {
            const tokenScore = Math.max(scoreToken(token, entry.name) * 2, scoreToken(token, entry.other));
            if (tokenScore === 0) {
                score = 0;
                break;
            }
            score += tokenScore;
        }
        if (score === 0) continue;
        // typing the name as written ("bmstractive" or "bms_tractive") beats a scattered match
        if (entry.name.compact.startsWith(queryCompact)) score += 150;
        else if (entry.name.compact.includes(queryCompact)) score += 60;
        scored.push({ signal: entry.signal, score });
    }

    return scored.sort((left, right) => right.score - left.score || left.signal.name.length - right.signal.name.length || left.signal.name.localeCompare(right.signal.name)).map((entry) => entry.signal);
}

/** [start, end) ranges of `text` that contain a query word, for bolding matches */
export function findMatchRanges(text: string, query: string): [number, number][] {
    const lower = text.toLowerCase();
    const marked = new Array<boolean>(text.length).fill(false);
    for (const token of tokenizeQuery(query)) {
        for (let index = lower.indexOf(token); index !== -1; index = lower.indexOf(token, index + 1)) {
            marked.fill(true, index, index + token.length);
        }
    }

    const ranges: [number, number][] = [];
    marked.forEach((isMarked, index) => {
        if (!isMarked) return;
        if (ranges.length > 0 && ranges[ranges.length - 1][1] === index) ranges[ranges.length - 1][1] = index + 1;
        else ranges.push([index, index + 1]);
    });
    return ranges;
}
