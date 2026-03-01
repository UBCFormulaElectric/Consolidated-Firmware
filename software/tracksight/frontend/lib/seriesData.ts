export class SeriesData {
	data: Array<number> = [];
	private minTree: Array<number> = [Infinity, Infinity];
	private maxTree: Array<number> = [-Infinity, -Infinity];
	private capacity = 1;

	constructor() { }

	get length(): number {
		return this.data.length;
	}

	get(i: number): number {
		return this.data[i];
	}

	get_min(l: number, r: number): number {
		if (this.data.length === 0) {
			return Infinity;
		}

		let left = Math.max(0, Math.min(l, this.data.length));
		let right = Math.max(0, Math.min(r, this.data.length));

		if (right <= left) {
			return Infinity;
		}

		left += this.capacity;
		right += this.capacity;

		let result = Infinity;
		while (left < right) {
			if (left % 2 === 1) {
				result = Math.min(result, this.minTree[left]);
				left += 1;
			}
			if (right % 2 === 1) {
				right -= 1;
				result = Math.min(result, this.minTree[right]);
			}
			left >>= 1;
			right >>= 1;
		}

		return result;
	}

	get_max(l: number, r: number): number {
		if (this.data.length === 0) {
			return -Infinity;
		}

		let left = Math.max(0, Math.min(l, this.data.length));
		let right = Math.max(0, Math.min(r, this.data.length));

		if (right <= left) {
			return -Infinity;
		}

		left += this.capacity;
		right += this.capacity;

		let result = -Infinity;
		while (left < right) {
			if (left % 2 === 1) {
				result = Math.max(result, this.maxTree[left]);
				left += 1;
			}
			if (right % 2 === 1) {
				right -= 1;
				result = Math.max(result, this.maxTree[right]);
			}
			left >>= 1;
			right >>= 1;
		}

		return result;
	}

	push(value: number): void {
		this.data.push(value);
		this.ensureCapacity(this.data.length);

		let node = this.capacity + this.data.length - 1;
		this.minTree[node] = value;
		this.maxTree[node] = value;

		node >>= 1;
		while (node > 0) {
			this.minTree[node] = Math.min(this.minTree[node * 2], this.minTree[node * 2 + 1]);
			this.maxTree[node] = Math.max(this.maxTree[node * 2], this.maxTree[node * 2 + 1]);
			node >>= 1;
		}
	}

	private ensureCapacity(requiredSize: number): void {
		if (requiredSize <= this.capacity) {
			return;
		}

		while (this.capacity < requiredSize) {
			this.capacity *= 2;
		}

		this.minTree = new Array(this.capacity * 2).fill(Infinity);
		this.maxTree = new Array(this.capacity * 2).fill(-Infinity);

		for (let i = 0; i < this.data.length; i++) {
			const node = this.capacity + i;
			this.minTree[node] = this.data[i];
			this.maxTree[node] = this.data[i];
		}

		for (let node = this.capacity - 1; node > 0; node--) {
			this.minTree[node] = Math.min(this.minTree[node * 2], this.minTree[node * 2 + 1]);
			this.maxTree[node] = Math.max(this.maxTree[node * 2], this.maxTree[node * 2 + 1]);
		}
	}
};