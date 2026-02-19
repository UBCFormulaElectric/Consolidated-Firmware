import { Calendar } from "lucide-react";

export default function Historic() {
	return (
		<div>
			{/** TODO(evan): Make this dynamic and a selector **/}
			{/** TODO(evan): Add the universal timeline here */}
			<div className="hover:bg-gray-300 flex items-center gap-4 rounded-lg px-4 py-4 transition-colors hover:cursor-pointer">
				<Calendar className="stroke-current" size={24} />
				<div className="flex flex-col items-start">
					<span className="text font-semibold">September 28th 2025</span>
					<span className="text-sm opacity-50">Session 1</span>
				</div>
			</div>
		</div>
	);
}