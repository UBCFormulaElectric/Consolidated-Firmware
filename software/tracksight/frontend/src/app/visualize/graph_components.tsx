import { toast } from "sonner"
import { FLASK_URL } from '@/app/constants';
import DropdownMenu from './dropdown_menu';
import { Dispatch, SetStateAction, useEffect, useState } from 'react';
import { FormattedData } from './timeplot';


export const getRandomColor = () => {
    const r = Math.floor(Math.random() * 256);
    const g = Math.floor(Math.random() * 256);
    const b = Math.floor(Math.random() * 256);
    return `rgb(${r},${g},${b})`;
};


export function MeasurementDropdown({ measurement, setMeasurement }: {
    measurement: string | null;
    setMeasurement: Dispatch<SetStateAction<string | null>>;
}) {
    const [allMeasurements, setAllMeasurements] = useState<string[]>([]);
    const [loading, setLoading] = useState<boolean>(true);
    useEffect(() => {
        (async () => {
            const fetchURL = `${FLASK_URL}/data/measurements`;
            const res = await fetch(fetchURL, {
                method: 'get',
            });
            if (!res.ok) {
                console.error(await res.text());
                toast.error("Error fetching measurements");
                return;
            }
            const data = await res.json();
            setAllMeasurements(data);
            setLoading(false);
        })();
    }, []);

    return (
        <DropdownMenu
            selectedOptions={measurement}
            setSelectedOptions={setMeasurement}
            options={allMeasurements}
            single={true}
            placeholder={"Measurements"}
            disabled={loading}
            loading={loading} />
    );
}


export function FieldDropdown({ fields, setFields, measurement }: {
    fields: string[];
    setFields: Dispatch<SetStateAction<string[]>>;
    measurement: string | null;
}) {
    const [allFields, setAllFields] = useState<string[]>([]);
    const [hasFetchedFields, setHasFetchedFields] = useState<boolean>(false);
    const [loading, setLoading] = useState<boolean>(false);

    useEffect(() => {
        setFields([]);
        if (measurement === null) return;
        (async () => {
            setLoading(true);
            setHasFetchedFields(false);
            try {
                const res = await fetch(new URL(`/data/measurement/${measurement}/signals`, FLASK_URL), {
                    method: 'get',
                });
                if (!res.ok) {
                    console.error(await res.text());
                    return;
                }
                setAllFields(await res.json());
                setHasFetchedFields(true);
            } catch (error) {
                console.error(error);
            } finally {
                setLoading(false);
            }
        })();
    }, [measurement]);

    return (
        <DropdownMenu
            disabled={!hasFetchedFields}
            loading={loading}
            selectedOptions={fields}
            setSelectedOptions={setFields}
            options={allFields}
            single={false}
            placeholder="Fields" />
    );
}

export function usePlotlyFormat(setGraphTitle: (title: string) => void): [
    FormattedData[],
    Dispatch<SetStateAction<Record<string, { times: Array<string>; values: Array<number>; }>>>
] {
    const [data, setData] = useState<Record<string, { times: Array<string>; values: Array<number>; }>>({});
    const [formattedData, setFormattedData] = useState<FormattedData[]>([]);
    useEffect(() => {
        setGraphTitle(Object.keys(data).join(" + "));
        setFormattedData(Object.entries(data).map(([graphName, { times, values }]) => ({
            name: graphName,
            x: times.map(x => new Date(x)), y: values,
            type: "scatter", mode: "lines+markers", line: { color: getRandomColor() }
        })));
    }, [data]);

    return [formattedData, setData];
}