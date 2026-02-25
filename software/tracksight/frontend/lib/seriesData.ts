export class SeriesData {
	data: Array<number> = [];

	constructor() { }

	get(i: number): number {
		return this.data[i];
	}

	get_min(l: number, r: number): number {
		throw new Error("Method not implemented.");
	}

	get_max(l: number, r: number): number {
		throw new Error("Method not implemented.");
	}

	insert(data: number, position: number) {
		throw new Error("Method not implemented.");
	}
};