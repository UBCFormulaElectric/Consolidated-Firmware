import { Calendar } from "lucide-react";

function Navbar() {
  return (
    <nav className="fixed top-0 left-0 z-50 h-min w-screen">
      <div className="flex flex-row items-center justify-between select-none px-8 py-4">
        {/** TODO(evan): Make this dynamic and a selector **/}
        <div className="hover:bg-gray-300 flex items-center gap-4 rounded-lg px-4 py-4 transition-colors hover:cursor-pointer">
          <Calendar className="stroke-current" size={24} />
          <div className="flex flex-col items-start">
            <span className="text font-semibold">September 28th 2025</span>
            <span className="text-sm opacity-50">Session 1</span>
          </div>
        </div>

        {/** TODO(evan): Add pause / play button here */}
      </div>
      {/** TODO(evan): Add the universal timeline here */}
    </nav>
  );
}

export default Navbar;
